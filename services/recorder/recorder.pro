## Recorder service

QT += core dbus
QT -= gui
QT += multimediakit-private

CONFIG += console
CONFIG -= app_bundle
CONFIG += mobility

MOBILITY = multimedia

TARGET = callrecorder_recorder_service

TEMPLATE = app

SOURCES += \
    src/main.cc \
    src/recorderservice.cc \
    ../../common/settings.cc

contains(MEEGO_EDITION,harmattan) {
    target.path =  /opt/callrecorder/bin
    INSTALLS += target
}

HEADERS += \
    src/recorderservice.h \
    ../../common/common.h \
    ../../common/settings.h





