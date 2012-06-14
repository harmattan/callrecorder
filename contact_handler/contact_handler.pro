QT += core dbus

CONFIG += console debug
CONFIG -= app_bundle
CONFIG += mobility

MOBILITY = contacts

TEMPLATE = app

TARGET = callrecorder_contact_handler

contains(MEEGO_EDITION,harmattan) {
    target.path = /opt/callrecorder/bin
    INSTALLS += target
}

SOURCES += \
    src/main.cc \
    src/contacthandler.cc

HEADERS += \
    src/contacthandler.h
