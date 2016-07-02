#include "freyaim.h"
#include "freyalogindialog.h"
#include "freyaim_global.h"
#include "freyaxmppclient.h"
#include "freyadefaultclient.h"
#include "freyamaindialog.h"
#include "freyasystemtrayicon.h"
#include "freyaidcarddialog.h"
#include "freyamessagedialog.h"

FreyaIM::FreyaIM(QObject *parent)
    : QObject(parent), FreyaBaseAction(FreyaBaseControl::GetFreyaControl(), FREYA_OBJ_FREYAIM)
{
    m_FreyaMainDialog = NULL;
}

FreyaIM::~FreyaIM()
{
    if(m_FreyaMainDialog)
    {
        delete m_FreyaMainDialog;
    }
}

void FreyaIM::InitApplication()
{
    LoadStyle(FREYA_PTH_DEFAULTSTYLEPATH);
    InitObject();
    InitWidget();
}

void FreyaIM::Execute(const quint64 &command)
{
    if(FREYA_CMD_LOGINEDSTATE == command)
    {
        delete m_FreyaLoginDialog;
        m_FreyaLoginDialog = NULL;
        m_FreyaMainDialog = new FreyaMainDialog();
        m_FreyaMainDialog->show();
    }
}

void FreyaIM::Execute(FreyaBaseData *pData)
{
    if(FREYA_CMD_SHOWIDCARD == pData->command)
    {
        FreyaIDCardDialog *pIDCard = static_cast<FreyaIDCardDialog*>(FreyaBaseControl::GetFreyaControl()->GetActionObject(FREYA_OBJ_IDCARDDIALOG + pData->arguments.toString()));
        if(!pIDCard)
        {
            pIDCard = new FreyaIDCardDialog(pData->arguments.toString(), m_FreyaMainDialog);
        }
        FreyaWindow::PromptAnyWindow(pIDCard);
    }
    else if(FREYA_CMD_SUBSCRIBEREQUEST == pData->command)
    {
        QString bareJid = pData->arguments.toString();
        FreyaMessageDialog *MsgDialog = new FreyaMessageDialog(FREYA_OBJ_MESSAGEDIALOG, 0);
        MsgDialog->DefinitionContent(bareJid + tr(" request to subscribe to your presence."));
        FreyaBaseData DecisionData;
        DecisionData.command = FREYA_CMD_SUBSCRIBERESULT;
        QVariantMap DecisionMap;
        DecisionMap.insert(FREYA_STR_XMPPBAREJID, bareJid);
        DecisionMap.insert(FREYA_CLS_MESSAGEBOXDECISION, true);
        DecisionData.arguments = DecisionMap;
        MsgDialog->AppendDecisionBtn(tr("Accept"), DecisionData);
        DecisionMap.insert(FREYA_CLS_MESSAGEBOXDECISION, false);
        DecisionData.arguments = DecisionMap;
        MsgDialog->AppendDecisionBtn(tr("Reject"), DecisionData);
        FreyaBaseData IDCardRequest;
        IDCardRequest.command = FREYA_CMD_SHOWIDCARD;
        IDCardRequest.arguments = bareJid;
        MsgDialog->AppendMoreBtn(tr("IDCard"), IDCardRequest);
        MsgDialog->show();
    }
}

void FreyaIM::InitObject()
{
    FreyaConfigData nFreyaConfigData;
    nFreyaConfigData.InitConfig();
    m_FreyaIMClient = new FreyaXmppClient(this);
    m_FreyaDefaultClient = new FreyaDefaultClient(this);

    m_Plugin = new FreyaBasePlugin("{263ba86a-6433-490b-a560-ef4ef447aa29}",
                                   FreyaBaseControl::GetFreyaControl(), FREYA_OBJ_FREYAPLUGIN);
    connect(m_Plugin, SIGNAL(ToPluginConnected(bool)), this, SLOT(OnPluginConnected(bool)));
}

void FreyaIM::InitWidget()
{
    m_FreyaLoginDialog = new FreyaLoginDialog;
    m_FreyaLoginDialog->SetObjectName("FreyaLoginDialog");
    m_FreyaLoginDialog->show();
    m_FreyaSystemTrayIcon = new FreyaSystemTrayIcon(this);
    m_FreyaSystemTrayIcon->show();
}

void FreyaIM::LoadStyle(const QString &filepath)
{
    QFile file(filepath);
    if(file.open(QFile::ReadOnly | QFile::Text))
    {
        qApp->setStyleSheet(file.readAll());
        file.close();
    }
}

void FreyaIM::OnPluginConnected(bool isConnected)
{
    if(isConnected)
    {
        m_Plugin->ImportPluginAuth(QStringList()
                                <<"{b1db3ef5-17cf-413e-ab71-48b9a0b46820}"
                                <<"{1d4397a2-dbcb-4ea5-a479-38d271d0962f}"
                                <<"{b9e2fc0d-d67e-41b7-b1bc-6a224fc646c7}"
                                <<"{e17639f9-ed6d-4e5c-9171-99eb732562ec}",
                                QStringList()
                                <<"{7398ae7c-1389-4758-a3d1-606ec002508a}"
                                <<"{b783c1a2-715d-489a-b457-14677b7bd6a0}"
                                <<"{c9a18fc4-a1a5-445e-9762-611f7131b0c2}"
                                <<"{34ab3393-84ef-4659-87ee-4ea4c38d516b}"
                                );
    }
}
