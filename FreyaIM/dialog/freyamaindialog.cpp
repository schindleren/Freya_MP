#include "freyaim_global.h"
#include "freyamaindialog.h"
#include "freyaportrait.h"
#include "freyacontacttree.h"
#include "freyastatebtn.h"
#include "freyaxmppclient.h"

FreyaMainDialog::FreyaMainDialog(FreyaWindow *parent) :
    FreyaWindow(parent, FreyaBaseControl::GetFreyaControl(), FREYA_OBJ_MAINDIALOG)
{
    InitWidget();
    FREYA_REQUESTEXECUTION(FREYA_CMD_CONFIGNREQUEST);
}

void FreyaMainDialog::Execute()
{
}

void FreyaMainDialog::Execute(const quint64 &command)
{
    if(FREYA_CMD_ROSTERRECEIVED == command)
    {
        LoadRoster();
    }
}

void FreyaMainDialog::Execute(FreyaBaseData *pData)
{
    if(FREYA_CMD_TRAYACTIVE == pData->command)
    {
        if(QSystemTrayIcon::Trigger ==  pData->arguments.toInt())
        {
            FreyaWindow::PromptAnyWindow(this);
        }
    }
    else if(FREYA_CMD_VCARDRESULT == pData->command)
    {
        if(pData->arguments.toString() == m_BareJid)
        {
            LoadLocalConfig();
        }
    }
    else if(FREYA_CMD_CONFIGNRESULT == pData->command)
    {
        if(m_BareJid.isEmpty())
        {
            QXmppConfiguration *xmppConfig = VARIANTTOCUSTOMCLS(pData->arguments, QXmppConfiguration);
            if(xmppConfig)
            {
                InitConfig(xmppConfig);
            }
        }
    }
    else if(FREYA_CMD_PRESENCECHANGED == pData->command)
    {
        QXmppPresence *pPresence = VARIANTTOCUSTOMCLS(pData->arguments, QXmppPresence);
        if(pPresence)
        {
            PresenceMessage(*pPresence);
        }
    }
}

void FreyaMainDialog::resizeEvent(QResizeEvent *e)
{
    FreyaBaseControl *pControl = FreyaBaseControl::GetFreyaControl();
    if(!m_BareJid.isEmpty())
    {
        pControl->GetConfigFromFile(FREYA_FUN_PERSONCONFIGPATH(m_BareJid) + FREYA_PTH_PERSONCONFIGPATH);
    }
    pControl->InsertConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_CONFIDGEOMETRY<<FREYA_CFG_CONFIDGEOMETRY_W, width());
    pControl->InsertConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_CONFIDGEOMETRY<<FREYA_CFG_CONFIDGEOMETRY_H, height());
    FreyaWindow::resizeEvent(e);
}

void FreyaMainDialog::moveEvent(QMoveEvent *e)
{
    FreyaBaseControl *pControl = FreyaBaseControl::GetFreyaControl();
    if(!m_BareJid.isEmpty())
    {
        pControl->GetConfigFromFile(FREYA_FUN_PERSONCONFIGPATH(m_BareJid) + FREYA_PTH_PERSONCONFIGPATH);
    }
    pControl->InsertConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_CONFIDGEOMETRY<<FREYA_CFG_CONFIDGEOMETRY_X, x());
    pControl->InsertConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_CONFIDGEOMETRY<<FREYA_CFG_CONFIDGEOMETRY_Y, y());
    FreyaWindow::moveEvent(e);
}

