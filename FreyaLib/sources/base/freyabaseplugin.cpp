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
        qDebug()<<"FreyaLib > "<<"FreyaBasePlugin:"<<"Request connected!";
        FreyaData data = FreyaBaseData::CreateDate();
        data->command = FREYALIB_CMD_PLUGINREQUEST;
        write(FreyaBaseData::Serialize(data));
        flush();
    }
}

FreyaBasePlugin::~FreyaBasePlugin()
{
    if(m_Pusher)
    {
        m_Pusher->disconnectFromServer();
        qDebug()<<"FreyaLib > "<<"Freya Plugin:"<<m_PluginServer->serverName()<<"disconnect:"<<m_Pusher->waitForDisconnected(1000);
        m_Pusher->deleteLater();
    }
}

bool FreyaBasePlugin::PluginConnected()
{
    return m_PluginServer->isListening();
}


bool FreyaBasePlugin::ImportPluginAuth(const QStringList &MsgCode, const QStringList &CmdCode)
{
    if(m_Pusher)
    {
        qDebug()<<"FreyaLib > " << "FreyaBasePlugin:" << "ImportPluginAuth" << MsgCode.size() << CmdCode.size();
        FreyaData data = FreyaBaseData::CreateDate();
        data->command = FREYALIB_CMD_PLUGINAUTHREQUEST;
        QVariantMap dataMap;
        dataMap.insert(FREYALIB_TYP_PLUGINMSGAUTH, MsgCode);
        dataMap.insert(FREYALIB_TYP_PLUGINCMDAUTH, CmdCode);
        data->SetArgument(dataMap);
        return m_Pusher->write(FreyaBaseData::Serialize(data));
    }
    return false;
}

void FreyaBasePlugin::PluginWrite(const quint64 &command)
{
    PluginWrite(FreyaBaseData::CreateDate(command));
}

void FreyaBasePlugin::PluginWrite(const FreyaData pData)
{
    if(m_Pusher)
    {
        pData->SetArgument(FREYALIB_FLG_PLUGINID, m_PluginServer->serverName());
        m_Pusher->write(FreyaBaseData::Serialize(pData));
        m_Pusher->flush();
        qDebug()<<"FreyaLib > " << "FreyaBasePlugin:" << "PluginWriteData" << hex << pData->command;
    }
}

void FreyaBasePlugin::Execute(const FreyaData /*pData*/)
{
}

void FreyaBasePlugin::OnReadyRead()
{
    FreyaData data = FreyaBaseData::Unserialize(readAll());
    if(FREYALIB_CMD_PLUGINRESULT == data->command)
    {
        m_PluginServer->listen(data->GetArgument().toString());
        FreyaData connectData = FreyaBaseData::CreateDate();
        connectData->command = FREYALIB_CMD_CONNECTREQUEST;
        connectData->SetArgument(m_PluginServer->serverName());
        write(FreyaBaseData::Serialize(connectData));
        flush();
    }
}

void FreyaBasePlugin::OnPusherConnected()
{
    m_Pusher = m_PluginServer->nextPendingConnection();
    connect(m_Pusher, SIGNAL(readyRead()), this, SLOT(OnPluginReadyRead()));
}

void FreyaBasePlugin::OnPluginReadyRead()
{
    FreyaData data = FreyaBaseData::Unserialize(m_Pusher->readAll());
    if(FREYALIB_CMD_CONNECTRESULT == data->command)
    {
        qDebug() << "FreyaLib > " << "FreyaBasePlugin:" << "Plugin connect success!";
        emit ToPluginConnected(PluginConnected());
    }
    else
    {
        m_FreyaBaseControl->RequestExecution(data, this);
    }
}
