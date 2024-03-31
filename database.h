#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QSqlTableModel>

int database_create_folder();
int database_init();
void database_uninit();

class table_model
{
public:
    explicit table_model(QString table_name);
    ~table_model();
    void show_error_message_box();
    QSqlTableModel *get_table_model();
    bool table_select();
    bool table_sync();
    void clear_match();
    bool table_delete_record(qint64 index);

private:
    QSqlTableModel *p_sql_table_model;
    QString table_name;
};

#endif // DATABASE_H
