#
# MUI QT PROJECT FILE
# Modify this if new project files are added
#

TEMPLATE = app
TARGET = 
DEPENDPATH += . rcc sources ui
INCLUDEPATH += . ../../inc

# Input
HEADERS += sources/mui.h sources/about.h
FORMS += ui/mui.ui ui/about.ui
SOURCES += sources/main.cpp sources/mui.cpp sources/about.cpp
RESOURCES += rcc/res.qrc

# for OS specific builds
win32 {
  LIBS += -L../../lib -lfmodex
  OBJECTS += ../../lib/cfmod.o ../../lib/cplaylist.o
} else {
  LIBS += -L/usr/lib -lfmodex
  OBJECTS += ../../lib/cfmod_linux.o ../../lib/cplaylist.o
}

# Output
OBJECTS_DIR = obj
UI_DIR = ui
MOC_DIR = moc
RCC_DIR = rcc
DESTDIR = ../../bin
