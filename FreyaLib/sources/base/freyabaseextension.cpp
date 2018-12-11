#include "freyapublicregister.h"
#include "freyabaseextension.h"
#include <QtMath>

FreyaPluginPusher::FreyaPluginPusher(QString PluginID, FreyaBaseControl *pControl, QObject *parent) :
    QObject(parent), m_MsgAuth(0), m_CmdAuth(0), m_PluginID(PluginID), m_FreyaControl(pControl)
{
    qDebug()<<"FreyaLib > "<<"FreyaPluginPusher:ID:"<<PluginID;

    m_Pusher = new QLocalSocket(this);
    connect(m_Pusher, SIGNAL(stateChanged(QLocalSocket::LocalSocketState)), this, SLOT(OnStateChanged(QLocalSocket::LocalSocketState)));
    m_Pusher->connectToServer(PluginID, QIODevice::ReadWrite);
    if(m_Pusher->waitForConnected(1000))
    {
        qDebug()<<"FreyaLib > "<<"FreyaPluginPusher:ID:"<<m_PluginID<<"SUCCESS!";
        connect(m_Pusher, SIGNAL(readyRead()), this, SLOT(OnReadyRead()));
        connect(m_Pusher, SIGNAL(disconnected()), this, SIGNAL(ToDisconnected()));
        FreyaData data = FreyaBaseData::CreateDate();
        data->command = FREYALIB_CMD_CONNECTRESULT;
        m_Pusher->write(FreyaBaseData::Serialize(data));
        m_Pusher->flush();
    }
}

void FreyaPluginPusher::PusherExecute(const FreyaData data)
{
    if(m_MsgAuth & data->command)
    {
        qDebug()<<"FreyaLib > "<<"PusherExcute:"<<hex<<"Command:"<<data->command<<"To:"<<m_PluginID;
        m_Pusher->write(FreyaBaseData::Serialize(data));
        m_Pusher->flush();
    }
}

QString &FreyaPluginPusher::FreyaPluginID()
{
    return m_PluginID;
}

void FreyaPluginPusher::OnReadyRead()
{
    FreyaData data(FreyaBaseData::Unserialize(m_Pusher->readAll()));
    if(data.isNull())
        return;

    if(FREYALIB_CMD_PLUGINAUTHREQUEST == data->command)
    {
        QVariantMap AuthCodeMap = data->GetArgument().toMap();
        QStringList MsgAuthList = AuthCodeMap.value(FREYALIB_TYP_PLUGINMSGAUTH).toStringList();
        QStringList CmdAuthList = AuthCodeMap.value(FREYALIB_TYP_PLUGINCMDAUTH).toStringList();
        foreach (const QString &MsgCode, MsgAuthList)
        {
            FreyaData pCodeData = m_FreyaControl->FindFreyaData(MsgCode);
            if(pCodeData)
            {
                m_MsgAuth = (m_MsgAuth | pCodeData->GetArgument().toInt());
            }
        }
        qDebug() << "FreyaLib > " << "PluginID:" << m_PluginID << "MsgAuth:" << hex << m_MsgAuth;
        foreach (const QString &CmdCode, CmdAuthList)
        {
            FreyaData pCodeData = m_FreyaControl->FindFreyaData(CmdCode);
            if(pCodeData)
            {
                m_CmdAuth = (m_CmdAuth | pCodeData->GetArgument().toInt());
            }
        }
        qDebug() << "FreyaLib > " << "PluginID:" << m_PluginID << "CmdAuth:" << hex << m_CmdAuth;
    }
    else // cmd request from plugin
    {
        if(m_CmdAuth & data->command)
        {
            qDebug() << "FreyaLib > " << "Allow up command:" << hex << data->command << "From:" << m_PluginID;
            emit ToPluginRequest(data);
        }
        else
        {
            qDebug() << "FreyaLib > " << "Not allow up command:" << hex << data->command << "From:" << m_PluginID;
        }
    }
}

void FreyaPluginPusher::OnStateChanged(QLocalSocket::LocalSocketState state)
{
    switch(state)
    {
    case QLocalSocket::UnconnectedState:
    {
        qDebug()<<"FreyaLib > "<<"FreyaPluginPusher::OnStateChanged:"<<"UnconnectedState";
    }
        break;

    case QLocalSocket::ConnectingState:
    {
        qDebug()<<"FreyaLib > "<<"FreyaPluginPusher::OnStateChanged:"<<"ConnectingState";
    }
        break;

    case QLocalSocket::ConnectedState:
    {
        qDebug()<<"FreyaLib > "<<"FreyaPluginPusher::OnStateChanged:"<<"ConnectedState";
    }
        break;

    case QLocalSocket::ClosingState:
    {
        qDebug()<<"FreyaLib > "<<"FreyaPluginPusher::OnStateChanged:"<<"ClosingState";
    }
        break;
    default:
        break;
    }
}

///////////////

