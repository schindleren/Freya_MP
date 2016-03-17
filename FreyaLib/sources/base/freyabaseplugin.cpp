#include "freyabaseplugin.h"

FreyaBasePlugin::FreyaBasePlugin(QString PlatformID, FreyaBaseControl *pControl, const char *objectName) :
    QLocalSocket(), FreyaBaseAction(pControl, objectName), m_Pusher(NULL)
{
    connect(this, SIGNAL(readyRead()), SLOT(OnReadyRead()));

    m_PluginServer = new QLocalServer(this);
    connect(m_PluginServer, SIGNAL(newConnection()), this, SLOT(OnPusherConnected()));

    connectToServer(PlatformID, QIODevice::ReadWrite);
    if(waitForConnected(1000))
    {
        qDebug()<<"Temp=>"<<"FreyaBasePlugin:"<<"Request connected!";
        FreyaBaseData data;
        data.command = FREYALIB_CMD_PLUGINREQUEST;
        write(FreyaBaseData::Serialize(data));
    }
}

bool FreyaBasePlugin::PluginConnected()
{
    return m_PluginServer->isListening();
}

void FreyaBasePlugin::ImportMsgAuth(const QStringList &AuthCode)
{
    if(m_Pusher)
    {
        FreyaBaseData data;
        data.command = FREYALIB_CMD_MSGAUTHREQUEST;
        data.arguments = AuthCode;
        m_Pusher->write(FreyaBaseData::Serialize(data));
    }
}

void FreyaBasePlugin::ImportCmdAuth(const QStringList &AuthCode)
{
    if(m_Pusher)
    {
        FreyaBaseData data;
        data.command = FREYALIB_CMD_CMDAUTHREQUEST;
        data.arguments = AuthCode;
        m_Pusher->write(FreyaBaseData::Serialize(data));
    }
}

void FreyaBasePlugin::Execute(const quint64 &command)
{
    if(m_Pusher)
    {
        FreyaBaseData data;
        data.command = command;
        m_Pusher->write(FreyaBaseData::Serialize(data));
    }
}

void FreyaBasePlugin::Execute(FreyaBaseData *pData)
{
    if(m_Pusher)
    {
        m_Pusher->write(FreyaBaseData::Serialize(*pData));
    }
}

void FreyaBasePlugin::OnReadyRead()
{
    FreyaBaseData data = FreyaBaseData::Unserialize(readAll());
    if(FREYALIB_CMD_PLUGINRESULT == data.command)
    {
        m_PluginServer->listen(data.arguments.toString());
        FreyaBaseData connectData;
        connectData.command = FREYALIB_CMD_CONNECTREQUEST;
        connectData.arguments = m_PluginServer->serverName();
        write(FreyaBaseData::Serialize(connectData));
    }
}

void FreyaBasePlugin::OnPusherConnected()
{
    m_Pusher = m_PluginServer->nextPendingConnection();
    connect(m_Pusher, SIGNAL(readyRead()), this, SLOT(OnPluginReadyRead()));
}

void FreyaBasePlugin::OnPluginReadyRead()
{
    FreyaBaseData data = FreyaBaseData::Unserialize(m_Pusher->readAll());
    if(FREYALIB_CMD_CONNECTRESULT == data.command)
    {
        qDebug()<<"Temp=>"<<"FreyaBasePlugin:"<<"Plugin connect success!";
        emit ToPluginConnected(PluginConnected());
    }
    else
    {
        FREYA_REQUESTEXECUTION(&data);
    }
}
