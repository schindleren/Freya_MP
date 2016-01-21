#include "freyaim_global.h"
#include "freyabasecontrol.h"
#include "freyaxmppclient.h"
#include "freyaportrait.h"
#include "freyamessagedialog.h"
#include <QSystemTrayIcon>
#include <QBuffer>

FreyaXmppClient::FreyaXmppClient(QObject *parent) :
    QXmppClient(parent), FreyaBaseAction(FreyaBaseControl::GetFreyaControl(), FREYA_OBJ_XMPPCLIENTNAME)
{
    logger()->setLoggingType(QXmppLogger::FileLogging);
    connect(this, SIGNAL(stateChanged(QXmppClient::State)), SLOT(OnClientStateChanged(QXmppClient::State)));
    connect(this, SIGNAL(iqReceived(QXmppIq)), SLOT(OnIqReceived(QXmppIq)));
    connect(this, SIGNAL(messageReceived(QXmppMessage)), SLOT(OnMessageReceived(QXmppMessage)));
    connect(this, SIGNAL(presenceReceived(QXmppPresence)), SLOT(OnPresenceReceived(QXmppPresence)));
    connect(this, SIGNAL(error(QXmppClient::Error)), SLOT(OnClientError(QXmppClient::Error)));
    connect(this, SIGNAL(loggerChanged(QXmppLogger*)), SLOT(OnLoggerChanged(QXmppLogger*)));

    connect(&rosterManager(), SIGNAL(rosterReceived()), this, SLOT(OnRosterReceived()));
    connect(&rosterManager(), SIGNAL(subscriptionReceived(QString)), this, SLOT(OnSubscriptionReceived(QString)));
    connect(&vCardManager(), SIGNAL(clientVCardReceived()), this, SLOT(OnClientVCardReceived()));
    connect(&vCardManager(), SIGNAL(vCardReceived(QXmppVCardIq)), this, SLOT(OnVCardReceived(QXmppVCardIq)));
}

FreyaXmppClient::~FreyaXmppClient()
{
    if(isConnected())
    {
        disconnectFromServer();
    }
}

void FreyaXmppClient::Execute()
{
}

void FreyaXmppClient::Execute(const quint64 &command)
{
    if(FREYA_CMD_LOGOUTEDSTATE == command)
    {
        disconnectFromServer();
    }
    else if(FREYA_CMD_VCARDREQUEST == command)
    {
        vCardManager().requestClientVCard();
    }
    else if(FREYA_CMD_CONFIGNREQUEST == command)
    {
        QXmppConfiguration xmppConfig = configuration();
        FreyaBaseData configData;
        configData.command = FREYA_CMD_CONFIGNRESULT;
        configData.arguments = CUSTOMCLSTOVARIANT(&xmppConfig);
        FREYA_REQUESTEXECUTION(&configData);
    }
}

void FreyaXmppClient::Execute(FreyaBaseData *pData)
{
    if(FREYA_CMD_CLIENTLOGIN == pData->command)
    {
        QVariantMap varMap = pData->arguments.toMap();
        QXmppConfiguration *pConf = VARIANTTOCUSTOMCLS(varMap.value(FREYA_CLS_XMPPCONFIGURATION), QXmppConfiguration);
        if(pConf)
        {
            configuration() = *pConf;
        }
        QXmppPresence pres;
        pres.setType(QXmppPresence::Available);
        pres.setAvailableStatusType((QXmppPresence::AvailableStatusType)varMap.value(FREYA_CLS_XMPPPRESENCESTATUS).toInt());
        connectToServer(configuration(), pres);
    }
    else if(FREYA_CMD_VCARDREQUEST == pData->command)
    {
        if(!pData->arguments.isNull())
        {
            FreyaBaseData *VCardRequestData = new FreyaBaseData(*pData);
            VCardRequestData->dataID = vCardManager().requestVCard(pData->arguments.toString());
            FreyaBaseControl::GetFreyaControl()->InsertBaseData(VCardRequestData);
        }
    }
    else if(FREYA_CMD_VCARDMODREQUEST == pData->command)
    {
        if(pData->arguments.toString() == configuration().jidBare())
        {
            SaveVcardToServer();
        }
    }
    else if(FREYA_CMD_MODIFYPRESENCE == pData->command)
    {
        QXmppPresence *pPresence = VARIANTTOCUSTOMCLS(pData->arguments, QXmppPresence);
        if(pPresence)
        {
            setClientPresence(*pPresence);
        }
    }
    else if(FREYA_CMD_SUBSCRIBERESULT == pData->command)
    {
        QVariantMap varMap = pData->arguments.toMap();
        if(varMap.value(FREYA_CLS_MESSAGEBOXDECISION).toBool())
        {
            rosterManager().acceptSubscription(varMap.value(FREYA_STR_XMPPBAREJID).toString());
            rosterManager().subscribe(varMap.value(FREYA_STR_XMPPBAREJID).toString());
        }
        else
        {
            rosterManager().refuseSubscription(varMap.value(FREYA_STR_XMPPBAREJID).toString());
            rosterManager().removeItem(varMap.value(FREYA_STR_XMPPBAREJID).toString());
        }
    }
}

