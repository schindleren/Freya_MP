#include "freyams_global.h"
#include "freyabasecontrol.h"
#include "freyamsclient.h"
#include "freyacryptogram.h"
#include "freyamsconfig.h"

FreyaMSClient::FreyaMSClient(QObject *parent) :
    QXmppClient(parent), FreyaBaseAction(FreyaBaseControl::GetFreyaControl(), FREYAMS_OBJ_XMPPCLIENT)
{
    logger()->setLoggingType(QXmppLogger::FileLogging);
    connect(this, SIGNAL(stateChanged(QXmppClient::State)), SLOT(OnClientStateChanged(QXmppClient::State)));
    connect(this, SIGNAL(iqReceived (QXmppIq)), SLOT(OnIqReceived(QXmppIq)));
    connect(this, SIGNAL(messageReceived(QXmppMessage)), SLOT(OnMessageReceived(QXmppMessage)));
    connect(this, SIGNAL(presenceReceived(QXmppPresence)), SLOT(OnPresenceReceived(QXmppPresence)));
    connect(this, SIGNAL(error(QXmppClient::Error)), SLOT(OnClientError(QXmppClient::Error)));
    connect(this, SIGNAL(loggerChanged(QXmppLogger*)), SLOT(OnLoggerChanged(QXmppLogger*)));

    connect(&rosterManager(), SIGNAL(rosterReceived()), this, SLOT(OnRosterReceived()));
}

FreyaMSClient::~FreyaMSClient()
{
    if(isConnected())
    {
        disconnectFromServer();
    }
}

void FreyaMSClient::Execute()
{
}

void FreyaMSClient::Execute(const quint64 &command)
{
    if(FREYAMS_CMD_LOGINCANCEL == command)
    {
        if(!isConnected())
        {
            disconnectFromServer();
        }
    }
}

void FreyaMSClient::Execute(FreyaBaseData *pData)
{
    if(FREYAMS_CMD_EXCUTECMD == pData->command)
    {
        QVariantMap commandMap = pData->arguments.toMap();
        QByteArray ba;
        QDataStream ds(&ba, QIODevice::WriteOnly);
        ds<<commandMap;
        sendMessage(configuration().jidBare() + "/" + commandMap.value(FREYAMS_STR_RESOURCENAME).toString(),
                    FreyaCryptogram::EnCryptogram(QString::fromUtf8(ba.toHex()), 2));
    }
    else if(FREYAMS_CMD_LOGINDEFAULT == pData->command)
    {
        QXmppConfiguration *pConf = VARIANTTOCUSTOMCLS(pData->arguments, QXmppConfiguration);
        if(pConf)
        {
            if(isConnected())
            {
                if(configuration().password() == pConf->password())
                {
                    OnClientStateChanged(QXmppClient::ConnectedState);
                }
            }
            else
            {
                configuration() = *pConf;
                QXmppPresence pres;
                pres.setType(QXmppPresence::Available);
                pres.setAvailableStatusType(QXmppPresence::Online);
                connectToServer(configuration(), pres);
            }
        }
    }
    else if(FREYAMS_CMD_CONNECTEDSTATE == pData->command)
    {
        if(QXmppClient::ConnectedState == pData->arguments)
        {
            FreyaMSConfig nFreyaConfigData;
            nFreyaConfigData.InsertGlobalData(configuration());
        }
    }
}

void FreyaMSClient::OnClientStateChanged(QXmppClient::State state)
{
    FreyaBaseData ConnectData;
    ConnectData.command = FREYAMS_CMD_CONNECTEDSTATE;
    ConnectData.arguments = state;
    FREYA_REQUESTEXECUTION(&ConnectData);
    qDebug()<<"Temp=>"<<"Jid:"<<configuration().jid();
}

void FreyaMSClient::OnRosterReceived()
{
    qDebug()<<"Temp=>"<<"Default Roster Received:"<<rosterManager().getRosterBareJids();
}

void FreyaMSClient::OnIqReceived(const QXmppIq &iq)
{
    if(iq.to() == configuration().jid())
    {
        FreyaBaseData regIqData;
        regIqData.dataID = iq.id();
        regIqData.command = 0x0;//"Temp=>OnIqReceived";
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

void FreyaMSClient::OnMessageReceived(const QXmppMessage &message)
{
    qDebug()<<"Temp=>"<<"Default Message Received:"<<message.body();
    QVariantMap commandMap;
    QDataStream ds(QByteArray::fromHex(FreyaCryptogram::DeCryptogram(message.body(), 2).toUtf8()));
    ds>>commandMap;
    commandMap.insert(FREYAMS_STR_RESOURCENAME, message.from().split("/").last());
    FreyaBaseData receiverData;
    receiverData.command = FREYAMS_CMD_RECEIVECMD;
    receiverData.arguments = commandMap;
    FREYA_REQUESTEXECUTION(&receiverData);
}

void FreyaMSClient::OnPresenceReceived(const QXmppPresence &presence)
{
    qDebug()<<"Temp=>"<<"Default Presence Received:"<<presence.type();
    FreyaBaseData presenceData;
    presenceData.command = FREYAMS_CMD_PRESENCE;
    QStringList resourceList = rosterManager().getResources(configuration().jidBare());
    resourceList.removeOne(configuration().resource());
    presenceData.arguments = resourceList;
    FREYA_REQUESTEXECUTION(&presenceData);
}

void FreyaMSClient::OnClientError(QXmppClient::Error error)
{
    QString errorString;
    switch (error)
    {
    case QXmppClient::NoError:         //< No error.
        return;
    case QXmppClient::SocketError:     //< Error due to TCP socket.
        errorString = tr("Unable to connect to the server!");
        break;
    case QXmppClient::KeepAliveError:  //< Error due to no response to a keep alive.
        errorString = tr("Abnormal network environment!");
        break;
    case QXmppClient::XmppStreamError: //< Error due to XML stream.
        errorString = tr("Wrong user name or password!");
        break;
    default:
        return;
    }
    FreyaInfor errInfor;
    errInfor.text = errorString;
    errInfor.color = Qt::red;

    FreyaBaseData data;
    data.command = FREYAMS_CMD_SHOWINFOR;
    data.arguments = CUSTOMCLSTOVARIANT(&errInfor);
    FREYA_REQUESTEXECUTION(&data);
}

void FreyaMSClient::OnLoggerChanged(QXmppLogger *logger)
{
    qDebug()<<"Temp=>"<<"Default Logger Changed:"<<logger;
}
