#include "freyalaunch.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setQuitOnLastWindowClosed(false);

    //set current work path
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    QString launchKey = "";
    if (1 < argc)
    {
        launchKey = QString::fromLocal8Bit(argv[1]);
    }
    FreyaLaunch l(launchKey);
    l.launch();

    return a.exec();
}
