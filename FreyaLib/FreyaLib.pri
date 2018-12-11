QT      += core gui widgets network
#QT      -= gui widgets

CONFIG(debug, debug|release) {
    FREYALIB_NAME = FreyaLib_d
} else {
    FREYALIB_NAME = FreyaLib
}

unix{
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
SVNVER = 90
message("faild to read svn version, use default version:$${SVNVER}")
}
VERSION = 5.4.3.$$SVNVER
DEFINES += FREYAVER=\\\"$$VERSION\\\"

message("$${FREYALIB_NAME} version:$${VERSION}")

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
