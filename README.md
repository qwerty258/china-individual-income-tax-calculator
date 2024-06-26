中国个税计算
==========

本程序的结果仅作参考，最终请按个税APP为准！

 - 比较全年一次性奖金是否单独计算。
 - 比较是否缴纳个人养老金。
 - 没有对其他类型收入计算纳税额。

所以大家一起来完善吧。

# 数据库编辑

1. [DB Browser for SQLite](https://sqlitebrowser.org/) 独立GUI数据库编辑器
1. [SQLite3 Editor](https://marketplace.visualstudio.com/items?itemName=yy0931.vscode-sqlite3-editor) VSCode 插件

# 测试数据输出

月薪8000，年终奖10000，个人养老金12000，养老480，医疗120，失业30，公积金241，租房每月1500，赡养老人每月3000

|   year | annual bonus   | personal pension   |   total income |   annual bonus |   tax |   annual bonus tax |   total tax |
|--------|----------------|--------------------|----------------|----------------|-------|--------------------|-------------|
|   2023 | exclude        | pay                |       96000.00 |       10000.00 |  0.00 |             300.00 |      300.00 |
|   2023 | exclude        | no pay             |       96000.00 |       10000.00 |  0.00 |             300.00 |      300.00 |
|   2023 | include        | pay                |      106000.00 |       10000.00 |  0.00 |               0.00 |        0.00 |
|   2023 | include        | no pay             |      106000.00 |       10000.00 |  0.00 |               0.00 |        0.00 |

月薪50000，年终奖50000，个人养老金12000，养老2500，医疗625，失业156，公积金2200，租房每月1500，赡养老人每月3000

|   year | annual bonus   | personal pension   |   total income |   annual bonus |      tax |   annual bonus tax |   total tax |
|--------|----------------|--------------------|----------------|----------------|----------|--------------------|-------------|
|   2023 | exclude        | pay                |      600000.00 |       50000.00 | 70137.00 |            2480.00 |    72617.00 |
|   2023 | exclude        | no pay             |      600000.00 |       50000.00 | 73148.40 |            2480.00 |    75628.40 |
|   2023 | include        | pay                |      650000.00 |       50000.00 | 84548.40 |               0.00 |    84548.40 |
|   2023 | include        | no pay             |      650000.00 |       50000.00 | 88148.40 |               0.00 |    88148.40 |
