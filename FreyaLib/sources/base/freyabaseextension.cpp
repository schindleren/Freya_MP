#include "freyapublicregister.h"
#include "freyabaseextension.h"
#include <QtMath>

FreyaPluginPusher::FreyaPluginPusher(QString PluginID, FreyaBaseControl *pControl, QObject *parent) :
    QThread(parent), m_MsgAuth(0), m_CmdAuth(0), m_PluginID(PluginID), m_FreyaControl(pControl)
{
    qDebug()<<"FreyaLib > "<<"FreyaPluginPusher:ID:"<<PluginID;
    m_Pusher = new QLocalSocket(this);
    connect(m_Pusher, SIGNAL(stateChanged(QLocalSocket::LocalSocketState)), this, SLOT(OnStateChanged(QLocalSocket::LocalSocketState)));
    m_Pusher->connectToServer(PluginID, QIODevice::ReadWrite);
    if(m_Pusher->waitForConnected(1000))
    {
        qDebug()<<"FreyaLib > "<<"FreyaPluginPusher:"<<"success!";
        connect(m_Pusher, SIGNAL(readyRead()), this, SLOT(OnReadyRead()));
        FreyaData data = FreyaBaseData::CreateDate();
        data->command = FREYALIB_CMD_CONNECTRESULT;
        m_Pusher->write(FreyaBaseData::Serialize(data));
    }
}

void FreyaPluginPusher::PusherExcute(const quint64 &command)
{
    if(m_MsgAuth & command)
    {
        qDebug()<<"FreyaLib > "<<"PusherExcute:"<<hex<<"Command:"<<command<<"To:"<<m_PluginID;
        FreyaData data = FreyaBaseData::CreateDate();
        data->command = command;
        m_Pusher->write(FreyaBaseData::Serialize(data));
    }
}

void FreyaPluginPusher::PusherExcute(const FreyaData data)
{
    if(m_MsgAuth & data->command)
    {
        qDebug()<<"FreyaLib > "<<"PusherExcute:"<<hex<<"Command:"<<data->command<<"To:"<<m_PluginID;
        m_Pusher->write(FreyaBaseData::Serialize(data));
    }
}

void FreyaPluginPusher::run()
{
    while(!m_DataIDList.isEmpty())
    {
        FreyaData pData = m_FreyaControl->TakeFreyaData(m_DataIDList.takeFirst());
        if(!pData)
            return;

        if(FREYALIB_CMD_PLUGINAUTHREQUEST == pData->command)
        {
            QVariantMap AuthCodeMap = pData->arguments.toMap();
            QStringList MsgAuthList = AuthCodeMap.value(FREYALIB_TYP_PLUGINMSGAUTH).toStringList();
            QStringList CmdAuthList = AuthCodeMap.value(FREYALIB_TYP_PLUGINCMDAUTH).toStringList();
            foreach (const QString &MsgCode, MsgAuthList)
            {
                FreyaData pCodeData = m_FreyaControl->FindFreyaData(MsgCode);
                if(pCodeData)
                {
                    m_MsgAuth = (m_MsgAuth | pCodeData->arguments.toInt());
                }
            }
            qDebug()<<"FreyaLib > "<<"PluginID:"<<m_PluginID<<"MsgAuth:"<<hex<<m_MsgAuth;
            foreach (const QString &CmdCode, CmdAuthList)
            {
                FreyaData pCodeData = m_FreyaControl->FindFreyaData(CmdCode);
                if(pCodeData)
                {
                    m_CmdAuth = (m_CmdAuth | pCodeData->arguments.toInt());
                }
            }
            qDebug()<<"FreyaLib > "<<"PluginID:"<<m_PluginID<<"CmdAuth:"<<hex<<m_CmdAuth;
        }
        else // cmd request from plugin
        {
            if(m_CmdAuth & pData->command)
            {
                qDebug()<<"FreyaLib > "<<"Allow up command:"<<hex<<pData->command<<"From:"<<m_PluginID;
                FreyaData aData(pData);
                emit ToPluginRequest(aData);
            }
        }
//        delete pData;
    }
}

void FreyaPluginPusher::OnReadyRead()
{
    FreyaData data = FreyaBaseData::Unserialize(m_Pusher->readAll());
    m_FreyaControl->InsertFreyaData(data);
    m_DataIDList.append(data->dataID);

    if(!isRunning())
    {
        start();
    }
}

void FreyaPluginPusher::OnStateChanged(QLocalSocket::LocalSocketState state)
{
    switch(state)
    {
    case QLocalSocket::UnconnectedState:
    {
        qDebug()<<"FreyaLib > "<<"FreyaPluginPusher::OnStateChanged:"<<"UnconnectedState";
        emit ToDisconnected();
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
        emit ToDisconnected();
    }
        break;
    default:
        break;
    }
}

