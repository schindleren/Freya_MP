#include "freyalog.h"

void FreyaLog::LoggingMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    bool needFileTip = true;
    QString text;
    switch(type)
    {
    case QtDebugMsg:
    {
        needFileTip = false;
        text = QString("DEBUG");
    }
        break;
    case QtWarningMsg:
    {
        text = QString("WARNING");
    }
        break;
    case QtSystemMsg: //QtCriticalMsg
    {
        text = QString("SYSTEM");
    }
        break;
    case QtFatalMsg:
    {
        text = QString("FATAL");
    }
        break;
    case QtInfoMsg:
    {
        needFileTip = false;
        text = QString("INFO");
    }
        break;
    default:
        text = QString("UNKNOW");
    }

    QString current_date = QDateTime::currentDateTime().toString("yy-MM-dd_hh:mm:ss");
    QString message = QString("%1-%2-%3").arg(current_date).arg(text).arg(msg);
    if(needFileTip)
    {
        QString context_info = QString("F(%1);L(%2)").arg(QString(context.file)).arg(context.line);
        message.append(QString("#%1").arg(context_info));
    }

    emit ToLoggingMessage(message);
}

void FreyaLog::SetLogPath(const QDir &logPath)
{
    m_LogPath = logPath.absolutePath();

    if(!logPath.exists())
    {
        logPath.mkpath(m_LogPath);
    }
}

QString FreyaLog::GetLogPath()
{
    return m_LogPath;
}

void FreyaLog::KeepLogSize(qint64 KBSize)
{
    m_KeepKBSize = KBSize;
}

void FreyaLog::MaxLogFileSize(qint64 KBSize)
{
    m_FileKBSize = KBSize;
}

QString FreyaLog::CreateNewFile()
{
    if(m_LogFile->isOpen())
    {
        m_LogTextStream->flush();
        m_LogFile->flush();
        m_LogFile->close();
    }
    else if(m_LogPath.isEmpty())
    {
        SetLogPath(QDir("./log/"));
    }

    CleanPath();

    QString newFileName = m_LogPath + QDir::separator() + QDateTime::currentDateTime().toString("yyyy-MM-dd.log");
    for(int i = 0; QFile::exists(newFileName); ++i)
    {
        newFileName = m_LogPath + QDir::separator() + QDateTime::currentDateTime().toString("yyyy-MM-dd_%1.log").arg(QString::number(i).rightJustified(3, '0'));
    }

    m_LogFile->setFileName(newFileName);
    if(m_LogFile->open(QFile::WriteOnly))
    {
        m_LogTextStream->setDevice(m_LogFile);

        QString defineString("****************************************************************************\r\n"
                             "Log from FreyaLog -- FreyaLib version: %1 BuildTime: %2 %3\r\n"
                             "****************************************************************************\r\n");
        *m_LogTextStream << defineString.arg(FREYAVER).arg(__DATE__).arg(__TIME__);
        m_LogTextStream->flush();
    }
    else
    {
        qWarning() << "FreyaLib > " << "cannot open log file:" << m_LogFile->errorString();
    }
    return newFileName;
}

void FreyaLog::CleanPath()
{
    QDir logPath(m_LogPath);
    QStringList logList = logPath.entryList(QDir::NoDotAndDotDot | QDir::Files, QDir::Name);

    qint64 filesSize = 0;
    for(int i = logList.size() - 1; i >= 0 ; --i)
    {
        QFile file(logList.at(i));
        filesSize += (file.size() / 1024);
        if(filesSize >= (m_KeepKBSize - m_FileKBSize))
        {
            qInfo() << "FreyaLib > " << "remove log file:" << logList.at(i) << file.remove() << file.errorString();
        }
    }
}

void FreyaLog::FlushLog()
{
    if(m_LogFile->isOpen())
    {
        m_LogTextStream->flush();
        m_LogFile->flush();
    }
}

void FreyaLog::OnLoggingMessage(const QString &message)
{
    if((!m_LogFile->isOpen()) || (m_LogFile->size() > (m_FileKBSize * 1024)))
    {
        CreateNewFile();
    }

    *m_LogTextStream << message << "\r\n";
    m_LogTextStream->flush();


    printf("%s\n", message.toUtf8().constData());
    fflush(stdout);
}

FreyaLog::FreyaLog(QObject *parent) : QObject(parent)
{
    m_LogThread = new QThread(NULL);
    moveToThread(m_LogThread);
    m_LogThread->start();

    m_LogFile = new QFile();

    m_LogTextStream = new QTextStream();
    m_LogTextStream->setCodec("UTF-8");

    m_KeepKBSize = DEFAULTPATHSIZE;
    m_FileKBSize = MAXFILESIZE;

    connect(this, SIGNAL(ToLoggingMessage(QString)), SLOT(OnLoggingMessage(QString)), Qt::QueuedConnection);
}

FreyaLog::~FreyaLog()
{
    FlushLog();
    m_LogFile->close();
    delete m_LogThread;
    delete m_LogTextStream;
    delete m_LogFile;
}

void FreyaLog::FreyaLogMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    GetFreyaLog()->LoggingMessage(type, context, msg);
}
