#include "freyalogindialog.h"
#include "freyaportrait.h"
#include "freyaabstractcontrol.h"
#include "freyaxmppclient.h"
#include "freyacryptogram.h"
#include "freyaregisterdialog.h"
#include "freyastatebtn.h"
#include "freyamessagedialog.h"
#include <QCryptographicHash>
#include <QSystemTrayIcon>

FreyaLoginDialog::FreyaLoginDialog(FreyaWindow *parent) :
    FreyaWindow(parent, FreyaBaseControl::GetFreyaControl(), FREYA_OBJ_LOGINDIALOG)
{
    InitWidget();
    InitData();
}

void FreyaLoginDialog::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
    {
        OnLoginIn();
    }
    else if(e->key() == Qt::Key_Escape)
    {
        qApp->quit();
    }
    FreyaWindow::keyPressEvent(e);
}

void FreyaLoginDialog::closeEvent(QCloseEvent *e)
{
    FreyaXmppClient *pXmppClient = static_cast<FreyaXmppClient*>(FreyaBaseControl::GetFreyaControl()->GetActionObject(FREYA_OBJ_XMPPCLIENTNAME));
    if(!pXmppClient || !pXmppClient->isConnected())
    {
        qApp->quit();
    }
    FreyaWindow::closeEvent(e);
}

void FreyaLoginDialog::Execute()
{
}

void FreyaLoginDialog::Execute(const quint64 &/*command*/)
{
}

void FreyaLoginDialog::Execute(FreyaBaseData *pData)
{
    if(FREYA_CMD_CONNECTSTATE == pData->command)
    {
        if(QXmppClient::ConnectedState == pData->arguments.toInt())
        {
            SaveGlobalConfig();
            FREYA_REQUESTEXECUTION(FREYA_CMD_LOGINEDSTATE);
        }
    }
}