void FreyaXmppClient::SaveVcardToLocal(const QXmppVCardIq &vCardIq, const QString &bareJid)
{
    FreyaBaseControl *pControl = FreyaBaseControl::GetFreyaControl();
    pControl->GetConfigFromFile(FREYA_FUN_PERSONCONFIGPATH(bareJid) + FREYA_PTH_PERSONCONFIGPATH);

    pControl->InsertConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDNICKNAME, vCardIq.nickName());
    pControl->InsertConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDDESCRIPTION, vCardIq.description());
    pControl->InsertConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDGENDER, vCardIq.middleName());
    pControl->InsertConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDEMAIL, vCardIq.email());
    pControl->InsertConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDURL, vCardIq.url());
    pControl->InsertConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDBIRTHDAY, vCardIq.birthday());
    pControl->InsertConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDFULLNAME, vCardIq.fullName());
    QList<QXmppVCardPhone> phoneList = vCardIq.phones();
    if(phoneList.size() > 0)
    {
        pControl->InsertConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDPHONE, phoneList.at(0).number());
    }
    QList<QXmppVCardAddress> addressList = vCardIq.addresses();
    if(addressList.size() > 0)
    {
        pControl->InsertConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDADDRESSES, addressList.at(0).locality());
    }
    QImage image = QImage::fromData(vCardIq.photo());
    if(!image.isNull())
    {
        image.save(FreyaPortrait::GetPortraitPathByBareJid(bareJid));
    }
}

void FreyaXmppClient::SaveVcardToServer()
{
    FreyaBaseControl *pControl = FreyaBaseControl::GetFreyaControl();
    pControl->GetConfigFromFile(FREYA_FUN_PERSONCONFIGPATH(configuration().jidBare()) + FREYA_PTH_PERSONCONFIGPATH);

    QXmppVCardIq vCardIq;
    vCardIq.setNickName(pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDNICKNAME).toString());
    vCardIq.setMiddleName(pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDGENDER).toString());
    vCardIq.setEmail(pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDEMAIL).toString());
    vCardIq.setUrl(pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDURL).toString());
    vCardIq.setBirthday(pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDBIRTHDAY).toDate());
    vCardIq.setFullName(pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDFULLNAME).toString());
    vCardIq.setDescription(pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDDESCRIPTION).toString());
    QXmppVCardPhone vCardPhone;
    vCardPhone.setNumber(pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDPHONE).toString());
    vCardIq.setPhones(QList<QXmppVCardPhone>()<<vCardPhone);
    QXmppVCardAddress vCardAdd;
    vCardAdd.setLocality(pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDADDRESSES).toString());
    vCardIq.setAddresses(QList<QXmppVCardAddress>()<<vCardAdd);

    QImage image(FreyaPortrait::GetPortraitPathByBareJid(configuration().jidBare()));
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::ReadWrite);
    image.save(&buffer, "PNG");
    vCardIq.setPhoto(ba);

    FreyaBaseData *CurData = new FreyaBaseData;
    vCardIq.setId(CurData->dataID);
    CurData->command = FREYA_CMD_VCARDMODREQUEST;
    CurData->arguments = configuration().jidBare();
    FreyaBaseControl::GetFreyaControl()->InsertBaseData(CurData);
    vCardManager().setClientVCard(vCardIq);
}

