#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "database.h"
#include "dialog_global_settings.h"
#include "dialog_about.h"

#include <QSqlRecord>

#include <QSettings>
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

extern QSettings global_settings;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->tabWidget);
    if (0 != database_create_folder())
    {
        close();
    }

    if (0 != database_init())
    {
        close();
    }

    p_table_income_model = new table_model("income");
    if (nullptr == p_table_income_model)
    {
        // TODO
    }
    else
    {
        ui->table_view_income->setContextMenuPolicy(Qt::CustomContextMenu);
        ui->table_view_income->setModel(p_table_income_model->get_table_model());
        ui->table_view_income->setSortingEnabled(true);
        ui->table_view_income->sortByColumn(0, Qt::SortOrder::AscendingOrder);
        ui->table_view_income->resizeColumnsToContents();
        QSqlRecord record = p_table_income_model->get_table_model()->record();
        p_table_income_model->get_table_model()->setHeaderData(record.indexOf("year"), Qt::Horizontal, tr("Year"));
        p_table_income_model->get_table_model()->setHeaderData(record.indexOf("month"), Qt::Horizontal, tr("Month"));
        p_table_income_model->get_table_model()->setHeaderData(record.indexOf("income"), Qt::Horizontal, tr("Income"));
        p_table_income_model->get_table_model()->setHeaderData(record.indexOf("bonus"), Qt::Horizontal, tr("Bonus"));
    }

    p_table_deduction_model = new table_model("deduction");
    if (nullptr == p_table_deduction_model)
    {
        // TODO
    }
    else
    {
        ui->table_view_income->setContextMenuPolicy(Qt::CustomContextMenu);
        ui->table_view_deduction->setModel(p_table_deduction_model->get_table_model());
        ui->table_view_deduction->setSortingEnabled(true);
        ui->table_view_deduction->sortByColumn(0, Qt::SortOrder::AscendingOrder);
        ui->table_view_deduction->resizeColumnsToContents();
        QSqlRecord record = p_table_deduction_model->get_table_model()->record();
        p_table_deduction_model->get_table_model()->setHeaderData(record.indexOf("year"), Qt::Horizontal, tr("Year"));
        p_table_deduction_model->get_table_model()->setHeaderData(record.indexOf("month"), Qt::Horizontal, tr("Month"));
        p_table_deduction_model->get_table_model()->setHeaderData(record.indexOf("tax-deduction"), Qt::Horizontal, tr("Tax Deduction"));
        p_table_deduction_model->get_table_model()->setHeaderData(record.indexOf("pension-insurance"), Qt::Horizontal, tr("Pension Insurance"));
        p_table_deduction_model->get_table_model()->setHeaderData(record.indexOf("medical-insurance"), Qt::Horizontal, tr("Medical Insurance"));
        p_table_deduction_model->get_table_model()->setHeaderData(record.indexOf("unemployment-insurance"), Qt::Horizontal, tr("Unemployment Insurance"));
        p_table_deduction_model->get_table_model()->setHeaderData(record.indexOf("housing-fund"), Qt::Horizontal, tr("Housing Fund"));
        p_table_deduction_model->get_table_model()->setHeaderData(record.indexOf("housing-loan"), Qt::Horizontal, tr("Housing Loan"));
        p_table_deduction_model->get_table_model()->setHeaderData(record.indexOf("house-renting"), Qt::Horizontal, tr("House Renting"));
        p_table_deduction_model->get_table_model()->setHeaderData(record.indexOf("elderly-care-support"), Qt::Horizontal, tr("Elderly Care Support"));
        p_table_deduction_model->get_table_model()->setHeaderData(record.indexOf("serious-illness-support"), Qt::Horizontal, tr("Serious Illness Support"));
        p_table_deduction_model->get_table_model()->setHeaderData(record.indexOf("adult-education"), Qt::Horizontal, tr("Adult Education"));
        p_table_deduction_model->get_table_model()->setHeaderData(record.indexOf("children-education"), Qt::Horizontal, tr("Children Education"));
        p_table_deduction_model->get_table_model()->setHeaderData(record.indexOf("infant-care-support"), Qt::Horizontal, tr("Infant Care Support"));
        p_table_deduction_model->get_table_model()->setHeaderData(record.indexOf("personal-pension"), Qt::Horizontal, tr("Personal Pension"));
    }

    tax_result_model.add_tax_result(&tax_report_list);
    ui->table_view_tax_result->setModel(&tax_result_model);
    ui->table_view_tax_result->resizeColumnsToContents();

    ui->date_edit->setDate(QDateTime::currentDateTime().date());
}

