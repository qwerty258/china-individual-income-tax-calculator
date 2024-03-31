#include "mainwindow.h"

#include <QApplication>
#include <QSettings>

QSettings global_settings(
    QSettings::NativeFormat,
    QSettings::UserScope,
    "china-individual-income-tax-calculator",
    "china-individual-income-tax-calculator");

int main(int argc, char *argv[])
{
    global_settings.sync();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
