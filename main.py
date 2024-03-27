#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# MIT License
#
# Copyright (c) 2024 yaofei zheng
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.


import sqlite3


def total_tax_calc(total_mount: float, levels: tuple) -> float:
    total_tax: float = 0.0
    total_mount_local = total_mount

    for i in levels:
        if total_mount_local > i[0]:
            total_tax += (total_mount_local - i[0]) * i[1]
            total_mount_local = i[0]

    return total_tax


def total_tax_version_2018(total_mount: float) -> float:
    # | 级数  | 全年应纳税所得额                 | 税率（％） |
    # |------|-------------------------------|-----------|
    # | 1    | 不超过 36000 元的               | 3         |
    # | 2    | 超过 36000 元至 144000 元的部分  | 10        |
    # | 3    | 超过 144000 元至 300000 元的部分 | 20        |
    # | 4    | 超过 300000 元至 420000 元的部分 | 25        |
    # | 5    | 超过 420000 元至 660000 元的部分 | 30        |
    # | 6    | 超过 660000 元至 960000 元的部分 | 35        |
    # | 7    | 超过 960000 元的部分            | 45        |

    levels_2018_version: tuple = [
        [960000, 0.45],
        [660000, 0.35],
        [420000, 0.3],
        [300000, 0.25],
        [144000, 0.2],
        [36000, 0.1],
        [0, 0.03]]

    return total_tax_calc(total_mount, levels_2018_version)


def main(year: int, add_bonus_to_total: bool):
    con = sqlite3.connect("./tax.db")
    cur = con.cursor()

    # "year" 年
    # "month" 月
    # "income" 收入
    # "bonus" 全年一次性奖金
    # "threshold" 减除费用（起征点）
    # "pension" 专项扣除 基本养老保险
    # "medical-insurance" 专项扣除 基本医疗保险
    # "unemployment-insurance" 专项扣除 失业保险
    # "housing-fund" 专项扣除 住房公积金
    # "housing-loan" 专项附加扣除 住房贷款利息
    # "house-renting" 专项附加扣除 住房租金
    # "elderly-support" 专项附加扣除 赡养老人
    # "serious-illness-support" 专项附加扣除 大病医疗
    # "adult-education" 专项附加扣除 继续教育
    # "children-education" 专项附加扣除 子女教育
    # "personal-pension" 个人养老金

    cur.execute("CREATE TABLE IF NOT EXISTS \"income-data\" (     \
                \"year\" INTEGER,                   \
                \"month\"   INTEGER,                \
                \"income\" REAL,                    \
                \"bonus\" REAL,                     \
                \"threshold\" REAL,                 \
                \"pension\" REAL,                   \
                \"medical-insurance\" REAL,         \
                \"unemployment-insurance\" REAL,    \
                \"housing-fund\" REAL,              \
                \"housing-loan\" REAL,              \
                \"house-renting\" REAL,             \
                \"elderly-support\" REAL,           \
                \"serious-illness-support\" REAL,   \
                \"adult-education\" REAL,           \
                \"children-education\" REAL,        \
                \"personal-pension\" REAL)")

    res = cur.execute("SELECT * FROM 'income-data' WHERE year={}".format(year))
    result = res.fetchall()
    # print(result)
    total_income: float = 0.0
    total_bonus: float = 0.0
    tax_start_point: float = 0.0
    total_pension: float = 0.0
    total_medical_insurance: float = 0.0
    total_unemployment_insurance: float = 0.0
    total_housing_fund: float = 0.0
    total_housing_loan: float = 0.0
    total_house_renting: float = 0.0
    total_elderly_support: float = 0.0
    total_serious_illness_support: float = 0.0
    total_adult_education: float = 0.0
    total_children_education: float = 0.0
    total_personal_pension: float = 0.0
    for i in result:
        total_income += i[2]
        total_bonus += i[3]
        tax_start_point += i[4]
        total_pension += i[5]
        total_medical_insurance += i[6]
        total_unemployment_insurance += i[7]
        total_housing_fund += i[8]
        total_housing_loan += i[9]
        total_house_renting += i[10]
        total_elderly_support += i[11]
        total_serious_illness_support += i[12]
        total_adult_education += i[13]
        total_children_education += i[14]
        total_personal_pension += i[15]

    print("总收入：{}".format(total_income))
    print("全年一次性奖金：{}".format(total_bonus))
    print("减除费用（起征点）：{}".format(tax_start_point))
    print("基本养老保险： {}".format(total_pension))
    print("基本医疗保险：{}".format(total_medical_insurance))
    print("失业保险：{}".format(total_unemployment_insurance))
    print("住房公积金：{}".format(total_housing_fund))
    print("住房贷款利息：{}".format(total_housing_loan))
    print("住房租金：{}".format(total_house_renting))
    print("赡养老人：{}".format(total_elderly_support))
    print("大病医疗：{}".format(total_serious_illness_support))
    print("继续教育：{}".format(total_adult_education))
    print("子女教育：{}".format(total_children_education))
    print("个人养老金：{}".format(total_personal_pension))

    tax_mount: float = 0.0
    if add_bonus_to_total:
        tax_mount = total_income \
            + total_bonus  \
            - tax_start_point  \
            - total_pension  \
            - total_medical_insurance\
            - total_unemployment_insurance  \
            - total_housing_fund \
            - total_housing_loan \
            - total_house_renting \
            - total_elderly_support \
            - total_serious_illness_support \
            - total_adult_education \
            - total_children_education \
            - total_personal_pension
    else:
        tax_mount = total_income \
            - tax_start_point  \
            - total_pension  \
            - total_medical_insurance\
            - total_unemployment_insurance  \
            - total_housing_fund \
            - total_housing_loan \
            - total_house_renting \
            - total_elderly_support \
            - total_serious_illness_support \
            - total_adult_education \
            - total_children_education \
            - total_personal_pension

    print("应纳税所得：{}".format(tax_mount))

    tax = total_tax_version_2018(tax_mount)

    return tax



if __name__ == "__main__":
    print("本程序的结果仅作参考，最终请按个税APP为准！")
    print("--------------")
    print("不计入应纳税额：{}".format(main(2023, False)))
    print("--------------")
    print("计入应纳税额：{}".format(main(2023, True)))
