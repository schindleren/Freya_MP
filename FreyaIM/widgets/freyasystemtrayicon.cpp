#include "freyaim_global.h"
#include "freyasystemtrayicon.h"
#include "freyabasecontrol.h"
#include "freyaxmppclient.h"
#include "overicon.h"

FreyaSystemTrayIcon::FreyaSystemTrayIcon(QObject *parent) :
    QSystemTrayIcon(parent), FreyaBaseAction(FreyaBaseControl::GetFreyaControl(), FREYA_OBJ_SYSTRAYICON)
{
    connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            SLOT(OnTrayActivated(QSystemTrayIcon::ActivationReason)));
    LoadTrayIcon(true);
    setToolTip(tr("FreyaIM"));
    InitBaseMenu();
}

FreyaSystemTrayIcon::~FreyaSystemTrayIcon()
{
    delete m_TrayMenu;
}

void FreyaSystemTrayIcon::InitBaseMenu()
{
    m_TrayMenu = new QMenu;
    m_TrayMenu->setObjectName("TrayMenu");
    m_TrayMenu->addAction(tr("Quit"), qApp, SLOT(quit()));

    setContextMenu(m_TrayMenu);
}

void FreyaSystemTrayIcon::LoadTrayIcon(bool isGray)
{
    if(isGray)
    {
        setIcon(QIcon(QPixmap::fromImage(OverIcon::Decoloration(QImage(FREUA_PTH_IMICON)))));
    }
    else
    {
        setIcon(QIcon(FREUA_PTH_IMICON));
    }
}

void FreyaSystemTrayIcon::Execute()
{
}

void FreyaSystemTrayIcon::Execute(const quint64 &/*command*/)
{
}

void FreyaSystemTrayIcon::Execute(FreyaBaseData *pData)
{
    if(FREYA_CMD_TRAYMESSAGE == pData->command)
    {
        QVariantMap MessageMap = pData->arguments.toMap();
        showMessage(tr("Freya IM Message"), MessageMap.value(FREYA_STR_TRAYMSGCONTENT).toString(),
                    (QSystemTrayIcon::MessageIcon)MessageMap.value(FREYA_INT_TRAYMSGSTYLE).toInt());
    }
    else if(FREYA_CMD_CONNECTSTATE == pData->command)
    {
        switch (pData->arguments.toInt())
        {
        case QXmppClient::ConnectedState:
        {
            FreyaXmppClient *pClient = static_cast<FreyaXmppClient*>(FreyaBaseControl::GetFreyaControl()->GetActionObject(FREYA_OBJ_XMPPCLIENTNAME));
            if(pClient)
            {
                setToolTip(tr("FreyaIM:") + pClient->configuration().user());
                LoadTrayIcon(false);
            }
        }
            break;
        case QXmppClient::DisconnectedState:
        {
            setToolTip(tr("FreyaIM:Not logged in"));
            LoadTrayIcon(true);
        }
            break;
        default:
            break;
        }
    }
}

void FreyaSystemTrayIcon::OnTrayActivated(const QSystemTrayIcon::ActivationReason &re)
{
    FreyaBaseData baseData;
    baseData.command = FREYA_CMD_TRAYACTIVE;
    baseData.arguments = re;
    FREYA_REQUESTEXECUTION(&baseData);
}
