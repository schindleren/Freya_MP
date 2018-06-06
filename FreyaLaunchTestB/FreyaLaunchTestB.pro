#-------------------------------------------------
#
# Project created by QtCreator 2018-03-28T09:23:20
#
#-------------------------------------------------
include($$PWD/../FreyaLib/FreyaLib.pri)

QT       += core

CONFIG(debug, debug|release) {
    FREYALAUNCHTESTB_NAME = FreyaLaunchTestB_d
} else {
    FREYALAUNCHTESTB_NAME = FreyaLaunchTestB
}

TARGET = $$FREYALAUNCHTESTB_NAME
TEMPLATE = lib

DESTDIR = $$PWD/../bin/module
DLLDESTDIR = $$PWD/../bin/module
#MOC_DIR = $$PWD/build_out
#UI_DIR = $$PWD/build_out
#OBJECTS_DIR = $$PWD/build_out

DEFINES += FREYALAUNCHTESTB_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        freyalaunchtestb.cpp

HEADERS += \
        freyalaunchtestb.h \
        freyalaunchtestb_global.h

INCLUDEPATH += $$FREYALIB_INCLUDEPATH

LIBS += -L$$PWD/../bin
unix { LIBS += -l$${FREYALIB_NAME} }
win32 { LIBS += -l$${FREYALIB_NAME}5 }
