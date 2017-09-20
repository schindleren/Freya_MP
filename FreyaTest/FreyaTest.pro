#-------------------------------------------------
#
# Project created by QtCreator 2017-09-08T09:53:57
#
#-------------------------------------------------
include($$PWD/../FreyaLib/FreyaLib.pri)

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG(debug, debug|release) {
    FREYATEST_NAME = FreyaTest_d
} else {
    FREYATEST_NAME = FreyaTest
}

DESTDIR = $$PWD/../bin
DLLDESTDIR = $$PWD/../bin
#MOC_DIR = $$PWD/freya_out
#UI_DIR = $$PWD/freya_out
#OBJECTS_DIR = $$PWD/freya_out

TARGET = $$FREYATEST_NAME

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
QMAKE_TARGET_PRODUCT = $$FREYATEST_NAME
QMAKE_TARGET_DESCRIPTION = Test of FreyaLib
QMAKE_TARGET_COPYRIGHT = Copyright (C) SchindlerenDesign

#RC_ICONS = "freyatest.ico"

SOURCES += \
        main.cpp \
        freyatest.cpp \
    localclienta.cpp \
    localclientb.cpp \
    localclients.cpp

HEADERS += \
        freyatest.h \
    freyatest_global.h \
    freyatest_define.h \
    localclienta.h \
    localclientb.h \
    localclients.h

INCLUDEPATH += $$FREYALIB_INCLUDEPATH

LIBS += -L$$PWD/../bin -l$${FREYALIB_NAME}3
