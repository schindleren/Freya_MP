QT      += core gui widgets network
#QT      -= gui widgets

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
message("Now FreyaLib has Gui module")
FREYALIB_INCLUDEPATH += $$PWD/include/gui
}
