# MUI QT PROJECT FILE
# Modify this if new project files are added
TEMPLATE = app
TARGET = mui
QT += widgets
DEPENDPATH += . \
    rcc \
    sources \
    ui
INCLUDEPATH += . \
    ../../inc

# Input
HEADERS += sources/mui.h \
    sources/about.h \
    sources/volumeslider.h \
    sources/metadatawidget.h \
    sources/errorlog.h \
    sources/editstyle.h \
    sources/artdata.h \
    sources/filesystembrowser.h \
    sources/musicdatamodel.h \
    sources/musicdata.h \
    sources/globals.h
FORMS += ui/mui.ui \
    ui/about.ui \
    ui/metadata.ui \
    ui/errorlog.ui \
    ui/editstyle.ui
SOURCES += sources/main.cpp \
    sources/mui.cpp \
    sources/about.cpp \
    sources/volumeslider.cpp \
    sources/metadatawidget.cpp \
    sources/errorlog.cpp \
    sources/editstyle.cpp \
    sources/artdata.cpp \
    sources/filesystembrowser.cpp \
    sources/musicdatamodel.cpp \
    sources/musicdata.cpp \
    sources/globals.cpp
RESOURCES += rcc/res.qrc
CONFIG += release

# for OS specific builds
win32 {
    LIBS += -L../../lib \
        -lfmodex
    OBJECTS += ../../lib/cfmod.o \
        ../../lib/cplaylist.o \
        ../../lib/caudiotags.o
}
else {
    LIBS += -L/usr/lib \
        -lfmodex
    OBJECTS += ../../lib/cfmod_linux.o \
        ../../lib/cplaylist_linux.o \
        ../../lib/caudiotags_linux.o
}

# Output
OBJECTS_DIR = obj
UI_DIR = ui
MOC_DIR = moc
RCC_DIR = rcc
DESTDIR = ../../bin
