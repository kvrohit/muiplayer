# MUI QT PROJECT FILE
# Modify this if new project files are added
TEMPLATE = app

TARGET = mui

QT += widgets \
    multimedia

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
    sources/artdata.h \
    sources/musicdatamodel.h \
    sources/musicdata.h \
    sources/globals.h \
    sources/metadata.h \
    sources/tagreader.h

FORMS += ui/mui.ui \
    ui/about.ui \
    ui/metadata.ui

SOURCES += sources/main.cpp \
    sources/mui.cpp \
    sources/about.cpp \
    sources/volumeslider.cpp \
    sources/metadatawidget.cpp \
    sources/artdata.cpp \
    sources/musicdatamodel.cpp \
    sources/musicdata.cpp \
    sources/globals.cpp \
    sources/tagreader.cpp

RESOURCES += rcc/res.qrc

CONFIG += release

# for OS specific builds
win32 {
    LIBS += -L../../lib
    OBJECTS += ../../lib/cplaylist.o \
        ../../lib/caudiotags.o
}
else {
    LIBS += -L/usr/lib
    OBJECTS += ../../lib/cplaylist_linux.o \
        ../../lib/caudiotags_linux.o
}

# Output
OBJECTS_DIR = obj
UI_DIR = ui
MOC_DIR = moc
RCC_DIR = rcc
DESTDIR = ../../bin
