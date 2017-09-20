#include "freyatest.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    FreyaTest w;
    w.show();

    return a.exec();
}
