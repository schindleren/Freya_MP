#-------------------------------------------------
#
# Project created by QtCreator 2015-04-27T16:49:24
#
#-------------------------------------------------
include(../qxmpp/qxmpp.pri)
include(../FreyaLib/FreyaLib.pri)

CONFIG(debug, debug|release) {
    FREYAIM_NAME = FreyaIM_d
} else {
    FREYAIM_NAME = FreyaIM
}
DESTDIR = $$PWD/../bin
TARGET = $$FREYAIM_NAME
TEMPLATE = app

INCLUDEPATH += $$FREYALIB_INCLUDEPATH \
    $$QXMPP_INCLUDEPATH \
    widgets \
    dialog \
    data \
    xmpp \
    experience

SOURCES += main.cpp\
    freyaim.cpp \
    dialog/freyalogindialog.cpp \
    widgets/freyaportrait.cpp \
    data/freyaconfigdata.cpp \
    xmpp/freyaxmppclient.cpp \
    dialog/freyamaindialog.cpp \
    widgets/freyasystemtrayicon.cpp \
    experience/overicon.cpp \
    widgets/freyabackwindow.cpp \
    dialog/freyaregisterdialog.cpp \
    xmpp/freyadefaultclient.cpp \
    experience/verifier.cpp \
    widgets/freyacontacttree.cpp \
    dialog/freyaidcarddialog.cpp \
    widgets/freyastatebtn.cpp \
    experience/calendaredit.cpp \
    widgets/freyacontactdelegate.cpp \
    data/freyadefaultcmd.cpp \
    data/freyascreencmd.cpp \
    dialog/freyamessagedialog.cpp

HEADERS  += \
    freyaim.h \
    dialog/freyalogindialog.h \
    widgets/freyaportrait.h \
    freyaim_global.h \
    data/freyaconfigdata.h \
    xmpp/freyaxmppclient.h \
    dialog/freyamaindialog.h \
    widgets/freyasystemtrayicon.h \
    experience/overicon.h \
    widgets/freyabackwindow.h \
    dialog/freyaregisterdialog.h \
    xmpp/freyadefaultclient.h \
    experience/verifier.h \
    widgets/freyacontacttree.h \
    dialog/freyaidcarddialog.h \
    widgets/freyastatebtn.h \
    experience/calendaredit.h \
    widgets/freyacontactdelegate.h \
    data/freyadefaultcmd.h \
    data/freyascreencmd.h \
    dialog/freyamessagedialog.h

TRANSLATIONS += $$PWD/resource/freyaimtrans.ts

RC_FILE += $$PWD/resource/freyaimicon.rc

LIBS += $$PWD/../bin/lib$${QXMPP_LIBRARY_NAME}0.a
LIBS += $$PWD/../bin/lib$${FREYALIB_NAME}1.a

RESOURCES += \
    freyaimrc.qrc
