QT       += core gui network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = DeviceCommunicationManager
TEMPLATE = app
DESTDIR = $$PWD/bin

INCLUDEPATH += $$PWD/src

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/device/devicebase.cpp \
    src/device/devicefactory.cpp \
    src/device/device8808a.cpp \
    src/device/devicedpo4104.cpp \
    src/device/devicen5750a.cpp \
    src/comm/serialcomm.cpp \
    src/comm/tcpcomm.cpp \
    src/comm/udpcomm.cpp \
    src/ui/devicewidget.cpp \
    src/ui/deviceconfigdialog.cpp \
    src/ui/devicelistwidget.cpp \
    src/ui/devicedragwidget.cpp \
    src/manager/devicemanager.cpp

HEADERS += \
    src/mainwindow.h \
    src/device/devicebase.h \
    src/device/devicefactory.h \
    src/device/device8808a.h \
    src/device/devicedpo4104.h \
    src/device/devicen5750a.h \
    src/comm/icomminterface.h \
    src/comm/serialcomm.h \
    src/comm/tcpcomm.h \
    src/comm/udpcomm.h \
    src/ui/devicewidget.h \
    src/ui/deviceconfigdialog.h \
    src/ui/devicelistwidget.h \
    src/ui/devicedragwidget.h \
    src/manager/devicemanager.h

FORMS += \
    src/ui/mainwindow.ui \
    src/ui/deviceconfigdialog.ui

RESOURCES += \
    resources/resources.qrc

# 默认构建目录
CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/bin/debug
} else {
    DESTDIR = $$PWD/bin/release
} 
