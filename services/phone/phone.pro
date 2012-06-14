## Phone service

QT += core dbus
QT -= gui

CONFIG += console
CONFIG -= app_bundle

TARGET = callrecorder_phone_service

TEMPLATE = app

SOURCES += \
    src/main.cc \
    src/phoneservice.cc \
    ../../common/settings.cc

contains(MEEGO_EDITION,harmattan) {
    target.path =  /opt/callrecorder/bin
    INSTALLS += target
}

HEADERS += \
    src/phoneservice.h \
    ../../common/common.h \
    ../../common/settings.h













