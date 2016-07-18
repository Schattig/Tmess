#-------------------------------------------------
#
# Project created by QtCreator 2016-07-01T15:30:21
#
#-------------------------------------------------

QT += widgets serialport serialbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Tmess
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingsdialog.cpp \
    console.cpp \
    uart.cpp \
    sensorbox.cpp \
    borderlayout.cpp \
    COM_handler.cpp \
    modbus.cpp \
    pyranobox.cpp \
    mod_handler.cpp \
    sensors.cpp \
    sensordialog.cpp

HEADERS  += mainwindow.h \
    settingsdialog.h \
    console.h \
    uart.h \
    sensorbox.h \
    borderlayout.h \
    COM_handler.h \
    modbus.h \
    pyranobox.h \
    mod_handler.h \
    sensors.h \
    sensordialog.h

FORMS    += \
    settingsdialog.ui \
    sensordialog.ui

