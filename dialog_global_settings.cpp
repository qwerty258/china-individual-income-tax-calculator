#include "dialog_global_settings.h"
#include "ui_dialog_global_settings.h"

#include <QDebug>
#include <QSettings>
#include <QStandardPaths>
#include <QFileDialog>

extern QSettings global_settings;

dialog_global_settings::dialog_global_settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialog_global_settings)
{
    ui->setupUi(this);
    global_settings.beginGroup("database");
    QString data_location = global_settings.value("database_location", "").toString();
    global_settings.endGroup();
    if (data_location.isEmpty())
    {
        data_location = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    }
    ui->line_edit_database_path->setText(data_location);
    qDebug() << data_location;
}

dialog_global_settings::~dialog_global_settings()
{
    delete ui;
}

void dialog_global_settings::on_push_button_save_clicked()
{
    global_settings.beginGroup("database");
    global_settings.setValue("database_location", ui->line_edit_database_path->text());
    global_settings.endGroup();
}

void dialog_global_settings::on_push_button_cancel_clicked()
{
    close();
}

void dialog_global_settings::on_push_button_select_path_clicked()
{
    QString path = ui->line_edit_database_path->text();
    if (QFileInfo(path).isDir())
    {
        path = QFileDialog::getExistingDirectory(this, "Select Path", path);
    }
    else
    {
        path = QFileDialog::getExistingDirectory(this, "Select Path");
    }
    ui->line_edit_database_path->setText(path);
}
