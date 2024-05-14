#ifndef DATASTRUCTDEFINES_H
#define DATASTRUCTDEFINES_H

typedef struct {
    double tax_point;
    double tax_rate;
} tax_level_t;

typedef struct {
    int year;
    bool add_bonus_to_total;
    bool pay_personal_pension;
    double total_income;
    double total_annual_bonus;
    double total_accidental_income;
    double tax_total_income;
    double tax_annual_bonus;
    double tax_personal_pension;
    double tax_accidental_income;
    double tax_total;
} yearly_tax_report_t;

#endif // DATASTRUCTDEFINES_H
