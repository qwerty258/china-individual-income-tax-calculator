#ifndef TABLE_MODEL_TAX_RESULT_H
#define TABLE_MODEL_TAX_RESULT_H

#include <QAbstractTableModel>

#include "datastructdefines.h"

class table_model_tax_result: public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit table_model_tax_result(QObject *parent = nullptr);
    void add_tax_result(QVector<yearly_tax_report_t>* p);
    void begin_update_data();
    void end_update_data();

    // Header
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QVector<yearly_tax_report_t>* p_data;
};

#endif // TABLE_MODEL_TAX_RESULT_H
