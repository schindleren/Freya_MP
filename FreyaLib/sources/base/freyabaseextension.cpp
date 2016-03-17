#include "freyapublicregister.h"
#include "freyabaseextension.h"
#include <QtMath>

FreyaPluginPusher::FreyaPluginPusher(QString PluginID, QObject *parent) :
    QLocalSocket(parent), m_MsgAuth(0), m_CmdAuth(0), m_PluginID(PluginID)
{
    qDebug()<<"Temp=>"<<"FreyaPluginPusher:ID:"<<PluginID;
    connect(this, SIGNAL(stateChanged(QLocalSocket::LocalSocketState)), SLOT(OnStateChanged(QLocalSocket::LocalSocketState)));
    connectToServer(PluginID, QIODevice::ReadWrite);
    if(waitForConnected(1000))
    {
        qDebug()<<"Temp=>"<<"FreyaPluginPusher:"<<"success!";
        connect(this, SIGNAL(readyRead()), SLOT(OnReadyRead()));
        FreyaBaseData data;
        data.command = FREYALIB_CMD_CONNECTRESULT;
        write(FreyaBaseData::Serialize(data));
    }
}

void FreyaPluginPusher::PusherExcute(const quint64 &command)
{
    if(m_MsgAuth & command)
    {
        qDebug()<<"PusherExcute:"<<hex<<"Command:"<<command<<"To:"<<m_PluginID;
        FreyaBaseData data;
        data.command = command;
        write(FreyaBaseData::Serialize(data));
    }
}

void FreyaPluginPusher::PusherExcute(FreyaBaseData *pData)
{
    if(m_MsgAuth & pData->command)
    {
        qDebug()<<"PusherExcute:"<<hex<<"Command:"<<pData->command<<"To:"<<m_PluginID;
        write(FreyaBaseData::Serialize(*pData));
    }
}

void FreyaPluginPusher::OnReadyRead()
{
    FreyaBaseData data = FreyaBaseData::Unserialize(readAll());
    qDebug()<<"DDDDDDDDDDDDDDDDDDD"<<hex<<data.command;
    FreyaBaseControl *pControl = FreyaBaseControl::GetFreyaControl();
    if(FREYALIB_CMD_MSGAUTHREQUEST == data.command)
    {
        QStringList AuthCodeList = data.arguments.toStringList();
        foreach (const QString &code, AuthCodeList)
        {
            FreyaBaseData *pData = pControl->FindBaseData(code);
            if(pData)
            {
                m_MsgAuth = (m_MsgAuth | pData->arguments.toInt());
            }
        }
        qDebug()<<"PluginID:"<<m_PluginID<<"MsgAuth:"<<hex<<m_MsgAuth;
    }
    else if(FREYALIB_CMD_CMDAUTHREQUEST == data.command)
    {
        QStringList AuthCodeList = data.arguments.toStringList();
        foreach (const QString &code, AuthCodeList)
        {
            FreyaBaseData *pData = pControl->FindBaseData(code);
            if(pData)
            {
                m_CmdAuth = (m_CmdAuth | pData->arguments.toInt());
            }
        }
        qDebug()<<"PluginID:"<<m_PluginID<<"CmdAuth:"<<hex<<m_CmdAuth;
    }
    else // cmd request from plugin
    {
        if(m_CmdAuth & data.command)
        {
            qDebug()<<"Allow up command:"<<data.command<<"From:"<<m_PluginID;
            pControl->RequestExecution(&data, parent());
        }
    }
}

void FreyaPluginPusher::OnStateChanged(QLocalSocket::LocalSocketState state)
{
    switch(state)
    {
    case UnconnectedState:
    {
        qDebug()<<"Temp=>"<<"FreyaPluginPusher::OnStateChanged:"<<"UnconnectedState";
        emit ToDisconnected();
    }
        break;

    case ConnectingState:
    {
        qDebug()<<"Temp=>"<<"FreyaPluginPusher::OnStateChanged:"<<"ConnectingState";
    }
        break;

    case ConnectedState:
    {
        qDebug()<<"Temp=>"<<"FreyaPluginPusher::OnStateChanged:"<<"ConnectedState";
    }
        break;

    case ClosingState:
    {
        qDebug()<<"Temp=>"<<"FreyaPluginPusher::OnStateChanged:"<<"ClosingState";
        emit ToDisconnected();
    }
        break;
    default:
        break;
    }
}

