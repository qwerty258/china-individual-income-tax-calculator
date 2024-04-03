#include "database.h"

#include <QSettings>
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>
#include <QDebug>

#define DATABASE_MAX_VERSION 0

extern QSettings global_settings;

QSqlDatabase db;

int database_exec_sql_file(QString path)
{
    QFile f(path);
    if (!f.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "open file error";
    }
    QTextStream in(&f);
    QSqlQuery query(db);
    QString sql = in.readAll();
    QStringList sqls = sql.trimmed().replace('\r', ' ').replace('\n', ' ').split("/*STATEMENT SEPARATOR*/");

    for (QStringList::Iterator it = sqls.begin(); it != sqls.end(); it++)
    {
        QString tmp = (*it);
        qDebug() << "SQL: " << tmp;
        if (tmp.isEmpty())
            continue;
        if (!query.exec(*it))
        {
            QMessageBox msg;
            msg.setText(path + ": " + *it + "ERROR: " + query.lastError().text());
            msg.exec();
            db.rollback();
            return -1;
        }
    }
    db.commit();
    return 0;
}

int database_create_folder()
{
    global_settings.beginGroup("database");
    QString path = global_settings.value("database_location", "").toString();
    qDebug() << path;
    if (path.isEmpty())
    {
        path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    }
    qDebug() << path;
    global_settings.setValue("database_location", path);
    global_settings.endGroup();

    QString file_path = path + "/tax.db";

    if (QFileInfo(file_path).exists())
    {
        if (!QFileInfo(file_path).isFile())
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Warning);
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setText("Database file found as folder, the folder will be trashed");
            msg.exec();
            if (!QFile::moveToTrash(file_path))
            {
                QMessageBox msg;
                msg.setIcon(QMessageBox::Critical);
                msg.setStandardButtons(QMessageBox::Ok);
                msg.setText("Trash folder error");
                msg.exec();
                return -1;
            }
        }
    }
    else
    {
        QDir dir(path);
        if (!dir.mkpath(path))
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Critical);
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setText("Create folder for database error");
            msg.exec();
            return -1;
        }
    }

    return 0;
}

int database_init()
{
    global_settings.beginGroup("database");
    QString path = global_settings.value("database_location", "").toString();
    global_settings.endGroup();
    QString file_path = path + "/tax.db";

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(file_path);
    if (!db.open())
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setText("database open error: " + db.lastError().text());
        msg.exec();
        return -1;
    }

    db.exec("PRAGMA synchronous = OFF;");
    db.exec("PRAGMA journal_mode = MEMORY;");

    size_t i = 0;

    if (db.tables().contains("version"))
    {
        QSqlQuery query(db);
        if (query.exec("SELECT * FROM version;"))
        {
            query.next();
            i = query.value(0).toUInt() + 1;
        }
        else
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Critical);
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setText("Get database version error: " + query.lastError().text());
            msg.exec();
            return -1;
        }
    }

    for (; i <= DATABASE_MAX_VERSION; i++)
    {
        QString sql_file = QString(":/database_%1.sql").arg(i, 6, 10, QChar('0'));
        if (0 != database_exec_sql_file(sql_file))
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Critical);
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setText("Execute SQL file " + sql_file + " error");
            msg.exec();
            return -1;
        }
    }

    return 0;
}

void database_uninit()
{
    if (db.isOpen())
    {
        db.close();
    }
}

table_model::table_model(QString table_name)
{
    p_sql_table_model = new QSqlTableModel(nullptr, db);
    if (nullptr == p_sql_table_model)
    {
        QMessageBox msgbox;
        msgbox.setIcon(QMessageBox::Critical);
        msgbox.setWindowTitle("Error");
        msgbox.setText("Database table model pointer ERROR");
        msgbox.setStandardButtons(QMessageBox::Ok);
        msgbox.exec();
    }
    this->table_name = table_name;
    p_sql_table_model->setTable(table_name);
    p_sql_table_model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    if (!p_sql_table_model->select())
    {
        show_error_message_box();
    }
}

table_model::~table_model()
{
    delete p_sql_table_model;
}

void table_model::show_error_message_box()
{
    QMessageBox msgbox;
    msgbox.setIcon(QMessageBox::Critical);
    msgbox.setWindowTitle("Error");
    msgbox.setText("Database ERROR: " + p_sql_table_model->lastError().text());
    msgbox.setStandardButtons(QMessageBox::Ok);
    msgbox.exec();
}

QSqlTableModel *table_model::get_table_model()
{
    return p_sql_table_model;
}

bool table_model::table_select()
{
    return p_sql_table_model->select();
}

bool table_model::table_sync()
{
    bool ret = p_sql_table_model->submitAll();
    if (ret)
    {
        p_sql_table_model->database().commit();
    }
    else
    {
        show_error_message_box();
        p_sql_table_model->revertAll();
        p_sql_table_model->database().rollback();
    }
    return ret;
}

bool table_model::table_add_record()
{
    QSqlRecord record = p_sql_table_model->record();

    if ("income" == table_name)
    {
        record.setValue("year", QDateTime::currentDateTime().date().year());
        record.setValue("month", 0);
        record.setValue("income", 0.0);
        record.setValue("bonus", 0.0);
    }
    else if ("deduction" == table_name)
    {
        record.setValue("year", QDateTime::currentDateTime().date().year());
        record.setValue("month", 0);
        record.setValue("threshold", 0.0);
        record.setValue("pension", 0.0);
        record.setValue("medical-insurance", 0.0);
        record.setValue("unemployment-insurance", 0.0);
        record.setValue("housing-fund", 0.0);
        record.setValue("housing-loan", 0.0);
        record.setValue("house-renting", 0.0);
        record.setValue("elderly-support", 0.0);
        record.setValue("serious-illness-support", 0.0);
        record.setValue("adult-education", 0.0);
        record.setValue("children-education", 0.0);
        record.setValue("personal-pension", 0.0);
    }
    else
    {
        return false;
    }

    if (p_sql_table_model->insertRecord(-1, record))
    {
        if (p_sql_table_model->submitAll())
        {
            p_sql_table_model->database().commit();
        }
        else
        {
            show_error_message_box();
        }
    }
    else
    {
        show_error_message_box();
        p_sql_table_model->database().rollback();
        return false;
    }
    return true;
}

bool table_model::table_delete_record(qint64 index)
{
    if (p_sql_table_model->removeRows(index, 1))
    {
        if (p_sql_table_model->submitAll())
        {
            p_sql_table_model->database().commit();
        }
        else
        {
            show_error_message_box();
            p_sql_table_model->database().rollback();
            return false;
        }
    }
    else
    {
        show_error_message_box();
        return false;
    }
    return true;
}
