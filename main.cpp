#include "mainwindow.h"

#include <QApplication>
#include <QSettings>
#include <QTranslator>
#include <QLocale>
#include <QDir>
#include <QDebug>

QSettings global_settings(
    QSettings::NativeFormat,
    QSettings::UserScope,
    "china-individual-income-tax-calculator",
    "china-individual-income-tax-calculator");

int main(int argc, char *argv[])
{
    global_settings.sync();
    QApplication a(argc, argv);
    QTranslator qt(nullptr);
    QLocale l;
    l.system();
    qDebug() << QDir::currentPath();
    qDebug() << QString("china-iit-calc_") + l.name();
    qDebug() << qt.load(QString("china-iit-calc_") + l.name() + ".qm", "", "", "");
    a.installTranslator(&qt);
    MainWindow w;
    w.show();
    return a.exec();
}