MainWindow::~MainWindow()
{
    qDebug() << __FILE__ << __LINE__ << p_table_income_model->table_sync();
    qDebug() << __FILE__ << __LINE__ << p_table_deduction_model->table_sync();
    delete ui;
    if (nullptr != p_table_income_model)
    {
        delete p_table_income_model;
    }
    if (nullptr != p_table_deduction_model)
    {
        delete p_table_deduction_model;
    }
    database_uninit();
}

void MainWindow::on_action_exit_triggered()
{
    close();
}

void MainWindow::on_action_export_data_as_csv_triggered()
{
    QMessageBox msg;
    msg.setText("export csv not implemented");
    msg.exec();
}


void MainWindow::on_action_settings_triggered()
{
    dialog_global_settings settings;
    settings.exec();
}

void MainWindow::on_action_about_triggered()
{
    dialog_about about;
    about.exec();
}

void MainWindow::on_push_button_calc_clicked()
{
    int year;
    int month;
    int day;
    ui->date_edit->date().getDate(&year, &month, &day);

    yearly_tax_report_t temp;

    bool add_bonus_to_total;
    bool pay_personal_pension;

    tax_result_model.begin_update_data();
    tax_report_list.clear();

    add_bonus_to_total = false;
    pay_personal_pension = true;
    year_tax_calc(add_bonus_to_total, pay_personal_pension);
    temp.year = year;
    temp.add_bonus_to_total = add_bonus_to_total;
    temp.pay_personal_pension = pay_personal_pension;
    temp.total_income = total_income;
    temp.total_annual_bonus = total_bonus;
    temp.total_accidental_income = total_accidental_income;
    temp.tax_total_income = tax_income;
    temp.tax_annual_bonus = tax_annual_bonus;
    temp.tax_personal_pension = tax_personal_pension;
    temp.tax_accidental_income = tax_accidental_income;
    temp.tax_total = tax_total;
    tax_report_list.push_back(temp);

    add_bonus_to_total = false;
    pay_personal_pension = false;
    year_tax_calc(add_bonus_to_total, pay_personal_pension);
    temp.year = year;
    temp.add_bonus_to_total = add_bonus_to_total;
    temp.pay_personal_pension = pay_personal_pension;
    temp.total_income = total_income;
    temp.total_annual_bonus = total_bonus;
    temp.total_accidental_income = total_accidental_income;
    temp.tax_total_income = tax_income;
    temp.tax_annual_bonus = tax_annual_bonus;
    temp.tax_personal_pension = tax_personal_pension;
    temp.tax_accidental_income = tax_accidental_income;
    temp.tax_total = tax_total;
    tax_report_list.push_back(temp);

    add_bonus_to_total = true;
    pay_personal_pension = true;
    year_tax_calc(add_bonus_to_total, pay_personal_pension);
    temp.year = year;
    temp.add_bonus_to_total = add_bonus_to_total;
    temp.pay_personal_pension = pay_personal_pension;
    temp.total_income = total_income;
    temp.total_annual_bonus = total_bonus;
    temp.total_accidental_income = total_accidental_income;
    temp.tax_total_income = tax_income;
    temp.tax_annual_bonus = tax_annual_bonus;
    temp.tax_personal_pension = tax_personal_pension;
    temp.tax_accidental_income = tax_accidental_income;
    temp.tax_total = tax_total;
    tax_report_list.push_back(temp);

    add_bonus_to_total = true;
    pay_personal_pension = false;
    year_tax_calc(add_bonus_to_total, pay_personal_pension);
    temp.year = year;
    temp.add_bonus_to_total = add_bonus_to_total;
    temp.pay_personal_pension = pay_personal_pension;
    temp.total_income = total_income;
    temp.total_annual_bonus = total_bonus;
    temp.total_accidental_income = total_accidental_income;
    temp.tax_total_income = tax_income;
    temp.tax_annual_bonus = tax_annual_bonus;
    temp.tax_personal_pension = tax_personal_pension;
    temp.tax_accidental_income = tax_accidental_income;
    temp.tax_total = tax_total;
    tax_report_list.push_back(temp);

    tax_result_model.end_update_data();
    ui->table_view_tax_result->resizeColumnsToContents();
}

