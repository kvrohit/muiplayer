# MUI QT PROJECT FILE
# Modify this if new project files are added
TEMPLATE = app

TARGET = mui

QT += widgets \
    multimedia

DEPENDPATH += . \
    rcc \
    src \
    ui

INCLUDEPATH += . \
    ../../inc

# Input
HEADERS += src/mui.h \
    src/about.h \
    src/volumeslider.h \
    src/metadatawidget.h \
    src/artdata.h \
    src/musicdatamodel.h \
    src/musicdata.h \
    src/globals.h \
    src/metadata.h \
    src/tagreader.h

FORMS += ui/mui.ui \
    ui/about.ui \
    ui/metadata.ui

SOURCES += src/main.cpp \
    src/mui.cpp \
    src/about.cpp \
    src/volumeslider.cpp \
    src/metadatawidget.cpp \
    src/artdata.cpp \
    src/musicdatamodel.cpp \
    src/musicdata.cpp \
    src/globals.cpp \
    src/tagreader.cpp

RESOURCES += rcc/res.qrc

CONFIG += release

# Output
OBJECTS_DIR = obj
UI_DIR = ui
MOC_DIR = moc
RCC_DIR = rcc
DESTDIR = ../../bin