///////////////

FreyaBaseExtension::FreyaBaseExtension(QString PlatformID, FreyaBaseControl *pControl, const char *objectName) :
    QLocalServer(), FreyaBaseAction(pControl, objectName)
{
    listen(PlatformID);
    connect(this, SIGNAL(newConnection()), SLOT(OnPluginRequest()));
}

void FreyaBaseExtension::DefineAuthCode(const QStringList &MsgAuth, const QStringList &CmdAuth)
{
    FreyaBaseControl *pControl = FreyaBaseControl::GetFreyaControl();
    for(int i = 0; i < qMin(MsgAuth.size(), 4); ++i)
    {
        FreyaBaseData *pData = new FreyaBaseData;
        pData->dataID = MsgAuth.at(i);
        pData->arguments = qPow(2, i); //0x01 0x02 0x04 0x08
        pControl->InsertBaseData(pData);
    }
    for(int j = 0; j < qMin(CmdAuth.size(), 4); ++j)
    {
        FreyaBaseData *pData = new FreyaBaseData;
        pData->dataID = CmdAuth.at(j);
        pData->arguments = qPow(2, j) * 0x10; //0x10 0x20 0x40 0x80
        pControl->InsertBaseData(pData);
    }
}

void FreyaBaseExtension::Execute(const quint64 &command)
{
    qDebug()<<"Ext_Execution:"<<"Command:"<<hex<<command;
    QListIterator<FreyaPluginPusher*> PusherIT(m_PusherList);
    while(PusherIT.hasNext())
    {
        PusherIT.next()->PusherExcute(command);
    }
}

void FreyaBaseExtension::Execute(FreyaBaseData *pData)
{
    qDebug()<<"Ext_Execution:"<<"DataID:"<<pData->dataID<<"Command:"<<hex<<pData->command<<dec<<"Arguments:"<<pData->arguments;
    QListIterator<FreyaPluginPusher*> PusherIT(m_PusherList);
    while(PusherIT.hasNext())
    {
        PusherIT.next()->PusherExcute(pData);
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
        FreyaBaseData data = FreyaBaseData::Unserialize(plugin->readAll());
        if(FREYALIB_CMD_PLUGINREQUEST == data.command)
        {
            qDebug()<<"Temp=>"<<"FreyaBaseExtension:"<<hex<<FREYALIB_CMD_PLUGINREQUEST;
            FreyaBaseData resultData;
            resultData.command = FREYALIB_CMD_PLUGINRESULT;
            QString PluginID = QUuid::createUuid().toString().toLower();
            resultData.arguments = PluginID;
            m_CurrentPluginID = PluginID;
            plugin->write(FreyaBaseData::Serialize(resultData));
        }
        else if(FREYALIB_CMD_CONNECTREQUEST == data.command)
        {
            QString PluginID = data.arguments.toString().toLower();
            qDebug()<<"Temp=>"<<"FreyaBaseExtension:"<<hex<<FREYALIB_CMD_CONNECTREQUEST<<PluginID;
            if(m_CurrentPluginID == PluginID)
            {
                FreyaPluginPusher *pPusher = new FreyaPluginPusher(PluginID, this);
                connect(pPusher, SIGNAL(ToDisconnected()), this, SLOT(OnPusherDisconnected()));
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

/*
 * Extension Platform      Plugin
 * _auth                    _request
 * |                        |
 * |-0x01                   |-ID
 * |-0x02                   |-*name
 * |-0x04                   |-*path
 * |-0x08                   |-*cfgMd5
 * |-...                    |-...
 *
 *         connect(cfgMd5)
 *     <-------------------
 *         request     (FREYALIB_CMD_PLUGINREQUEST)
 *     <-------------------
 *         result (ID) (FREYALIB_CMD_PLUGINRESULT)
 *     ------------------->
 *         connectPlugin(ID)
 *     <-------------------
 *         connect
 *     ------------------->
 *         testCommand(data)
 *     ------------------->
 *          .
 *          .
 *          .
 *         authCode
 *     <-------------------
 */