void FreyaLoginDialog::InitWidget()
{
    setWindowTitle(tr("Freya Login"));
    setFixedSize(430, 300);
    SetResizeFrameWidth(0);
    setAttribute(Qt::WA_TranslucentBackground);

    QVBoxLayout *pMainVLay = new QVBoxLayout(this);
    pMainVLay->setSpacing(0);
    pMainVLay->setMargin(0);

    m_LoginPublishLab = new QLabel(this);
    m_LoginPublishLab->setObjectName("LoginPublishLab");
    m_LoginPublishLab->setFixedHeight(150);
    pMainVLay->addWidget(m_LoginPublishLab);

    QVBoxLayout *pPubVLay = new QVBoxLayout(m_LoginPublishLab);
    pPubVLay->setSpacing(0);
    pPubVLay->setMargin(0);

    QHBoxLayout *pCloseHLay = new QHBoxLayout;
    pCloseHLay->setSpacing(0);
    pCloseHLay->setMargin(0);
    pPubVLay->addLayout(pCloseHLay);

    pCloseHLay->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    m_LoginSetBtn = new QToolButton(m_LoginPublishLab);
    m_LoginSetBtn->setObjectName("LoginSetBtn");
    m_LoginSetBtn->setFixedSize(30, 30);
    m_LoginSetBtn->setCheckable(true);
    m_LoginSetBtn->setToolTip(tr("Setting"));
    pCloseHLay->addWidget(m_LoginSetBtn, 0, Qt::AlignTop);
    m_LoginMinBtn = new QToolButton(m_LoginPublishLab);
    m_LoginMinBtn->setObjectName("LoginMinBtn");
    m_LoginMinBtn->setFixedSize(30, 30);
    m_LoginMinBtn->setToolTip(tr("Hide"));
    pCloseHLay->addWidget(m_LoginMinBtn, 0, Qt::AlignTop);
    connect(m_LoginMinBtn, SIGNAL(clicked()), this, SLOT(showMinimized()));
    m_LoginCloseBtn = new QToolButton(m_LoginPublishLab);
    m_LoginCloseBtn->setObjectName("LoginCloseBtn");
    m_LoginCloseBtn->setFixedSize(50, 30);
    m_LoginCloseBtn->setToolTip(tr("Close"));
    pCloseHLay->addWidget(m_LoginCloseBtn, 0, Qt::AlignTop);
    connect(m_LoginCloseBtn, SIGNAL(clicked()), this, SLOT(close()));

    ///
    m_LoginSetWidget = new FreyaBaseWidget(m_LoginPublishLab);
    m_LoginSetWidget->setObjectName("LoginSetWidget");
    m_LoginSetWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pPubVLay->addWidget(m_LoginSetWidget);
    m_LoginSetWidget->hide();
    connect(m_LoginSetBtn, SIGNAL(clicked(bool)), m_LoginSetWidget, SLOT(setVisible(bool)));

    QVBoxLayout *pSetVLay = new QVBoxLayout(m_LoginSetWidget);
    pSetVLay->setSpacing(10);
    pSetVLay->setMargin(20);

    QHBoxLayout *pHostHLay = new QHBoxLayout;
    pHostHLay->setSpacing(10);
    pHostHLay->setMargin(0);
    pSetVLay->addLayout(pHostHLay);

    QLabel *pLoginHostLab = new QLabel(m_LoginSetWidget);
    pLoginHostLab->setObjectName("LoginHostLab");
    pLoginHostLab->setText(tr("Server:"));
    pLoginHostLab->setFixedSize(40, 30);
    pHostHLay->addWidget(pLoginHostLab);
    m_LoginHostLEdit = new QLineEdit(m_LoginSetWidget);
    m_LoginHostLEdit->setObjectName("LoginHostLEdit");
    m_LoginHostLEdit->setFixedHeight(pLoginHostLab->height());
    m_LoginHostLEdit->setContextMenuPolicy(Qt::NoContextMenu);
    pHostHLay->addWidget(m_LoginHostLEdit);

    QLabel *pLoginPortLab = new QLabel(m_LoginSetWidget);
    pLoginPortLab->setObjectName("LoginPortLab");
    pLoginPortLab->setText(tr("Port:"));
    pLoginPortLab->setFixedSize(pLoginHostLab->size());
    pHostHLay->addWidget(pLoginPortLab);
    m_LoginPortLEdit = new QLineEdit(m_LoginSetWidget);
    m_LoginPortLEdit->setObjectName("LoginPortLEdit");
    m_LoginPortLEdit->setFixedHeight(pLoginHostLab->height());
    m_LoginPortLEdit->setFixedWidth(80);
    m_LoginPortLEdit->setContextMenuPolicy(Qt::NoContextMenu);
    pHostHLay->addWidget(m_LoginPortLEdit);

    QHBoxLayout *pDomainHLay = new QHBoxLayout;
    pDomainHLay->setSpacing(10);
    pDomainHLay->setMargin(0);
    pSetVLay->addLayout(pDomainHLay);

    QLabel *pLoginDomainLab = new QLabel(m_LoginSetWidget);
    pLoginDomainLab->setObjectName("LoginDomainLab");
    pLoginDomainLab->setText(tr("Domain:"));
    pLoginDomainLab->setFixedSize(pLoginHostLab->size());
    pDomainHLay->addWidget(pLoginDomainLab);
    m_LoginDomainLEdit = new QLineEdit(m_LoginSetWidget);
    m_LoginDomainLEdit->setObjectName("LoginDomainLEdit");
    m_LoginDomainLEdit->setFixedHeight(pLoginHostLab->height());
    m_LoginDomainLEdit->setContextMenuPolicy(Qt::NoContextMenu);
    pDomainHLay->addWidget(m_LoginDomainLEdit);

    QLabel *pLoginResourceLab = new QLabel(m_LoginSetWidget);
    pLoginResourceLab->setObjectName("LoginResourceLab");
    pLoginResourceLab->setText(tr("Res:"));
    pLoginResourceLab->setFixedSize(pLoginHostLab->size());
    pDomainHLay->addWidget(pLoginResourceLab);
    m_LoginResourceLEdit = new QLineEdit(m_LoginSetWidget);
    m_LoginResourceLEdit->setObjectName("LoginResourceLEdit");
    m_LoginResourceLEdit->setFixedHeight(pLoginHostLab->height());
    m_LoginResourceLEdit->setContextMenuPolicy(Qt::NoContextMenu);
    pDomainHLay->addWidget(m_LoginResourceLEdit);

    ///
    m_LoginWidget = new FreyaBaseWidget(this);
    m_LoginWidget->setObjectName("LoginWidget");
    pMainVLay->addWidget(m_LoginWidget);

    QHBoxLayout *pLoginHLay = new QHBoxLayout(m_LoginWidget);
    pLoginHLay->setSpacing(10);
    pLoginHLay->setContentsMargins(20, 25, 20, 25);

    m_LoginPortrait = new FreyaPortrait(m_LoginWidget);
    m_LoginPortrait->setFixedSize(100, 100);
    m_LoginPortrait->SetDefault(FREYA_PTH_GLOBALRESPATH FREYA_PTH_DEFPORTRAITPATH);
    pLoginHLay->addWidget(m_LoginPortrait);

    QVBoxLayout *pAccountVLay = new QVBoxLayout;
    pAccountVLay->setSpacing(0);
    pAccountVLay->setMargin(0);
    pLoginHLay->addLayout(pAccountVLay);

    m_LoginAccountCombo = new QComboBox(m_LoginWidget);
    m_LoginAccountCombo->setObjectName("LoginAccountCombo");
    m_LoginAccountCombo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_LoginAccountCombo->setEditable(true);
    m_LoginAccountCombo->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9_]*", Qt::CaseInsensitive), m_LoginAccountCombo));
    m_LoginAccountCombo->setContextMenuPolicy(Qt::NoContextMenu);
    pAccountVLay->addWidget(m_LoginAccountCombo);
    connect(m_LoginAccountCombo, SIGNAL(currentTextChanged(QString)), this, SLOT(OnAccoundComboChanged(QString)));

    m_LoginPasswordLEdit = new QLineEdit(m_LoginWidget);
    m_LoginPasswordLEdit->setObjectName("LoginPasswordLEdit");
    m_LoginPasswordLEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_LoginPasswordLEdit->setEchoMode(QLineEdit::Password);
    m_LoginPasswordLEdit->setContextMenuPolicy(Qt::NoContextMenu);
    pAccountVLay->addWidget(m_LoginPasswordLEdit);

    QHBoxLayout *pKeepHLay = new QHBoxLayout;
    pKeepHLay->setSpacing(0);
    pKeepHLay->setMargin(0);
    pAccountVLay->addLayout(pKeepHLay);

    m_LoginKeepCheck = new QCheckBox(m_LoginWidget);
    m_LoginKeepCheck->setObjectName("LoginKeepCheck");
    m_LoginKeepCheck->setFixedHeight(30);
    m_LoginKeepCheck->setText(tr("Remember"));
    pKeepHLay->addWidget(m_LoginKeepCheck);

    m_LoginStateBtn = new FreyaStateBtn(m_LoginWidget);
    m_LoginStateBtn->setFixedHeight(24);
    pKeepHLay->addWidget(m_LoginStateBtn);

    QVBoxLayout *pRegisterVLay = new QVBoxLayout;
    pRegisterVLay->setSpacing(0);
    pRegisterVLay->setMargin(0);
    pLoginHLay->addLayout(pRegisterVLay);

    m_LoginRegisterBtn = new QToolButton(m_LoginWidget);
    m_LoginRegisterBtn->setObjectName("LoginRegisterBtn");
    m_LoginRegisterBtn->setText(tr("Register ID"));
    pRegisterVLay->addWidget(m_LoginRegisterBtn);
    connect(m_LoginRegisterBtn, SIGNAL(clicked()), this, SLOT(OnRegisterAccound()));

    m_LoginFindPSWBtn = new QToolButton(m_LoginWidget);
    m_LoginFindPSWBtn->setObjectName("LoginFindPSWBtn");
    m_LoginFindPSWBtn->setText(tr("Forget PSW"));
    pRegisterVLay->addWidget(m_LoginFindPSWBtn);

    m_LoginInBtn = new QToolButton(m_LoginWidget);
    m_LoginInBtn->setObjectName("LoginInBtn");
    m_LoginInBtn->setFixedSize(100, 40);
    m_LoginInBtn->setText(tr("Login"));
    pRegisterVLay->addWidget(m_LoginInBtn);
    connect(m_LoginInBtn, SIGNAL(clicked()), this, SLOT(OnLoginIn()));

    ///
    m_LoginTipsLab = new QLabel(this);
    m_LoginTipsLab->setObjectName("LoginTipsLab");
    m_LoginTipsLab->setAlignment(Qt::AlignCenter);
    m_LoginTipsLab->hide();
    m_LoginMovie = new QMovie(m_LoginTipsLab);
    m_LoginMovie->setFileName(FREYA_PTH_LOGINGIFPATH);
    m_LoginTipsLab->setMovie(m_LoginMovie);

    QVBoxLayout *pLoginTipsVLay = new QVBoxLayout(m_LoginTipsLab);
    pLoginTipsVLay->setSpacing(0);
    pLoginTipsVLay->setMargin(20);

    m_LoginInCancelBtn = new QToolButton(m_LoginTipsLab);
    m_LoginInCancelBtn->setFixedSize(160, 40);
    m_LoginInCancelBtn->setText(tr("Cancel"));
    pLoginTipsVLay->addWidget(m_LoginInCancelBtn, 0, Qt::AlignHCenter | Qt::AlignBottom);
    connect(m_LoginInCancelBtn, SIGNAL(clicked()), this, SLOT(OnLoginInCancel()));
}

