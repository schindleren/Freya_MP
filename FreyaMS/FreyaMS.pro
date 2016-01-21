#-------------------------------------------------
#
# Project created by QtCreator 2015-06-12T13:38:45
#
#-------------------------------------------------
include(../qxmpp/qxmpp.pri)
include(../FreyaLib/FreyaLib.pri)

CONFIG(debug, debug|release) {
    FREYAMS_NAME = FreyaMS_d
} else {
    FREYAMS_NAME = FreyaMS
}
DESTDIR = $$PWD/../bin
TARGET = $$FREYAMS_NAME
TEMPLATE = app

INCLUDEPATH += $$FREYALIB_INCLUDEPATH \
    $$QXMPP_INCLUDEPATH \
    widgets \
    dialog \
    data \
    xmpp \
    experience

SOURCES += main.cpp \
    freyams.cpp \
    widgets/freyacmd.cpp \
    widgets/freyamsdialog.cpp \
    widgets/freyamsloginwidget.cpp \
    data/freyamsconfig.cpp \
    xmpp/freyamsclient.cpp \
    widgets/freyamswidget.cpp \
    widgets/freyamsfuntab.cpp

HEADERS  += freyams_global.h \
    freyams.h \
    widgets/freyacmd.h \
    widgets/freyamsdialog.h \
    widgets/freyamsloginwidget.h \
    data/freyamsconfig.h \
    xmpp/freyamsclient.h \
    widgets/freyamswidget.h \
    widgets/freyamsfuntab.h

TRANSLATIONS += $$PWD/resource/freyamstrans.ts

RC_FILE += $$PWD/resource/freyamsicon.rc

LIBS += $$PWD/../bin/lib$${QXMPP_LIBRARY_NAME}0.a
LIBS += $$PWD/../bin/lib$${FREYALIB_NAME}1.a
