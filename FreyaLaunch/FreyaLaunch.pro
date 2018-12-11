#-------------------------------------------------
#
# Project created by QtCreator 2018-02-26T09:38:36
#
#-------------------------------------------------
include($$PWD/../FreyaLib/FreyaLib.pri)

QT       += core

CONFIG(debug, debug|release) {
    FREYALAUNCH_NAME = FreyaLaunch_d
} else {
    FREYALAUNCH_NAME = FreyaLaunch
}

DESTDIR = $$PWD/../bin
DLLDESTDIR = $$PWD/../bin
#MOC_DIR = $$PWD/build_out
#UI_DIR = $$PWD/build_out
#OBJECTS_DIR = $$PWD/build_out

TARGET = $$FREYALAUNCH_NAME

TEMPLATE = app
unix {
SVNVER = $$system(svn info | grep 'Changed\ Rev' | cut -b 19-)
isEmpty(SVNVER){
SVNVER = $$system(svn info | grep '最后修改的版本' | cut -b 24-)
}
}
win32{
#SVNVER = $$system(for /f \"delims=\" %i in (\'svn info ^| findstr \"Revision\"\') do set rev=%i ^| echo %rev:~10%)
VERSTR = $$system('svn info | findstr "Revision"')
SVNVER = $$replace(VERSTR, Revision: , )
}
isEmpty(SVNVER){
SVNVER = 95
message("faild to read svn version, use default version:$${SVNVER}")
}
VERSION = 1.2.2.$$SVNVER
DEFINES += FREYALAUNCH_VER=\\\"$$VERSION\\\"

message("$${FREYALAUNCH_NAME} version:$${VERSION}")

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_TARGET_COMPANY = SchindlerenDesign
QMAKE_TARGET_PRODUCT = $$FREYALAUNCH_NAME
QMAKE_TARGET_DESCRIPTION = Launch of the application work with Freyalib
QMAKE_TARGET_COPYRIGHT = Copyright (C) SchindlerenDesign

RC_ICONS = "freyalaunch.ico"

SOURCES += \
        main.cpp \
        freyalaunch.cpp

HEADERS += \
        freya_global.h \
        freyalaunch.h

INCLUDEPATH += $$FREYALIB_INCLUDEPATH

LIBS += -L$$PWD/../bin
unix { LIBS += -l$${FREYALIB_NAME} }
win32 { LIBS += -l$${FREYALIB_NAME}5 }