void MainWindow::year_tax_calc(bool add_bonus_to_total, bool pay_personal_pension)
{
    int year;
    int month;
    int day;
    ui->date_edit->date().getDate(&year, &month, &day);
    qDebug() << year;

    total_income = 0.0;
    total_bonus = 0.0;
    total_accidental_income = 0.0;
    QSqlTableModel* p_sql_table_model_temp;

    p_sql_table_model_temp = p_table_income_model->get_table_model();
    QSqlRecord record = p_sql_table_model_temp->record();
    for(int row = 0; row < p_sql_table_model_temp->rowCount(); row++)
    {
        QVariant row_year = p_sql_table_model_temp->data(p_sql_table_model_temp->index(row, record.indexOf("year")));
        if (year == row_year.toInt())
        {
            QVariant cell_value;
            cell_value = p_sql_table_model_temp->data(p_sql_table_model_temp->index(row, record.indexOf("income")));
            total_income += cell_value.toDouble();
            cell_value = p_sql_table_model_temp->data(p_sql_table_model_temp->index(row, record.indexOf("bonus")));
            total_bonus += cell_value.toDouble();
            cell_value = p_sql_table_model_temp->data(p_sql_table_model_temp->index(row, record.indexOf("accidental-income")));
            total_accidental_income += cell_value.toDouble();
        }
    }

    qDebug() << "total_income: " << total_income;
    qDebug() << "total_bonus: " << total_bonus;
    qDebug() << "total_accidental_income: " << total_accidental_income;

    tax_start_point = 0.0;
    total_pension = 0.0;
    total_medical_insurance = 0.0;
    total_unemployment_insurance = 0.0;
    total_housing_fund = 0.0;
    total_housing_loan = 0.0;
    total_house_renting = 0.0;
    total_elderly_support = 0.0;
    total_serious_illness_support = 0.0;
    total_adult_education = 0.0;
    total_children_education = 0.0;
    total_infant_care = 0.0;
    total_personal_pension = 0.0;

    p_sql_table_model_temp = p_table_deduction_model->get_table_model();
    record = p_sql_table_model_temp->record();
    for (int row = 0; row < p_sql_table_model_temp->rowCount(); row++)
    {
        QVariant row_year = p_sql_table_model_temp->data(p_sql_table_model_temp->index(row, record.indexOf("year")));
        if (year == row_year.toInt())
        {
            QVariant cell_value;
            cell_value = p_sql_table_model_temp->data(p_sql_table_model_temp->index(row, record.indexOf("tax-deduction")));
            tax_start_point += cell_value.toDouble();
            cell_value = p_sql_table_model_temp->data(p_sql_table_model_temp->index(row, record.indexOf("pension-insurance")));
            total_pension += cell_value.toDouble();
            cell_value = p_sql_table_model_temp->data(p_sql_table_model_temp->index(row, record.indexOf("medical-insurance")));
            total_medical_insurance += cell_value.toDouble();
            cell_value = p_sql_table_model_temp->data(p_sql_table_model_temp->index(row, record.indexOf("unemployment-insurance")));
            total_unemployment_insurance += cell_value.toDouble();
            cell_value = p_sql_table_model_temp->data(p_sql_table_model_temp->index(row, record.indexOf("housing-fund")));
            total_housing_fund += cell_value.toDouble();
            cell_value = p_sql_table_model_temp->data(p_sql_table_model_temp->index(row, record.indexOf("housing-loan")));
            total_housing_loan += cell_value.toDouble();
            cell_value = p_sql_table_model_temp->data(p_sql_table_model_temp->index(row, record.indexOf("house-renting")));
            total_house_renting += cell_value.toDouble();
            cell_value = p_sql_table_model_temp->data(p_sql_table_model_temp->index(row, record.indexOf("elderly-care-support")));
            total_elderly_support += cell_value.toDouble();
            cell_value = p_sql_table_model_temp->data(p_sql_table_model_temp->index(row, record.indexOf("serious-illness-support")));
            total_serious_illness_support += cell_value.toDouble();
            cell_value = p_sql_table_model_temp->data(p_sql_table_model_temp->index(row, record.indexOf("adult-education")));
            total_adult_education += cell_value.toDouble();
            cell_value = p_sql_table_model_temp->data(p_sql_table_model_temp->index(row, record.indexOf("children-education")));
            total_children_education += cell_value.toDouble();
            cell_value = p_sql_table_model_temp->data(p_sql_table_model_temp->index(row, record.indexOf("infant-care-support")));
            total_infant_care += cell_value.toDouble();
            cell_value = p_sql_table_model_temp->data(p_sql_table_model_temp->index(row, record.indexOf("personal-pension")));
            total_personal_pension += cell_value.toDouble();
        }
    }

    ui->label_tax_start_point->setText(QString::number(tax_start_point, 'f', 2));
    ui->label_pension->setText(QString::number(total_pension, 'f', 2));
    ui->label_medical_insurance->setText(QString::number(total_medical_insurance, 'f', 2));
    ui->label_unemployment_insurance->setText(QString::number(total_unemployment_insurance, 'f', 2));
    ui->label_house_fund->setText(QString::number(total_housing_fund, 'f', 2));
    ui->label_house_loan->setText(QString::number(total_housing_loan, 'f', 2));
    ui->label_house_renting->setText(QString::number(total_house_renting, 'f', 2));
    ui->label_elderly_support->setText(QString::number(total_elderly_support, 'f', 2));
    ui->label_illness_support->setText(QString::number(total_serious_illness_support, 'f', 2));
    ui->label_adult_education->setText(QString::number(total_adult_education, 'f', 2));
    ui->label_children_education->setText(QString::number(total_children_education, 'f', 2));
    ui->label_infant_care->setText(QString::number(total_infant_care, 'f', 2));
    ui->label_personal_pension->setText(QString::number(total_personal_pension, 'f', 2));

    if (add_bonus_to_total)
    {
        total_income += total_bonus;
    }

    double tax_mount = total_income
                       - tax_start_point
                       - total_pension
                       - total_medical_insurance
                       - total_unemployment_insurance
                       - total_housing_fund
                       - total_housing_loan
                       - total_house_renting
                       - total_elderly_support
                       - total_serious_illness_support
                       - total_adult_education
                       - total_infant_care
                       - total_children_education;

    if (pay_personal_pension)
    {
        tax_mount -= total_personal_pension;
    }

    tax_income = total_tax_version_2018(tax_mount);

    tax_annual_bonus = 0.0;
    if (!add_bonus_to_total)
    {
        tax_annual_bonus = annual_bonus_version_2019(total_bonus);
    }

    if (pay_personal_pension)
    {
        tax_personal_pension = total_personal_pension * 0.03;
    }
    else
    {
        tax_personal_pension = 0;
    }

    tax_accidental_income = total_accidental_income * 0.2;

    tax_total = tax_income + tax_annual_bonus + tax_personal_pension + tax_accidental_income;
}