FreyaBaseExtension::FreyaBaseExtension(QString PlatformID, FreyaBaseControl *pControl, const char *objectName) :
    QLocalServer(), FreyaBaseAction(pControl, objectName), m_FreyaControl(pControl), m_isListening(false)
{
    m_isListening = listen(PlatformID);
    connect(this, SIGNAL(newConnection()), SLOT(OnPluginRequest()), Qt::QueuedConnection);
    m_Thread = new QThread;
    moveToThread(m_Thread);
    m_Thread->start();
}

FreyaBaseExtension::~FreyaBaseExtension()
{
    m_Thread->terminate();
    m_Thread->wait();
    delete m_Thread;
}

bool FreyaBaseExtension::DefineAuthCode(const QStringList &MsgAuth, const QStringList &CmdAuth)
{
    for(int i = 0; i < qMin(MsgAuth.size(), 4); ++i)
    {
        FreyaData pData = FreyaBaseData::CreateDate();
        pData->dataID = MsgAuth.at(i);
        pData->SetArgument(qPow(2, i)); //0x01 0x02 0x04 0x08
        m_FreyaBaseControl->InsertFreyaData(pData);
    }
    for(int j = 0; j < qMin(CmdAuth.size(), 4); ++j)
    {
        FreyaData pData = FreyaBaseData::CreateDate();
        pData->dataID = CmdAuth.at(j);
        pData->SetArgument(qPow(2, j) * 0x10); //0x10 0x20 0x40 0x80
        m_FreyaBaseControl->InsertFreyaData(pData);
    }
    return m_isListening;
}

void FreyaBaseExtension::Execute(const FreyaData data)
{
    QMutexLocker locker(&m_PluginListMutex);
    if(!m_PusherList.isEmpty())
    {
        qDebug() << "FreyaLib > " << "Ext_Execution:" << "DataID:" << data->dataID << "Command:" << hex << data->command << dec << "Arguments:" << data->GetArgument();
    }
    QListIterator<FreyaPluginPusher*> PusherIT(m_PusherList);
    while(PusherIT.hasNext())
    {
        FreyaPluginPusher* pPusher = PusherIT.next();
        if(pPusher && pPusher->FreyaPluginID() != data->GetArgument(FREYALIB_FLG_PLUGINID))
        {
            pPusher->PusherExecute(data);
        }
    }
}

void FreyaBaseExtension::OnPluginRequest()
{
    QLocalSocket *plugin = nextPendingConnection();
    connect(plugin, SIGNAL(readyRead()), this, SLOT(OnReadyRead()), Qt::QueuedConnection);
}

void FreyaBaseExtension::OnReadyRead()
{
    QLocalSocket *plugin = qobject_cast<QLocalSocket *>(sender());
    if (plugin)
    {
        FreyaData data = FreyaBaseData::Unserialize(plugin->readAll());
        if(FREYALIB_CMD_PLUGINREQUEST == data->command)
        {
            qDebug() << "FreyaLib > " << "FreyaBaseExtension:" << hex << FREYALIB_CMD_PLUGINREQUEST;
            FreyaData resultData = FreyaBaseData::CreateDate();
            resultData->command = FREYALIB_CMD_PLUGINRESULT;
            QString PluginID = QUuid::createUuid().toString().toLower();
            resultData->SetArgument(PluginID);
            m_WaitPluginIDList.append(PluginID);
            plugin->write(FreyaBaseData::Serialize(resultData));
            plugin->flush();
        }
        else if(FREYALIB_CMD_CONNECTREQUEST == data->command)
        {
            connect(plugin, SIGNAL(disconnected()), plugin, SLOT(deleteLater()));
            QString PluginID = data->GetArgument().toString().toLower();
            qDebug() << "FreyaLib > " << "FreyaBaseExtension:" << hex << FREYALIB_CMD_CONNECTREQUEST << PluginID;
            if(m_WaitPluginIDList.contains(PluginID))
            {
                m_WaitPluginIDList.removeOne(PluginID);
                FreyaPluginPusher* pPusher = new FreyaPluginPusher(PluginID, m_FreyaBaseControl);
                connect(pPusher, SIGNAL(ToPluginRequest(FreyaData)), this, SLOT(OnPuserRequest(FreyaData)), Qt::QueuedConnection);
                connect(pPusher, SIGNAL(ToDisconnected()), this, SLOT(OnPusherDisconnected()), Qt::QueuedConnection);
                m_PusherList.append(pPusher);
            }
            plugin->disconnectFromServer();
        }
    }
}

void FreyaBaseExtension::OnPusherDisconnected()
{
    QMutexLocker locker(&m_PluginListMutex);
    FreyaPluginPusher* pPusher = qobject_cast<FreyaPluginPusher*>(sender());
    if(pPusher && m_PusherList.removeOne(pPusher))
    {
        qDebug() << "FreyaLib > " << "PusherDisconnected:" << pPusher->FreyaPluginID();
        pPusher->deleteLater();
    }
}

void FreyaBaseExtension::OnPuserRequest(const FreyaData data)
{
    m_FreyaControl->RequestExecution(data, this);
}
