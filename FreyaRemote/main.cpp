#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QMutex>
#include <QDateTime>
#include "freyaremote.h"
#include "../FreyaLaunch/freya_global.h"

#define NOLOGTOFILE
#define LOGFILE "./config/freyaremote.log"

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();

    QString text;
    switch((int)type)
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;
    case QtWarningMsg:
        text = QString("Warning:");
        break;
    case QtCriticalMsg:
        text = QString("Critical:");
        break;
    case QtFatalMsg:
        text = QString("Fatal:");
    }
    QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString current_date = QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss zzz");
    QString message = QString("%1-%2 %3 ### %4").arg(current_date).arg(text).arg(msg).arg(context_info);
    QFile file(LOGFILE);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();

    mutex.unlock();
}

int main(int argc,char *argv[])
{
    QCoreApplication app(argc, argv);
    QDir::setCurrent(QCoreApplication::applicationDirPath());

#ifdef LOGTOFILE
    qInstallMessageHandler(outputMessage);
#endif
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

    fprintf(stderr, "FreyaRemote > Wait for connect to FreyaLaunch\n");
    return app.exec();
}