///////////////

FreyaBaseExtension::FreyaBaseExtension(QString PlatformID, FreyaBaseControl *pControl, const char *objectName) :
    QLocalServer(), FreyaBaseAction(pControl, objectName), m_FreyaControl(pControl)
{
    listen(PlatformID);
    connect(this, SIGNAL(newConnection()), SLOT(OnPluginRequest()));
}

void FreyaBaseExtension::DefineAuthCode(const QStringList &MsgAuth, const QStringList &CmdAuth)
{
    for(int i = 0; i < qMin(MsgAuth.size(), 4); ++i)
    {
        FreyaData pData = FreyaBaseData::CreateDate();
        pData->dataID = MsgAuth.at(i);
        pData->arguments = qPow(2, i); //0x01 0x02 0x04 0x08
        m_FreyaBaseControl->InsertFreyaData(pData);
    }
    for(int j = 0; j < qMin(CmdAuth.size(), 4); ++j)
    {
        FreyaData pData = FreyaBaseData::CreateDate();
        pData->dataID = CmdAuth.at(j);
        pData->arguments = qPow(2, j) * 0x10; //0x10 0x20 0x40 0x80
        m_FreyaBaseControl->InsertFreyaData(pData);
    }
}

void FreyaBaseExtension::Execute(const quint64 &command)
{
    qDebug()<<"FreyaLib > "<<"Ext_Execution:"<<"Command:"<<hex<<command;
    QListIterator<FreyaPluginPusher*> PusherIT(m_PusherList);
    while(PusherIT.hasNext())
    {
        PusherIT.next()->PusherExcute(command);
    }
}

void FreyaBaseExtension::Execute(const FreyaData data)
{
    qDebug()<<"FreyaLib > "<<"Ext_Execution:"<<"DataID:"<<data->dataID<<"Command:"<<hex<<data->command<<dec<<"Arguments:"<<data->arguments;
    QListIterator<FreyaPluginPusher*> PusherIT(m_PusherList);
    while(PusherIT.hasNext())
    {
        PusherIT.next()->PusherExcute(data);
    }
}

void FreyaBaseExtension::OnPluginRequest()
{
    QLocalSocket *plugin = nextPendingConnection();
    connect(plugin, SIGNAL(readyRead()), this, SLOT(OnReadyRead()));
}

void FreyaBaseExtension::OnReadyRead()
{
    QLocalSocket *plugin = qobject_cast<QLocalSocket *>(sender());
    if (plugin)
    {
        FreyaData data = FreyaBaseData::Unserialize(plugin->readAll());
        if(FREYALIB_CMD_PLUGINREQUEST == data->command)
        {
            qDebug()<<"FreyaLib > "<<"FreyaBaseExtension:"<<hex<<FREYALIB_CMD_PLUGINREQUEST;
            FreyaData resultData = FreyaBaseData::CreateDate();
            resultData->command = FREYALIB_CMD_PLUGINRESULT;
            QString PluginID = QUuid::createUuid().toString().toLower();
            resultData->arguments = PluginID;
            m_CurrentPluginID = PluginID;
            plugin->write(FreyaBaseData::Serialize(resultData));
        }
        else if(FREYALIB_CMD_CONNECTREQUEST == data->command)
        {
            QString PluginID = data->arguments.toString().toLower();
            qDebug()<<"FreyaLib > "<<"FreyaBaseExtension:"<<hex<<FREYALIB_CMD_CONNECTREQUEST<<PluginID;
            if(m_CurrentPluginID == PluginID)
            {
                FreyaPluginPusher *pPusher = new FreyaPluginPusher(PluginID, m_FreyaBaseControl, this);
                connect(pPusher, SIGNAL(ToDisconnected()), this, SLOT(OnPusherDisconnected()));
                connect(pPusher, SIGNAL(ToPluginRequest(FreyaData)), this, SLOT(OnPuserRequest(FreyaData)), Qt::BlockingQueuedConnection);
                m_PusherList.append(pPusher);
            }
        }
    }
}

void FreyaBaseExtension::OnPusherDisconnected()
{
    FreyaPluginPusher *pPusher = qobject_cast<FreyaPluginPusher*>(sender());
    if(m_PusherList.removeOne(pPusher))
    {
        pPusher->deleteLater();
    }
}

void FreyaBaseExtension::OnPuserRequest(const FreyaData data)
{
    m_FreyaControl->RequestExecution(data, this);
}
