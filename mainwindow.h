#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

typedef struct {
    double tax_point;
    double tax_rate;
} tax_level_t;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_exit_triggered();

    void on_action_export_data_as_csv_triggered();

    void on_action_settings_triggered();

    void on_action_about_triggered();

    void on_push_button_calc_clicked();

private:
    Ui::MainWindow *ui;
    table_model *p_table_income_model;
    table_model *p_table_deduction_model;
    double total_tax_calc(double total_mount, QVector<tax_level_t>& levels);
    double annual_bonus_version_2019(double total_mount);
    double total_tax_version_2018(double total_mount);
};
#endif // MAINWINDOW_H