double MainWindow::total_tax_calc(double total_mount, QVector<tax_level_t>& levels)
{
    double total_tax = 0.0;
    for (QVector<tax_level_t>::Iterator it = levels.begin(); it != levels.end(); it++)
    {
        if (total_mount > (*it).tax_point)
        {
            total_tax += (total_mount - (*it).tax_point) * (*it).tax_rate;
            total_mount = (*it).tax_point;
        }
    }
    return total_tax;
}

// 全年一次性奖金优惠政策 2019 - 2021
// https://www.gov.cn/zhengce/zhengceku/2018-12/31/content_5440172.htm
// 全年一次性奖金优惠政策 2022 - 2023
// https://www.gov.cn/zhengce/zhengceku/2021-12/31/content_5665897.htm
// 全年一次性奖金优惠政策 2023 - 2027
// https://www.gov.cn/zhengce/zhengceku/202308/content_6900595.htm

// 按月换算后的综合所得税率表
// |级数 |全月应纳税所得额           |税率（%）  |速算扣除数 |
// |-----|-------------------------|-----------|----------|
// |1    |不超过3000元的             |3         |0         |
// |2    |超过3000元至12000元的部分  |10         |210        |
// |3    |超过12000元至25000元的部分 |20         |1410       |
// |4    |超过25000元至35000元的部分 |25         |2660       |
// |5    |超过35000元至55000元的部分 |30         |4410       |
// |6    |超过55000元至80000元的部分 |35         |7160       |
// |7    |超过80000元的部分          |45         |15160      |
double MainWindow::annual_bonus_tax_calc(double total_mount, QVector<tax_level_t> &levels)
{
    double monthly_mount = total_mount / 12;
    double rate = 0.0;
    double reduction = 0.0;
    for (QVector<tax_level_t>::Iterator it = levels.begin(); it != levels.end(); it++)
    {
        if (monthly_mount > (*it).tax_point)
        {
            rate = (*it).tax_rate;
            reduction = (*it).tax_fast_reduction;
            break;
        }
    }
    return total_mount * rate - reduction;
}

