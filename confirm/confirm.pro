CONFIG += meegotouch-boostable meegotouch

TEMPLATE = app
TARGET = confirmation

contains(MEEGO_EDITION,harmattan) {
    target.path = /opt/callrecorder/bin
    INSTALLS += target
}

SOURCES += \
    src/main.cc \
    src/saveconfirmation.cpp \
    src/mainapp.cpp \
    ../common/settings.cc

HEADERS += \
    src/saveconfirmation.h \
    ../common/common.h \
    src/mainapp.h \
    ../common/settings.h





