#include "table_model_tax_result.h"

table_model_tax_result::table_model_tax_result(QObject *parent)
    : QAbstractTableModel(parent)
{

}

void table_model_tax_result::add_tax_result(QVector<yearly_tax_report_t>* p)
{
    p_data = p;
}

void table_model_tax_result::begin_update_data()
{
    layoutAboutToBeChanged();
}

void table_model_tax_result::end_update_data()
{
    layoutChanged();
}

QVariant table_model_tax_result::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant header_name;
    if ((Qt::Orientation::Horizontal == orientation) && (Qt::DisplayRole == role))
    {
        switch (section)
        {
        case 0:
            header_name.setValue(tr("Year"));
            break;
        case 1:
            header_name.setValue(tr("Add Bonus to Total"));
            break;
        case 2:
            header_name.setValue(tr("Pay Personal Pension"));
            break;
        case 3:
            header_name.setValue(tr("Total Income"));
            break;
        case 4:
            header_name.setValue(tr("Total Annual Bonus"));
            break;
        case 5:
            header_name.setValue(tr("Tax Total Income"));
            break;
        case 6:
            header_name.setValue(tr("Tax Annual Bonus"));
            break;
        case 7:
            header_name.setValue(tr("Tax Personal Pension"));
            break;
        case 8:
            header_name.setValue(tr("Tax Total"));
            break;
        default:
            break;
        }
    }

    return header_name;
}

int table_model_tax_result::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return p_data->size();
}

int table_model_tax_result::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 9;
}

QVariant table_model_tax_result::data(const QModelIndex &index, int role) const
{
    QVariant value;
    if (!index.isValid())
        return value;

    if (Qt::DisplayRole == role)
    {
        switch (index.column())
        {
        case 0:
            value.setValue((*p_data)[index.row()].year);
            break;
        case 1:
            value.setValue((*p_data)[index.row()].add_bonus_to_total ? tr("Yes") : tr("No"));
            break;
        case 2:
            value.setValue((*p_data)[index.row()].pay_personal_pension ? tr("Yes"): tr("No"));
            break;
        case 3:
            value.setValue(QString::number((*p_data)[index.row()].total_income, 'f', 2));
            break;
        case 4:
            value.setValue(QString::number((*p_data)[index.row()].total_annual_bonus, 'f', 2));
            break;
        case 5:
            value.setValue(QString::number((*p_data)[index.row()].tax_total_income, 'f', 2));
            break;
        case 6:
            value.setValue(QString::number((*p_data)[index.row()].tax_annual_bonus, 'f', 2));
            break;
        case 7:
            value.setValue(QString::number((*p_data)[index.row()].tax_personal_pension, 'f', 2));
            break;
        case 8:
            value.setValue(QString::number((*p_data)[index.row()].tax_total, 'f', 2));
            break;
        default:
            break;
        }
    }
    return value;
}