double MainWindow::annual_bonus_version_2019(double total_mount)
{
    tax_level_t temp_level;
    QVector<tax_level_t> levels;

    temp_level.tax_point = 80000.0;
    temp_level.tax_rate = 0.45;
    temp_level.tax_fast_reduction = 15160.0;
    levels.push_back(temp_level);

    temp_level.tax_point = 55000.0;
    temp_level.tax_rate = 0.35;
    temp_level.tax_fast_reduction = 7160.0;
    levels.push_back(temp_level);

    temp_level.tax_point = 35000.0;
    temp_level.tax_rate = 0.3;
    temp_level.tax_fast_reduction = 4410.0;
    levels.push_back(temp_level);

    temp_level.tax_point = 25000.0;
    temp_level.tax_rate = 0.25;
    temp_level.tax_fast_reduction = 2660.0;
    levels.push_back(temp_level);

    temp_level.tax_point = 12000.0;
    temp_level.tax_rate = 0.2;
    temp_level.tax_fast_reduction = 1410.0;
    levels.push_back(temp_level);

    temp_level.tax_point = 3000.0;
    temp_level.tax_rate = 0.1;
    temp_level.tax_fast_reduction = 210;
    levels.push_back(temp_level);

    temp_level.tax_point = 0.0;
    temp_level.tax_rate = 0.03;
    temp_level.tax_fast_reduction = 0.0;
    levels.push_back(temp_level);

    return annual_bonus_tax_calc(total_mount, levels);
}

// | 级数  | 全年应纳税所得额                 | 税率（％） |
// |------|-------------------------------|-----------|
// | 1    | 不超过 36000 元的               | 3         |
// | 2    | 超过 36000 元至 144000 元的部分  | 10        |
// | 3    | 超过 144000 元至 300000 元的部分 | 20        |
// | 4    | 超过 300000 元至 420000 元的部分 | 25        |
// | 5    | 超过 420000 元至 660000 元的部分 | 30        |
// | 6    | 超过 660000 元至 960000 元的部分 | 35        |
// | 7    | 超过 960000 元的部分            | 45        |
double MainWindow::total_tax_version_2018(double total_mount)
{
    tax_level_t temp_level;
    QVector<tax_level_t> levels;

    temp_level.tax_point = 960000.0;
    temp_level.tax_rate = 0.45;
    levels.push_back(temp_level);

    temp_level.tax_point = 660000.0;
    temp_level.tax_rate = 0.35;
    levels.push_back(temp_level);

    temp_level.tax_point = 420000.0;
    temp_level.tax_rate = 0.3;
    levels.push_back(temp_level);

    temp_level.tax_point = 300000.0;
    temp_level.tax_rate = 0.25;
    levels.push_back(temp_level);

    temp_level.tax_point = 144000.0;
    temp_level.tax_rate = 0.2;
    levels.push_back(temp_level);

    temp_level.tax_point = 36000.0;
    temp_level.tax_rate = 0.1;
    levels.push_back(temp_level);

    temp_level.tax_point = 0.0;
    temp_level.tax_rate = 0.03;
    levels.push_back(temp_level);

    return total_tax_calc(total_mount, levels);
}

