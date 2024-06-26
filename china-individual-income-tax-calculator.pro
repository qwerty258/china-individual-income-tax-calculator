QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    database.cpp \
    dialog_about.cpp \
    dialog_global_settings.cpp \
    main.cpp \
    mainwindow.cpp \
    table_model_tax_result.cpp

HEADERS += \
    database.h \
    datastructdefines.h \
    dialog_about.h \
    dialog_global_settings.h \
    mainwindow.h \
    table_model_tax_result.h

FORMS += \
    dialog_about.ui \
    dialog_global_settings.ui \
    mainwindow.ui

TRANSLATIONS += \
    china_iit_calc_en_US.ts \
    china_iit_calc_zh_CN.ts

RESOURCES += \
    china-iit-calc.qrc

macx {
    QMAKE_APPLE_DEVICE_ARCHS = x86_64 arm64
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
