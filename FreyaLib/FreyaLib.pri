
QT       += core gui widgets network sql svg webkit webkitwidgets multimedia

CONFIG(debug, debug|release) {
    FREYALIB_NAME = FreyaLib_d
} else {
    FREYALIB_NAME = FreyaLib
}

FREYALIB_INCLUDEPATH = $$PWD/include \
                        $$PWD/include/abstract \
                        $$PWD/include/base \
                        $$PWD/include/gui \
                        $$PWD/include/experience
