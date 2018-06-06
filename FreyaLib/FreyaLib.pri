QT      += core gui widgets network
#QT      -= gui widgets

unix {
SVNVER = $$system(svn info | grep 'Changed\ Rev' | cut -b 19-)
isEmpty($$SVNVER){
SVNVER = $$system(svn info | grep '最后修改的版本' | cut -b 24-)
}
} win32 { SVNVER = 16 }

VERSION = 5.3.0.$$SVNVER
DEFINES += FREYAVER=\\\"$$VERSION\\\"

CONFIG(debug, debug|release) {
    FREYALIB_NAME = FreyaLib_d
} else {
    FREYALIB_NAME = FreyaLib
}

FREYALIB_INCLUDEPATH = $$PWD/include \
                        $$PWD/include/abstract \
                        $$PWD/include/base \
                        $$PWD/include/experience

contains(QT, gui) {
message("Now FreyaLib HAS GUI module")
FREYALIB_INCLUDEPATH += $$PWD/include/gui
}else{
message("Now FreyaLib has NO gui module")
}
