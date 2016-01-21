#include "freyams_global.h"
#include "freyams.h"
#include <QApplication>
#include <QTranslator>
#include <QFile>
#include <QTextStream>
#include <QtDebug>
#include <QMutex>
#include <QDateTime>

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();

    QString text;
    switch(type)
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
    QString current_date = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString message = QString("%1-%2 %3 ### %4").arg(current_date).arg(text).arg(context_info).arg(msg);
    QFile file("FreyaMS_log.log");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();

    mutex.unlock();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
//    qInstallMessageHandler(outputMessage);

    app.setQuitOnLastWindowClosed(false);

    QTranslator translator;
    translator.load(FREYAMS_PTH_DEFAULTTRANSPATH);
    app.installTranslator(&translator);

    FreyaMS obj;
    obj.InitApplication();

    return app.exec();
}
