#include "freyaplugin.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FreyaPlugin w;
    w.show();

    return a.exec();
}
