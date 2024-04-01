#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

#include "database.h"
#include "datastructdefines.h"
#include "table_model_tax_result.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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
    void year_tax_calc(bool add_bonus_to_total, bool pay_personal_pension);

    double total_income;
    double total_bonus;

    double tax_start_point;
    double total_pension;
    double total_medical_insurance;
    double total_unemployment_insurance;
    double total_housing_fund;
    double total_housing_loan;
    double total_house_renting;
    double total_elderly_support;
    double total_serious_illness_support;
    double total_adult_education;
    double total_children_education;
    double total_personal_pension;

    double tax_income;
    double tax_annual_bonus;
    double tax_personal_pension;
    double tax_total;

    QVector<yearly_tax_report_t> tax_report_list;

    table_model_tax_result tax_result_model;
};
#endif // MAINWINDOW_H