void FreyaLoginDialog::InitData()
{
    FreyaBaseControl *pControl = FreyaBaseControl::GetFreyaControl();
    pControl->GetConfigFromFile(FREYA_PTH_GLOBALCONFIGPATH);

    m_LoginHostLEdit->setText(pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDDEFAULT<<FREYA_CFG_CONFIDSERVER).toString());
    m_LoginPortLEdit->setText(pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDDEFAULT<<FREYA_CFG_CONFIDPORT).toString());
    m_LoginDomainLEdit->setText(pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDDEFAULT<<FREYA_CFG_CONFIDDOMAIN).toString());
    m_LoginResourceLEdit->setText(pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDDEFAULT<<FREYA_CFG_CONFIDRESOURCE).toString());
    m_LoginResourceLEdit->setEnabled(m_LoginResourceLEdit->text().isEmpty());

    QVariantMap StateMap = pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDDEFAULT<<FREYA_CFG_CONFIDSTATES).toMap();
    m_LoginStateBtn->LoadStates(StateMap);
    m_LoginAccountCombo->addItems(pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDDEFAULT<<FREYA_CFG_CONFIDUSERS).toStringList());
}

void FreyaLoginDialog::SaveGlobalConfig()
{
    FreyaBaseControl *pControl = FreyaBaseControl::GetFreyaControl();
    pControl->GetConfigFromFile(FREYA_PTH_GLOBALCONFIGPATH);
    if(m_LoginHostLEdit->text() != pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDDEFAULT<<FREYA_CFG_CONFIDSERVER).toString())
    {
        pControl->SetConfig(QStringList()<<FREYA_CFG_CONFIDDEFAULT<<FREYA_CFG_CONFIDSERVER, m_LoginHostLEdit->text());
    }
    if(m_LoginPortLEdit->text() != pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDDEFAULT<<FREYA_CFG_CONFIDPORT).toString())
    {
        pControl->SetConfig(QStringList()<<FREYA_CFG_CONFIDDEFAULT<<FREYA_CFG_CONFIDPORT, m_LoginPortLEdit->text());
    }
    if(m_LoginDomainLEdit->text() != pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDDEFAULT<<FREYA_CFG_CONFIDDOMAIN).toString())
    {
        pControl->SetConfig(QStringList()<<FREYA_CFG_CONFIDDEFAULT<<FREYA_CFG_CONFIDDOMAIN, m_LoginDomainLEdit->text());
    }

    QStringList AccountList = pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDDEFAULT<<FREYA_CFG_CONFIDUSERS).toStringList();
    AccountList.insert(0, m_LoginAccountCombo->currentText());
    AccountList.removeDuplicates();
    pControl->SetConfig(QStringList()<<FREYA_CFG_CONFIDDEFAULT<<FREYA_CFG_CONFIDUSERS, AccountList);

    pControl->GetConfigFromFile(FREYA_FUN_PERSONCONFIGPATH(m_LoginAccountCombo->currentText() + "@" + m_LoginDomainLEdit->text()) + FREYA_PTH_PERSONCONFIGPATH);

    QString PasswordString = m_LoginKeepCheck->isChecked() ? FreyaCryptogram::EnCryptogram(m_LoginPasswordLEdit->text()): "";
    pControl->InsertConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_CONFIDPASSWORD, PasswordString);
    pControl->InsertConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_CONFIDSTATE, m_LoginStateBtn->GetState());//m_LoginStateCombo->currentData());
}

