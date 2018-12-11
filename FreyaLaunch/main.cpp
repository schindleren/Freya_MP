#include "freyalaunch.h"
#include "freyalog.h"
#include "freya_global.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setQuitOnLastWindowClosed(false);

    //set current work path
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    qInstallMessageHandler(FreyaLog::FreyaLogMessage);
    qputenv("path", qgetenv("path").append(QString(";%1;%2").arg(MODULEDIR).arg(MODULEDEPENDDIR)));

    QString launchKey = "";
    if (1 < argc)
    {
        launchKey = QString::fromLocal8Bit(argv[1]);
    }
    FreyaLaunch l(launchKey);
    l.launch();

    return a.exec();
}
