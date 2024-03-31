#ifndef DIALOG_GLOBAL_SETTINGS_H
#define DIALOG_GLOBAL_SETTINGS_H

#include <QDialog>

namespace Ui {
class dialog_global_settings;
}

class dialog_global_settings : public QDialog
{
    Q_OBJECT

public:
    explicit dialog_global_settings(QWidget *parent = nullptr);
    ~dialog_global_settings();

private slots:
    void on_push_button_save_clicked();

    void on_push_button_cancel_clicked();

    void on_push_button_select_path_clicked();

private:
    Ui::dialog_global_settings *ui;
};

#endif // DIALOG_GLOBAL_SETTINGS_H
