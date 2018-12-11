#-------------------------------------------------
#
# Project created by QtCreator 2015-04-27T14:35:43
#
#-------------------------------------------------
include(FreyaLib.pri)

DESTDIR = $$PWD/../bin
DLLDESTDIR = $$PWD/../bin
#MOC_DIR = $$PWD/freya_out
#UI_DIR = $$PWD/freya_out
#OBJECTS_DIR = $$PWD/freya_out

TARGET = $$FREYALIB_NAME

TEMPLATE = lib

QMAKE_TARGET_COMPANY = SchindlerenDesign
QMAKE_TARGET_PRODUCT = FreyaLib
QMAKE_TARGET_DESCRIPTION = C++ application development framework.
QMAKE_TARGET_COPYRIGHT = Copyright (C) SchindlerenDesign

DEFINES += FREYALIB_LIBRARY

unix {
    target.path = /usr/local/freyalib
    INSTALLS += target
}

INCLUDEPATH += $$FREYALIB_INCLUDEPATH

SOURCES += sources/freyalib.cpp \
    sources/base/freyabasecontrol.cpp \
    sources/base/freyabaseaction.cpp \
    sources/base/freyapublicregister.cpp \
    sources/base/freyacommanddistribution.cpp \
    sources/base/freyabasejson.cpp \
    sources/base/freyacryptogram.cpp \
    sources/base/freyabaseextension.cpp \
    sources/base/freyabaseplugin.cpp \
    sources/base/freyabaseactionex.cpp \
    sources/experience/singleapplication.cpp \
    sources/experience/freyalog.cpp

HEADERS += include/freyalib.h \
    include/freyalib_global.h \
    include/abstract/freyaabstractaction.h \
    include/abstract/freyaabstractcontrol.h \
    include/base/freyabaseaction.h \
    include/base/freyabasecontrol.h \
    include/base/freyapublicregister.h \
    include/base/freyacommanddistribution.h \
    include/base/freyabasejson.h \
    include/base/freyacryptogram.h \
    include/base/freyabaseextension.h \
    include/base/freyabaseplugin.h \
    include/base/freyabaseactionex.h \
    include/experience/singleapplication.h \
    include/experience/freyalog.h

contains(QT, gui) {
SOURCES += sources/gui/freyawindow.cpp \
    sources/base/freyabasewidget.cpp \
    sources/base/freyabasedialog.cpp \
    sources/experience/dragpanel.cpp \
    sources/experience/rtabwidget.cpp \
    sources/experience/roundedportrait.cpp

HEADERS += include/gui/freyawindow.h \
    include/abstract/freyaabstractwidget.h \
    include/base/freyabasewidget.h \
    include/base/freyabasedialog.h \
    include/experience/dragpanel.h \
    include/experience/rtabwidget.h \
    include/experience/roundedportrait.h
}

#RC_FILE += freyalib.rc