void FreyaMainDialog::InitWidget()
{
    setWindowFlags(windowFlags() | Qt::Tool);
    setMinimumSize(FREYA_INF_DEFCONFMAINRECT.size());
    setGeometry(FREYA_INF_DEFCONFMAINRECT);
    SetShowMaxAble(false);

    QHBoxLayout *pBackHLay = new QHBoxLayout(this);
    pBackHLay->setSpacing(0);
    pBackHLay->setMargin(0);
    m_BackgroundWidget = new QWidget(this);
    m_BackgroundWidget->setObjectName("MainDiaBKWidget");
    m_BackgroundWidget->setMouseTracking(true);
    pBackHLay->addWidget(m_BackgroundWidget);
    QVBoxLayout *pMainVLay = new QVBoxLayout(m_BackgroundWidget);
    pMainVLay->setSpacing(0);
    pMainVLay->setMargin(0);

    m_TitleLab = new QLabel(m_BackgroundWidget);
    m_TitleLab->setObjectName("MainDiaTitleLab");
    m_TitleLab->setFixedHeight(26);
    m_TitleLab->setMouseTracking(true);
    m_TitleLab->setText(tr("Freya IM"));
    pMainVLay->addWidget(m_TitleLab);
    QHBoxLayout *pTitleHLay = new QHBoxLayout(m_TitleLab);
    pTitleHLay->setSpacing(0);
    pTitleHLay->setMargin(0);
    pTitleHLay->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    m_MinBtn = new QToolButton(m_TitleLab);
    m_MinBtn->setObjectName("MainDiaMinBtn");
    m_MinBtn->setMouseTracking(true);
    m_MinBtn->setFixedSize(m_TitleLab->height(), m_TitleLab->height());
    pTitleHLay->addWidget(m_MinBtn);
    connect(m_MinBtn, SIGNAL(clicked()), this, SLOT(hide()));
    m_CloseBtn = new QToolButton(m_TitleLab);
    m_CloseBtn->setObjectName("MainDiaCloseBtn");
    m_CloseBtn->setMouseTracking(true);
    m_CloseBtn->setFixedSize(m_TitleLab->height(), m_TitleLab->height());
    pTitleHLay->addWidget(m_CloseBtn);
    connect(m_CloseBtn, SIGNAL(clicked()), this, SLOT(hide()));
    connect(m_CloseBtn, SIGNAL(clicked()), qApp, SLOT(quit()));

    QHBoxLayout *pPortraitHLay = new QHBoxLayout;
    pPortraitHLay->setSpacing(0);
    pPortraitHLay->setMargin(0);
    pMainVLay->addLayout(pPortraitHLay);
    m_MainPortrait = new FreyaPortrait(m_BackgroundWidget);
    m_MainPortrait->setMouseTracking(true);
    m_MainPortrait->setFixedSize(60, 60);
    m_MainPortrait->SetDefault(FREYA_PTH_GLOBALRESPATH FREYA_PTH_DEFPORTRAITPATH);
    pPortraitHLay->addWidget(m_MainPortrait);
    connect(m_MainPortrait, SIGNAL(clicked()), this, SLOT(OnShowIDCard()));

    QVBoxLayout *pNameVLay = new QVBoxLayout;
    pNameVLay->setSpacing(0);
    pNameVLay->setMargin(0);
    pPortraitHLay->addLayout(pNameVLay);
    QHBoxLayout *pStateHLay = new QHBoxLayout;
    pStateHLay->setSpacing(0);
    pStateHLay->setMargin(0);
    pNameVLay->addLayout(pStateHLay);

    m_NickNameLab = new QLabel(m_BackgroundWidget);
    m_NickNameLab->setObjectName("MainDiaNameLab");
    m_NickNameLab->setMouseTracking(true);
    m_NickNameLab->setFixedHeight(30);
    pStateHLay->addWidget(m_NickNameLab);
    m_ChangeStateBtn = new FreyaStateBtn(m_BackgroundWidget);
    m_ChangeStateBtn->setMouseTracking(true);
    m_ChangeStateBtn->setFixedSize(m_NickNameLab->height()*2, m_NickNameLab->height());
    pStateHLay->addWidget(m_ChangeStateBtn);
    FreyaBaseControl::GetFreyaControl()->GetConfigFromFile(FREYA_PTH_GLOBALCONFIGPATH);
    QVariantMap StateMap = FreyaBaseControl::GetFreyaControl()->GetConfig(QStringList()<<FREYA_CFG_CONFIDDEFAULT<<FREYA_CFG_CONFIDSTATES).toMap();
    m_ChangeStateBtn->LoadStates(StateMap);
    connect(m_ChangeStateBtn, SIGNAL(ToStateChanged(int)), this, SLOT(OnCliendStateChanged(int)));

    m_DescriptionLEdit = new QLineEdit(m_BackgroundWidget);
    m_DescriptionLEdit->setObjectName("MainDiaDesLEdit");
    m_DescriptionLEdit->setMouseTracking(true);
    m_DescriptionLEdit->setFixedHeight(26);
    pNameVLay->addWidget(m_DescriptionLEdit);

    m_ContactSearchLEdit = new QLineEdit(m_BackgroundWidget);
    m_ContactSearchLEdit->setObjectName("MainDiaSearchLEdit");
    m_ContactSearchLEdit->setMouseTracking(true);
    m_ContactSearchLEdit->setFixedHeight(30);
    pMainVLay->addWidget(m_ContactSearchLEdit);

    m_ContactTWidget = new RTabWidget(m_BackgroundWidget, true);
    m_ContactTWidget->setObjectName("MainDiaContactTWidget");
    m_ContactTWidget->SetTabBKHeight(40);
    m_ContactTWidget->setMouseTracking(true);
    pMainVLay->addWidget(m_ContactTWidget);

    m_ContactTree = new FreyaContactTree(m_ContactTWidget);
    m_ContactTWidget->AddTabAndWidget(m_ContactTree, "MainContactTree", tr("Contact"));
    m_ContactTWidget->OnCurrentTab(0);

    QHBoxLayout *pMainMenuHLay = new QHBoxLayout;
    pMainMenuHLay->setSpacing(0);
    pMainMenuHLay->setMargin(0);
    pMainVLay->addLayout(pMainMenuHLay);

    m_MainMenuBtn = new QToolButton(m_TitleLab);
    m_MainMenuBtn->setObjectName("MainDiaMenuBtn");
    m_MainMenuBtn->setMouseTracking(true);
    m_MainMenuBtn->setFixedSize(30, 30);
    pMainMenuHLay->addWidget(m_MainMenuBtn);

    pMainMenuHLay->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
}

