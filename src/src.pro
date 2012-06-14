## Call Recorder Controller

CONFIG += meegotouch-boostable meegotouch debug

QT += core dbus
CONFIG += console
CONFIG += mobility

MOBILITY = multimedia messaging connectivity contacts


##  LIBS += -lcontentaction

TEMPLATE = app
TARGET = callrecorder

SOURCES += \
    main.cc \
    mainpage.cc \
    mainapp.cc \
    controllerpage.cc \
    headerwidget.cc \
    temppage.cc \
    phoneinterface.cc \
    recorderinterface.cc \
    aboutpage.cc \
    model/recordfilecell.cpp \
    model/recordfilemodel.cpp \
    ../common/settings.cc \
    playback.cc \
    deletefiles.cc \
    searchbox.cc \
    mark.cc \
    share.cc

contains(MEEGO_EDITION,harmattan) {
    target.path = /opt/callrecorder/bin
    desktop.files = ../meta/callrecorder.desktop
    desktop.path = /usr/share/applications
    icon.files = ../meta/callrecorder.png
    icon.path = /usr/share/icons/hicolor/80x80/apps
    style.files = ../meta/callrecorder.css
    style.path = /opt/callrecorder/style
    autorun.files = ../meta/*.conf
    autorun.path = /etc/init/apps
    translations.files = ../meta/*.qm
    translations.path = /usr/share/l10n/meegotouch
    INSTALLS += target desktop icon style autorun translations
}

OTHER_FILES += \
    ../qtc_packaging/debian_harmattan/rules \
    ../qtc_packaging/debian_harmattan/manifest.aegis \
    ../qtc_packaging/debian_harmattan/copyright \
    ../qtc_packaging/debian_harmattan/control \
    ../qtc_packaging/debian_harmattan/compat \
    ../qtc_packaging/debian_harmattan/conffiles \
    ../qtc_packaging/debian_harmattan/preinst \
    ../qtc_packaging/debian_harmattan/changelog \
    ../qtc_packaging/debian_harmattan/prerm \
    ../meta/callrecorder.css \
    ../meta/callrecorder.desktop \
    ../meta/callrecorder.png \
    ../meta/callrecorder64.png \
    ../meta/callrecorder_en.ts \
    ../meta/callrecorder_phone.conf \
    ../meta/callrecorder_recorder.conf \
    ../meta/callrecorder_ru.ts \
    ../meta/translation_id.txt \
    ../meta/callrecorder_tr.ts \
    ../meta/callrecorder_vi.ts \
    ../meta/callrecorder_pl.ts \
    ../meta/callrecorder_da.ts

HEADERS += \
    mainpage.h \
    mainapp.h \
    controllerpage.h \
    headerwidget.h \
    temppage.h \
    phoneinterface.h \
    recorderinterface.h \
    aboutpage.h \
    model/recordfilecell.h \
    model/recordfilemodel.h \
    ../common/common.h \
    ../common/settings.h \
    itemcreator.h \
    playback.h \
    deletefiles.h \
    searchbox.h \
    mark.h \
    share.h















































