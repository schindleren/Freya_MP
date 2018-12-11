#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QMutex>
#include <QDateTime>
#include "freyaremote.h"
#include "../FreyaLaunch/freya_global.h"

int main(int argc,char *argv[])
{
    QCoreApplication app(argc, argv);
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    FreyaRemote r;

    int arg = 1;
    if (arg < argc)
    {
        QString opt = QString::fromLocal8Bit(argv[arg]);
        if (opt == QLatin1String("h") || opt == QLatin1String("help"))
        {
            FreyaRemote::ShowHelp(argv[0]);
            return 0;
        }
    }

    r.ConnectToLaunch(argc, argv);

    fprintf(stdout, "FreyaRemote > Wait for connect to FreyaLaunch\n");
    return app.exec();
}
