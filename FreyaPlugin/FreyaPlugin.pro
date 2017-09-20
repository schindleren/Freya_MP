#-------------------------------------------------
#
# Project created by QtCreator 2017-09-20T20:24:13
#
#-------------------------------------------------
include($$PWD/../FreyaLib/FreyaLib.pri)

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG(debug, debug|release) {
    FREYAPLUGIN_NAME = FreyaPlugin_d
} else {
    FREYAPLUGIN_NAME = FreyaPlugin
}

DESTDIR = $$PWD/../bin
DLLDESTDIR = $$PWD/../bin
#MOC_DIR = $$PWD/freya_out
#UI_DIR = $$PWD/freya_out
#OBJECTS_DIR = $$PWD/freya_out

TARGET = $$FREYAPLUGIN_NAME

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_TARGET_COMPANY = "SchindlerenDesign"
QMAKE_TARGET_PRODUCT = $$FREYAPLUGIN_NAME
QMAKE_TARGET_DESCRIPTION = Test of FreyaTest `s plugin
QMAKE_TARGET_COPYRIGHT = Copyright (C) SchindlerenDesign

#RC_ICONS = "freyatest.ico"

SOURCES += \
        main.cpp \
        freyaplugin.cpp

HEADERS += \
        freyaplugin.h

INCLUDEPATH += $$FREYALIB_INCLUDEPATH \
            $$PWD/../FreyaTest

LIBS += -L$$PWD/../bin -l$${FREYALIB_NAME}3