void FreyaMainDialog::InitConfig(QXmppConfiguration *xmppConfig)
{
    FreyaBaseControl *pControl = FreyaBaseControl::GetFreyaControl();
    m_BareJid = xmppConfig->jidBare();
    pControl->GetConfigFromFile(FREYA_FUN_PERSONCONFIGPATH(m_BareJid) + FREYA_PTH_PERSONCONFIGPATH);
    QVariant varX = pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_CONFIDGEOMETRY<<FREYA_CFG_CONFIDGEOMETRY_X);
    if(varX.isValid())
    {
        move(varX.toInt(), y());
        move(x(), pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_CONFIDGEOMETRY<<FREYA_CFG_CONFIDGEOMETRY_Y).toInt());
        resize(pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_CONFIDGEOMETRY<<FREYA_CFG_CONFIDGEOMETRY_W).toInt(), height());
        resize(width(), pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_CONFIDGEOMETRY<<FREYA_CFG_CONFIDGEOMETRY_H).toInt());
    }
    m_ChangeStateBtn->SetState(pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_CONFIDSTATE).toInt());

    LoadLocalConfig();
    FREYA_REQUESTEXECUTION(FREYA_CMD_VCARDREQUEST);
}

void FreyaMainDialog::LoadLocalConfig()
{
    FreyaBaseControl *pControl = FreyaBaseControl::GetFreyaControl();
    pControl->GetConfigFromFile(FREYA_FUN_PERSONCONFIGPATH(m_BareJid) + FREYA_PTH_PERSONCONFIGPATH);
    QString NickName = pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDNICKNAME).toString();
    m_NickNameLab->setText(NickName.isEmpty() ? m_BareJid.split("@").at(0): NickName);
    m_DescriptionLEdit->setText(pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDDESCRIPTION).toString());
    m_MainPortrait->SetPortraitByBareJid(m_BareJid);
}

void FreyaMainDialog::LoadRoster()
{
    FreyaXmppClient *pClient = static_cast<FreyaXmppClient*>(FreyaBaseControl::GetFreyaControl()->GetActionObject(FREYA_OBJ_XMPPCLIENTNAME));
    if(pClient)
    {
        QStringList RosterList = pClient->rosterManager().getRosterBareJids();
        foreach(const QString &bareJid, RosterList)
        {
            m_ContactTree->InsertChild(pClient->rosterManager().getRosterEntry(bareJid));
        }
    }
}

void FreyaMainDialog::PresenceMessage(const QXmppPresence &Presence)
{
    qDebug()<<"Temp=>"<<"FreyaMainDialog::PresenceMessage"<<Presence.availableStatusType()<<Presence.from()<<Presence.to();
}

void FreyaMainDialog::OnShowIDCard()
{
    FreyaBaseData IDCardRequest;
    IDCardRequest.command = FREYA_CMD_SHOWIDCARD;
    IDCardRequest.arguments = m_BareJid;
    FREYA_REQUESTEXECUTION(&IDCardRequest);
}

void FreyaMainDialog::OnCliendStateChanged(int state)
{
    FreyaBaseData PresenceData;
    PresenceData.command = FREYA_CMD_MODIFYPRESENCE;
    QXmppPresence presence;
    presence.setType(QXmppPresence::Available);
    presence.setAvailableStatusType((QXmppPresence::AvailableStatusType)state);
    PresenceData.arguments = CUSTOMCLSTOVARIANT(&presence);
    FREYA_REQUESTEXECUTION(&PresenceData);
}