void FreyaXmppClient::OnClientStateChanged(QXmppClient::State state)
{
    FreyaBaseData ConnectData;
    ConnectData.command = FREYA_CMD_CONNECTSTATE;
    ConnectData.arguments = state;
    FREYA_REQUESTEXECUTION(&ConnectData);
    qDebug()<<"Temp=>"<<"Jid:"<<configuration().jid();
}

void FreyaXmppClient::OnIqReceived(const QXmppIq &iq)
{
    FreyaBaseData *pData = FreyaBaseControl::GetFreyaControl()->TakeBaseData(iq.id());
    if(pData)
    {
        FreyaBaseData CurData(*pData);
        delete pData;
        if(QXmppIq::Result == iq.type())
        {
            if(FREYA_CMD_VCARDMODREQUEST == CurData.command)
            {
                CurData.command = FREYA_CMD_VCARDRESULT;
                FREYA_REQUESTEXECUTION(&CurData);
            }
        }
    }
    qDebug()<<"Temp=>"<<"Freya IQ Received:"<<iq.id();
}

void FreyaXmppClient::OnRosterReceived()
{
    FREYA_REQUESTEXECUTION(FREYA_CMD_ROSTERRECEIVED);
}

void FreyaXmppClient::OnMessageReceived(const QXmppMessage &message)
{
    qDebug()<<"Temp=>"<<"Freya Message Received:"<<message.body();
}

void FreyaXmppClient::OnPresenceReceived(const QXmppPresence &presence)
{
    FreyaBaseData PresenceData;
    PresenceData.command = FREYA_CMD_PRESENCECHANGED;
    PresenceData.arguments = CUSTOMCLSTOVARIANT(&presence);
    FREYA_REQUESTEXECUTION(&PresenceData);
}

void FreyaXmppClient::OnClientError(QXmppClient::Error error)
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

    FreyaBaseData data;
    data.command = FREYA_CMD_TRAYMESSAGE;
    QVariantMap messageMap;
    messageMap.insert(FREYA_INT_TRAYMSGSTYLE, QSystemTrayIcon::Information);
    messageMap.insert(FREYA_STR_TRAYMSGCONTENT, errorString);
    data.arguments = messageMap;
    FREYA_REQUESTEXECUTION(&data);
}

void FreyaXmppClient::OnLoggerChanged(QXmppLogger *logger)
{
    qDebug()<<"Temp=>"<<"Freya Logger Changed:"<<logger;
}

void FreyaXmppClient::OnClientVCardReceived()
{
    SaveVcardToLocal(vCardManager().clientVCard(), configuration().jidBare());
    FreyaBaseData VCardData;
    VCardData.command = FREYA_CMD_VCARDRESULT;
    VCardData.arguments = configuration().jidBare();
    FREYA_REQUESTEXECUTION(&VCardData);
}

void FreyaXmppClient::OnVCardReceived(const QXmppVCardIq &vCardIq)
{
    FreyaBaseData *pvCardData = FreyaBaseControl::GetFreyaControl()->TakeBaseData(vCardIq.id());
    if(pvCardData)
    {
        FreyaBaseData VCardData;
        VCardData.dataID = pvCardData->dataID;
        VCardData.command = FREYA_CMD_VCARDRESULT;
        VCardData.arguments = pvCardData->arguments;
        delete pvCardData;
        SaveVcardToLocal(vCardIq, VCardData.arguments.toString());
        FREYA_REQUESTEXECUTION(&VCardData);
    }
}

void FreyaXmppClient::OnSubscriptionReceived(const QString &bareJid)
{
    FreyaBaseData subData;
    subData.command = FREYA_CMD_SUBSCRIBEREQUEST;
    subData.arguments = bareJid;
    FREYA_REQUESTEXECUTION(&subData);
}
