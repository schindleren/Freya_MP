#ifndef FREYALOG_H
#define FREYALOG_H

#include "freyalib_global.h"

#include <QObject>
#include <QThread>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QDebug>
#include <QSharedPointer>
#include <QMetaType>

#define DEFAULTPATHSIZE 51200
#define MAXFILESIZE     10240

class FREYALIBSHARED_EXPORT FreyaLog : public QObject
{
    Q_OBJECT
public:
    void LoggingMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    void SetLogPath(const QDir &logPath);
    QString GetLogPath();
    void KeepLogSize(qint64 KBSize = DEFAULTPATHSIZE);
    void MaxLogFileSize(qint64 KBSize = MAXFILESIZE);
    QString CreateNewFile();
    void CleanPath();
    void FlushLog();

private:
    explicit FreyaLog(QObject *parent = nullptr);
    ~FreyaLog();

signals:
    void ToLoggingMessage(const QString &message);

public slots:
    void OnLoggingMessage(const QString &message);

private:
    QThread         *m_LogThread;
    QTextStream     *m_LogTextStream;
    QFile           *m_LogFile;
    QString         m_LogPath;
    qint64          m_KeepKBSize;
    qint64          m_FileKBSize;

public:
    static void FreyaLogMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    static FreyaLog *GetFreyaLog()
    {
        static FreyaLog *pGetFreyaLog = NULL;
        if(NULL == pGetFreyaLog)
        {
            pGetFreyaLog = new FreyaLog();
            qDebug() << "FreyaLib > " << "Structure FreyaLog" << pGetFreyaLog;
        }
        return pGetFreyaLog;
    }
};

#endif // FREYALOG_H