void MainWindow::on_table_view_income_customContextMenuRequested(const QPoint &pos)
{
    QMenu* menu = new QMenu(this);

    QAction* action_add_row = new QAction(tr("Add one row at bottom"), this);
    menu->addAction(action_add_row);
    connect(action_add_row, SIGNAL(triggered()), this, SLOT(on_table_view_income_customContextMenuRequested_action_add_row()));

    QAction* action_delete = new QAction(tr("Delete"), this);
    menu->addAction(action_delete);
    connect(action_delete, SIGNAL(triggered()), this, SLOT(on_table_view_income_customContextMenuRequested_action_delete()));

    menu->popup(ui->table_view_income->viewport()->mapToGlobal(pos));
}

void MainWindow::on_table_view_income_customContextMenuRequested_action_add_row()
{
    qDebug() << __FILE__ << __LINE__ << p_table_income_model->table_add_record();
    qDebug() << __FILE__ << __LINE__ << p_table_income_model->table_sync();
    qDebug() << __FILE__ << __LINE__ << p_table_income_model->table_select();
}

void MainWindow::on_table_view_income_customContextMenuRequested_action_delete()
{
    QModelIndexList index_list = ui->table_view_income->selectionModel()->selectedIndexes();

    qDebug() << "index size:" << index_list.size();

    for(int i = 0; i < index_list.count(); i++)
    {
        p_table_income_model->table_delete_record(index_list[i].row());
    }
    qDebug() << __FILE__ << __LINE__ << p_table_income_model->table_sync();
    qDebug() << __FILE__ << __LINE__ << p_table_income_model->table_select();
}

void MainWindow::on_table_view_deduction_customContextMenuRequested(const QPoint &pos)
{
    QMenu* menu = new QMenu(this);

    QAction* action_add_row = new QAction(tr("Add one row at bottom"), this);
    menu->addAction(action_add_row);
    connect(action_add_row, SIGNAL(triggered()), this, SLOT(on_table_view_deduction_customContextMenuRequested_action_add_row()));

    QAction* action_delete = new QAction(tr("Delete"), this);
    menu->addAction(action_delete);
    connect(action_delete, SIGNAL(triggered()), this, SLOT(on_table_view_deduction_customContextMenuRequested_action_delete()));

    menu->popup(ui->table_view_deduction->viewport()->mapToGlobal(pos));
}

void MainWindow::on_table_view_deduction_customContextMenuRequested_action_add_row()
{
    qDebug() << __FILE__ << __LINE__ << p_table_deduction_model->table_add_record();
    qDebug() << __FILE__ << __LINE__ << p_table_income_model->table_sync();
    qDebug() << __FILE__ << __LINE__ << p_table_deduction_model->table_select();
}

void MainWindow::on_table_view_deduction_customContextMenuRequested_action_delete()
{
    QModelIndexList index_list = ui->table_view_deduction->selectionModel()->selectedIndexes();

    qDebug() << "index size:" << index_list.size();

    for (int i = 0; i < index_list.size(); i++)
    {
        p_table_deduction_model->table_delete_record(index_list[i].row());
    }
    qDebug() << __FILE__ << __LINE__ << p_table_income_model->table_sync();
    qDebug() << __FILE__ << __LINE__ << p_table_deduction_model->table_select();
}