bool FreyaLoginDialog::CheckLoginInfor()
{
    FreyaBaseData data;
    data.command = FREYA_CMD_TRAYMESSAGE;
    QVariantMap messageMap;
    messageMap.insert(FREYA_INT_TRAYMSGSTYLE, QSystemTrayIcon::Critical);
    if(m_LoginAccountCombo->currentText().isEmpty())
    {
        messageMap.insert(FREYA_STR_TRAYMSGCONTENT, tr("Account cannot be empty!"));
    }
    else if(m_LoginPasswordLEdit->text().isEmpty())
    {
        messageMap.insert(FREYA_STR_TRAYMSGCONTENT, tr("Password cannot be empty!"));
    }
    else if(m_LoginHostLEdit->text().isEmpty())
    {
        messageMap.insert(FREYA_STR_TRAYMSGCONTENT, tr("Server cannot be empty!"));
    }
    else if(m_LoginPortLEdit->text().isEmpty())
    {
        messageMap.insert(FREYA_STR_TRAYMSGCONTENT, tr("Port cannot be empty!"));
    }
    else if(m_LoginDomainLEdit->text().isEmpty())
    {
        messageMap.insert(FREYA_STR_TRAYMSGCONTENT, tr("Domain cannot be empty!"));
    }
    else if(m_LoginResourceLEdit->text().isEmpty())
    {
        messageMap.insert(FREYA_STR_TRAYMSGCONTENT, tr("Resource cannot be empty!"));
    }
    else
    {
        return true;
    }
    data.arguments = messageMap;
    FREYA_REQUESTEXECUTION(&data);
    return false;
}

