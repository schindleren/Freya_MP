#include "freyams_global.h"
#include "freyamsloginwidget.h"
#include "QXmppConfiguration.h"
#include "QXmppClient.h"

FreyaMSLoginWidget::FreyaMSLoginWidget(QWidget *parent) :
    QWidget(parent), FreyaBaseAction(FreyaBaseControl::GetFreyaControl(), FREYAMS_OBJ_LOGINWIDGET)
{
    InitWidget();
}

void FreyaMSLoginWidget::Execute(const quint64 &/*command*/)
{
}

void FreyaMSLoginWidget::Execute(FreyaBaseData */*pData*/)
{
}

void FreyaMSLoginWidget::keyPressEvent(QKeyEvent *e)
{
    if(Qt::Key_Return == e->key())
    {
        OnLogin();
    }
    QWidget::keyPressEvent(e);
}

void FreyaMSLoginWidget::showEvent(QShowEvent *e)
{
    m_FunWidget->setDisabled(false);
    m_PassWDLEdit->clear();
    QWidget::showEvent(e);
}

void FreyaMSLoginWidget::InitWidget()
{
    FreyaBaseControl *pControl = FreyaBaseControl::GetFreyaControl();
    pControl->GetConfigFromFile(FREYAMS_PTH_GLOBALCONFIGPATH);

    setMouseTracking(true);
    QVBoxLayout *BackVLay = new QVBoxLayout(this);
    BackVLay->setSpacing(0);
    BackVLay->setMargin(0);
    BackVLay->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    m_FunWidget = new QWidget(this);
    m_FunWidget->setObjectName("LoginFunWidget");
    m_FunWidget->setMouseTracking(true);
    m_FunWidget->setFixedSize(400, 180);
    BackVLay->addWidget(m_FunWidget, 0, Qt::AlignCenter);

    QHBoxLayout *MainHLay = new QHBoxLayout(m_FunWidget);
    MainHLay->setSpacing(0);
    MainHLay->setMargin(0);

    QVBoxLayout *NetWorkVLay = new QVBoxLayout;
    NetWorkVLay->setSpacing(0);
    NetWorkVLay->setMargin(0);
    MainHLay->addLayout(NetWorkVLay);

    QHBoxLayout *HostHLay = new QHBoxLayout;
    HostHLay->setSpacing(0);
    HostHLay->setMargin(0);
    NetWorkVLay->addLayout(HostHLay);
    QLabel *HostLab = new QLabel(m_FunWidget);
    HostLab->setObjectName("HostLab");
    HostLab->setFixedSize(70, 26);
    HostLab->setText(tr("Host:"));
    HostHLay->addWidget(HostLab);
    m_HostLEdit = new QLineEdit(m_FunWidget);
    m_HostLEdit->setObjectName("HostLEdit");
    m_HostLEdit->setFixedSize(100, 26);
    m_HostLEdit->setText(pControl->GetConfig(QStringList()<<FREYAMS_CFG_CONFIDDEFAULT<<FREYAMS_CFG_CONFIDSERVER).toString());
    HostHLay->addWidget(m_HostLEdit);

    QHBoxLayout *PortHLay = new QHBoxLayout;
    PortHLay->setSpacing(0);
    PortHLay->setMargin(0);
    NetWorkVLay->addLayout(PortHLay);
    QLabel *PortLab = new QLabel(m_FunWidget);
    PortLab->setObjectName("PortLab");
    PortLab->setFixedSize(HostLab->size());
    PortLab->setText(tr("Port:"));
    PortHLay->addWidget(PortLab);
    m_PortLEdit = new QLineEdit(m_FunWidget);
    m_PortLEdit->setObjectName("PortLEdit");
    m_PortLEdit->setFixedSize(m_HostLEdit->size());
    m_PortLEdit->setText(pControl->GetConfig(QStringList()<<FREYAMS_CFG_CONFIDDEFAULT<<FREYAMS_CFG_CONFIDPORT).toString());
    PortHLay->addWidget(m_PortLEdit);

    QHBoxLayout *DomianHLay = new QHBoxLayout;
    DomianHLay->setSpacing(0);
    DomianHLay->setMargin(0);
    NetWorkVLay->addLayout(DomianHLay);
    QLabel *DomianLab = new QLabel(m_FunWidget);
    DomianLab->setObjectName("DomianLab");
    DomianLab->setFixedSize(HostLab->size());
    DomianLab->setText(tr("Domian:"));
    DomianHLay->addWidget(DomianLab);
    m_DomianLEdit = new QLineEdit(m_FunWidget);
    m_DomianLEdit->setObjectName("DomianLEdit");
    m_DomianLEdit->setFixedSize(m_HostLEdit->size());
    m_DomianLEdit->setText(pControl->GetConfig(QStringList()<<FREYAMS_CFG_CONFIDDEFAULT<<FREYAMS_CFG_CONFIDDOMAIN).toString());
    DomianHLay->addWidget(m_DomianLEdit);

    QHBoxLayout *ResourceHLay = new QHBoxLayout;
    ResourceHLay->setSpacing(0);
    ResourceHLay->setMargin(0);
    NetWorkVLay->addLayout(ResourceHLay);
    QLabel *ResourceLab = new QLabel(m_FunWidget);
    ResourceLab->setObjectName("ResourceLab");
    ResourceLab->setFixedSize(HostLab->size());
    ResourceLab->setText(tr("Resource:"));
    ResourceHLay->addWidget(ResourceLab);
    m_ResourceLEdit = new QLineEdit(m_FunWidget);
    m_ResourceLEdit->setObjectName("ResourceLEdit");
    m_ResourceLEdit->setFixedSize(m_HostLEdit->size());
    m_ResourceLEdit->setReadOnly(true);
    m_ResourceLEdit->setText(pControl->GetConfig(QStringList()<<FREYAMS_CFG_CONFIDDEFAULT<<FREYAMS_CFG_CONFIDRESOURCE).toString());
    ResourceHLay->addWidget(m_ResourceLEdit);

    QLabel *LineLab = new QLabel(m_FunWidget);
    LineLab->setObjectName("LineLab");
    LineLab->setFixedWidth(1);
    MainHLay->addWidget(LineLab);

    QVBoxLayout *AccountVLay = new QVBoxLayout;
    AccountVLay->setSpacing(0);
    AccountVLay->setMargin(0);
    MainHLay->addLayout(AccountVLay);

    QHBoxLayout *AccountHLay = new QHBoxLayout;
    AccountHLay->setSpacing(0);
    AccountHLay->setMargin(0);
    AccountVLay->addLayout(AccountHLay);
    QLabel *AccountLab = new QLabel(m_FunWidget);
    AccountLab->setObjectName("AccountLab");
    AccountLab->setFixedSize(HostLab->size());
    AccountLab->setText(tr("Account:"));
    AccountHLay->addWidget(AccountLab);
    m_AccountLEdit = new QLineEdit(m_FunWidget);
    m_AccountLEdit->setObjectName("AccountLEdit");
    m_AccountLEdit->setFixedSize(m_HostLEdit->size());
    m_AccountLEdit->setText(pControl->GetConfig(QStringList()<<FREYAMS_CFG_CONFIDDEFAULT<<FREYAMS_CFG_CONFIDACCOUND).toString());
    AccountHLay->addWidget(m_AccountLEdit);

    QHBoxLayout *PassWDHLay = new QHBoxLayout;
    PassWDHLay->setSpacing(0);
    PassWDHLay->setMargin(0);
    AccountVLay->addLayout(PassWDHLay);
    QLabel *PassWDLab = new QLabel(m_FunWidget);
    PassWDLab->setObjectName("PassWDLab");
    PassWDLab->setFixedSize(HostLab->size());
    PassWDLab->setText(tr("Password:"));
    PassWDHLay->addWidget(PassWDLab);
    m_PassWDLEdit = new QLineEdit(m_FunWidget);
    m_PassWDLEdit->setObjectName("PassWDLEdit");
    m_PassWDLEdit->setFixedSize(m_HostLEdit->size());
    m_PassWDLEdit->setEchoMode(QLineEdit::Password);
    PassWDHLay->addWidget(m_PassWDLEdit);

    QHBoxLayout *LoginHLay = new QHBoxLayout;
    LoginHLay->setSpacing(30);
    LoginHLay->setMargin(0);
    BackVLay->addLayout(LoginHLay);
    m_LoginBtn = new QToolButton(this);
    m_LoginBtn->setObjectName("LoginBtn");
    m_LoginBtn->setText(tr("Login"));
    m_LoginBtn->setFixedSize(100, 26);
    LoginHLay->addWidget(m_LoginBtn, 0, Qt::AlignRight);
    connect(m_LoginBtn, SIGNAL(clicked()), this, SLOT(OnLogin()));
    m_CancelLoginBtn = new QToolButton(this);
    m_CancelLoginBtn->setObjectName("CancelLoginBtn");
    m_CancelLoginBtn->setText(tr("Cancel"));
    m_CancelLoginBtn->setFixedSize(100, 26);
    LoginHLay->addWidget(m_CancelLoginBtn, 0, Qt::AlignLeft);
    connect(m_CancelLoginBtn, SIGNAL(clicked()), this, SLOT(OnCancelLogin()));
    BackVLay->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
}

void FreyaMSLoginWidget::OnLogin()
{
    m_FunWidget->setDisabled(true);

    FreyaBaseData loginData;
    loginData.command = FREYAMS_CMD_LOGINDEFAULT;

    QXmppConfiguration configuration;
    configuration.setHost(m_HostLEdit->text());
    configuration.setPort(m_PortLEdit->text().toInt());
    configuration.setDomain(m_DomianLEdit->text());
    configuration.setResource(m_ResourceLEdit->text() + QDateTime::currentDateTime().toString("-yyMMddHHmmss"));
    configuration.setJid(m_AccountLEdit->text() + "@" + m_DomianLEdit->text());
    configuration.setPassword(m_PassWDLEdit->text());

    loginData.arguments = CUSTOMCLSTOVARIANT(&configuration);
    FREYA_REQUESTEXECUTION(&loginData);
}

void FreyaMSLoginWidget::OnCancelLogin()
{
    m_FunWidget->setDisabled(false);
    FREYA_REQUESTEXECUTION(FREYAMS_CMD_LOGINCANCEL);
}
