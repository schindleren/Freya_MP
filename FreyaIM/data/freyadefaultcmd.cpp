#include "freyadefaultcmd.h"
#include "freyabasecontrol.h"
#include "QXmppPresence.h"

FreyaDefaultCMD::FreyaDefaultCMD(QObject *parent)
    : QObject(parent)
{
    m_CommandTimer = new QTimer(this);
    m_CommandTimer->setInterval(500);
    connect(m_CommandTimer, SIGNAL(timeout()), this, SLOT(OnOutput()));

    m_CommandProcess = new QProcess(this);
    m_CommandProcess->setWorkingDirectory(QDir::currentPath());
    connect(m_CommandProcess, SIGNAL(finished(int)), this, SLOT(OnOutput()));
    connect(m_CommandProcess, SIGNAL(finished(int)), this, SLOT(OnStopOutput()));
}

void FreyaDefaultCMD::ParseCommand(const QVariantMap &commandMap)
{
    if(commandMap.contains(FREYAMS_STR_COMMANDPATH))
    {
        m_CommandProcess->setWorkingDirectory(commandMap.value(FREYAMS_STR_COMMANDPATH).toString());
    }
    if(commandMap.contains(FREYAMS_STR_COMMANDLINE))
    {
        QString workPath = m_CommandProcess->workingDirectory();
        QString command = commandMap.value(FREYAMS_STR_COMMANDLINE).toString();
        QStringList commandList = command.split(" ");

        if(commandList.size() > 1 && !commandList.at(0).compare("cd", Qt::CaseInsensitive))
        {
            TurnToDir(workPath + "/" + commandList.at(1));
        }
        else if(commandList.size() == 1 && commandList.at(0).contains(":", Qt::CaseInsensitive))
        {
            TurnToDir(commandList.at(0));
        }
        else if(commandList.at(0) == FREYAMS_STR_CMDSTOPCURRENTCMD)
        {
            m_CommandProcess->kill();
            m_CommandTimer->stop();
            SendOutput(FREYAMS_STR_CMDRECEIVEFINISH);
        }
        else
        {
            m_CommandProcess->start("cmd", QStringList()<<"/c"<<command);
            m_CommandProcess->waitForStarted();
            m_CommandTimer->start();
            SendOutput(FREYAMS_STR_CMDRECEIVEBEGIN, QDir::cleanPath(workPath));
        }
    }
    else
    {
        SendOutput(FREYAMS_STR_CMDRECEIVEFINISH);
    }
}

void FreyaDefaultCMD::SendOutput(const QString &ReceiveState, const QString &text)
{
    QVariantMap receiveMap;
    receiveMap.insert(ReceiveState, text);
    emit ToOutput(receiveMap);
}

void FreyaDefaultCMD::TurnToDir(const QString &targetDir)
{
    QString workPath = m_CommandProcess->workingDirectory();
    QDir dir;
    if(dir.exists(targetDir))
    {
        m_CommandProcess->setWorkingDirectory(targetDir);
        workPath = QDir::cleanPath(targetDir);
        SendOutput(FREYAMS_STR_CMDRECEIVEBEGIN, QDir::cleanPath(workPath));
        SendOutput(FREYAMS_STR_CMDRECEIVEFINISH);
    }
    else
    {
        SendOutput(FREYAMS_STR_CMDRECEIVEBEGIN, QDir::cleanPath(workPath));
        SendOutput(FREYAMS_STR_CMDRECEIVE, "Directory is not exists!");
        SendOutput(FREYAMS_STR_CMDRECEIVEFINISH);
    }
}

void FreyaDefaultCMD::OnOutput()
{
    QString text = QString::fromLocal8Bit(m_CommandProcess->readAllStandardOutput());
    if(!text.isEmpty())
    {
        SendOutput(FREYAMS_STR_CMDRECEIVE, text);
    }
}

void FreyaDefaultCMD::OnStopOutput()
{
    m_CommandTimer->stop();
    SendOutput(FREYAMS_STR_CMDRECEIVEFINISH);
}
