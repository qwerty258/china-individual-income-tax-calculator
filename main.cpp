#include "mainwindow.h"

#include <QApplication>
#include <QSettings>
#include <QTranslator>
#include <QLocale>
#include <QDir>
#include <QMessageBox>
#include <QDebug>

QSettings global_settings(
    QSettings::NativeFormat,
    QSettings::UserScope,
    "china-individual-income-tax-calculator",
    "china-individual-income-tax-calculator");

QTranslator qt(nullptr);
QLocale l;

int main(int argc, char *argv[])
{
    global_settings.sync();
    QApplication a(argc, argv);
    l.system();
    if (qt.load(":/china_iit_calc_" + l.name()))
    {
        a.installTranslator(&qt);
    }
    else
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Warning);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setText("Translation file " + QString("china_iit_calc_") + l.name() + " missing, fallback to english");
        msg.exec();
    }
    MainWindow w;
    w.show();
    return a.exec();
}
