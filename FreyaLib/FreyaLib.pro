#-------------------------------------------------
#
# Project created by QtCreator 2015-04-27T14:35:43
#
#-------------------------------------------------
include(FreyaLib.pri)

DESTDIR = $$PWD/../bin
DLLDESTDIR = $$PWD/../bin
TARGET = $$FREYALIB_NAME

TEMPLATE = lib

VERSION = 2.1.0.4

DEFINES += FREYALIB_LIBRARY

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += $$FREYALIB_INCLUDEPATH

SOURCES += sources/freyalib.cpp \
    sources/base/freyabasewidget.cpp \
    sources/base/freyabasecontrol.cpp \
    sources/base/freyabaseaction.cpp \
    sources/base/freyabasedialog.cpp \
    sources/experience/dragpanel.cpp \
    sources/base/freyapublicregister.cpp \
    sources/gui/freyawindow.cpp \
    sources/experience/rtabwidget.cpp \
    sources/experience/singleapplication.cpp \
    sources/base/freyabasejson.cpp \
    sources/base/freyacryptogram.cpp \
    sources/experience/roundedportrait.cpp \
    sources/base/freyabaseextension.cpp \
    sources/base/freyabaseplugin.cpp \
    sources/base/freyabaseactionex.cpp

HEADERS += include/freyalib.h\
    include/freyalib_global.h \
    include/abstract/freyaabstractaction.h \
    include/abstract/freyaabstractcontrol.h \
    include/abstract/freyaabstractwidget.h \
    include/base/freyabasewidget.h \
    include/base/freyabasecontrol.h \
    include/base/freyabaseaction.h \
    include/base/freyabasedialog.h \
    include/experience/dragpanel.h \
    include/base/freyapublicregister.h \
    include/gui/freyawindow.h \
    include/experience/rtabwidget.h \
    include/experience/singleapplication.h \
    include/base/freyabasejson.h \
    include/base/freyacryptogram.h \
    include/experience/roundedportrait.h \
    include/base/freyabaseextension.h \
    include/base/freyabaseplugin.h \
    include/base/freyabaseactionex.h

RC_FILE += freyalib.rc
