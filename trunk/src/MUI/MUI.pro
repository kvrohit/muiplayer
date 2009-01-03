#
# MUI QT PROJECT FILE
# Modify this if new project files are added
#

TEMPLATE = app
TARGET = 
DEPENDPATH += . rcc sources ui
INCLUDEPATH += . ../../inc

# Input
HEADERS     +=      sources/mui.h \
                    sources/about.h \
                    sources/playlistmodel.h \
                    sources/volumeslider.h \
                    sources/metadatawidget.h \
					sources/support.h

FORMS       +=      ui/mui.ui \
                    ui/about.ui \
                    ui/metadata.ui

SOURCES     +=      sources/main.cpp \
                    sources/mui.cpp \
                    sources/about.cpp \
                    sources/playlistmodel.cpp \
                    sources/volumeslider.cpp \
                    sources/metadatawidget.cpp \
					sources/support.cpp
                    
RESOURCES += rcc/res.qrc
CONFIG += release

# for OS specific builds
win32 {
  LIBS += -L../../lib -lfmodex
  OBJECTS += ../../lib/cfmod.o \
             ../../lib/cplaylist.o \
             ../../lib/caudiotags.o    
} else {
  LIBS += -L/usr/lib -lfmodex
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