void FreyaLoginDialog::OnLoginIn()
{
    if(!CheckLoginInfor())
    {
        return;
    }
    m_LoginTipsLab->setGeometry(rect());
    m_LoginTipsLab->show();
    m_LoginMovie->start();

    QXmppConfiguration configuration;
    configuration.setHost(m_LoginHostLEdit->text());
    configuration.setPort(m_LoginPortLEdit->text().toInt());
    configuration.setDomain(m_LoginDomainLEdit->text());
    configuration.setResource(m_LoginResourceLEdit->text());
    configuration.setJid(m_LoginAccountCombo->currentText() + "@" + m_LoginDomainLEdit->text());
    configuration.setPassword(QCryptographicHash::hash(m_LoginPasswordLEdit->text().toUtf8(),
                                                       QCryptographicHash::Md5).toHex());
    FreyaBaseData loginData;
    loginData.command = FREYA_CMD_CLIENTLOGIN;
    QVariantMap varMap;
    varMap.insert(FREYA_CLS_XMPPCONFIGURATION, CUSTOMCLSTOVARIANT(&configuration));
    varMap.insert(FREYA_CLS_XMPPPRESENCESTATUS, m_LoginStateBtn->GetState());
    loginData.arguments = varMap;
    FREYA_REQUESTEXECUTION(&loginData);
}

void FreyaLoginDialog::OnLoginInCancel()
{
    m_LoginTipsLab->hide();
    m_LoginMovie->stop();
    FREYA_REQUESTEXECUTION(FREYA_CMD_LOGOUTEDSTATE);
}

void FreyaLoginDialog::OnAccoundComboChanged(const QString &account)
{
    FreyaBaseControl *pControl = FreyaBaseControl::GetFreyaControl();
    pControl->GetConfigFromFile(FREYA_FUN_PERSONCONFIGPATH(account + "@" + m_LoginDomainLEdit->text()) + FREYA_PTH_PERSONCONFIGPATH);

    m_LoginPortrait->SetPortraitByBareJid(account + "@" + m_LoginDomainLEdit->text());

    QString PswString = pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_CONFIDPASSWORD).toString();
    int state = pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_CONFIDSTATE).toInt();

    m_LoginPasswordLEdit->setText(FreyaCryptogram::DeCryptogram(PswString));
    m_LoginKeepCheck->setChecked(!m_LoginPasswordLEdit->text().isEmpty());
    m_LoginStateBtn->SetState(state);
}

void FreyaLoginDialog::OnRegisterAccound()
{
    FreyaRegisterDialog freyaRegister;
    freyaRegister.exec();
}
