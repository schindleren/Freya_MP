#include "freyaim_global.h"
#include "freyaregisterdialog.h"
#include "verifier.h"
#include "freyadefaultclient.h"

FreyaRegisterDialog::FreyaRegisterDialog(FreyaBackWindow *parent) :
    FreyaBackWindow(parent, FREYA_OBJ_REGISTERDIALOG)
{
    InitWidget();
}

FreyaRegisterDialog::~FreyaRegisterDialog()
{
}

void FreyaRegisterDialog::InitWidget()
{
    SetShowMaxAble(false);
    SetResizeFrameWidth(0);
    needMaxBtn(false);
    setFixedSize(300, 260);
    SetTitle(tr("FreyaRegisterDialog"));

    QWidget *MainWidget = MainBackWidget();
    QVBoxLayout *MainVLay = new QVBoxLayout(MainWidget);
    MainVLay->setSpacing(10);
    MainVLay->setMargin(10);

    m_RegisterInforWidget = new QWidget(this);
    m_RegisterInforWidget->setObjectName("RegisterInforWidget");
    m_RegisterInforWidget->hide();

    QVBoxLayout *pRegInforVLay = new QVBoxLayout(m_RegisterInforWidget);
    pRegInforVLay->setSpacing(0);
    pRegInforVLay->setMargin(0);

    m_RegisterMovieLab = new QLabel(m_RegisterInforWidget);
    m_RegisterMovieLab->setObjectName("RegisterMovieLab");
    m_RegisterMovieLab->setAlignment(Qt::AlignCenter);
    m_RegisterMovie = new QMovie(m_RegisterMovieLab);
    m_RegisterMovie->setFileName(FREYA_PTH_LOGINGIFPATH);
    m_RegisterMovieLab->setMovie(m_RegisterMovie);
    pRegInforVLay->addWidget(m_RegisterMovieLab);
    m_RegisterInforLab = new QLabel(m_RegisterInforWidget);
    m_RegisterInforLab->setObjectName("RegisterInforLab");
    m_RegisterInforLab->setFixedHeight(30);
    m_RegisterInforLab->setAlignment(Qt::AlignCenter);
    pRegInforVLay->addWidget(m_RegisterInforLab);
    m_RegisterGoBtn = new QToolButton(m_RegisterInforWidget);
    m_RegisterGoBtn->setObjectName("RegisterGoBtn");
    m_RegisterGoBtn->setFixedSize(160, 40);
    m_RegisterGoBtn->setText(tr("Return"));
    pRegInforVLay->addWidget(m_RegisterGoBtn, 0, Qt::AlignCenter);
    connect(m_RegisterGoBtn, SIGNAL(clicked()), m_RegisterInforWidget, SLOT(hide()));

    QHBoxLayout *AccountHLay = new QHBoxLayout;
    AccountHLay->setSpacing(0);
    AccountHLay->setMargin(0);
    MainVLay->addLayout(AccountHLay);
    QLabel *AccountLab = new QLabel(MainWidget);
    AccountLab->setObjectName("AccountLab");
    AccountLab->setText(tr("Account:"));
    AccountLab->setFixedSize(80, 30);
    AccountHLay->addWidget(AccountLab);
    m_AccountLEdit = new QLineEdit(MainWidget);
    m_AccountLEdit->setObjectName("AccountLEdit");
    m_AccountLEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_AccountLEdit->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9_]*", Qt::CaseInsensitive), m_AccountLEdit));
    m_AccountLEdit->setContextMenuPolicy(Qt::NoContextMenu);
    AccountHLay->addWidget(m_AccountLEdit);

    QHBoxLayout *EmailHLay = new QHBoxLayout;
    EmailHLay->setSpacing(0);
    EmailHLay->setMargin(0);
    MainVLay->addLayout(EmailHLay);
    QLabel *EmailLab = new QLabel(MainWidget);
    EmailLab->setObjectName("EmailLab");
    EmailLab->setFixedSize(AccountLab->size());
    EmailLab->setText(tr("Email:"));
    EmailHLay->addWidget(EmailLab);
    m_EmailLEdit = new QLineEdit(MainWidget);
    m_EmailLEdit->setObjectName("EmailLEdit");
    m_EmailLEdit->setContextMenuPolicy(Qt::NoContextMenu);
    m_EmailLEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    EmailHLay->addWidget(m_EmailLEdit);

    QHBoxLayout *PassWordHLay = new QHBoxLayout;
    PassWordHLay->setSpacing(0);
    PassWordHLay->setMargin(0);
    MainVLay->addLayout(PassWordHLay);
    QLabel *PassWordLab = new QLabel(MainWidget);
    PassWordLab->setObjectName("PassWordLab");
    PassWordLab->setText(tr("Password:"));
    PassWordLab->setFixedSize(AccountLab->size());
    PassWordHLay->addWidget(PassWordLab);
    m_PassWordLEdit = new QLineEdit(MainWidget);
    m_PassWordLEdit->setObjectName("PassWordLEdit");
    m_PassWordLEdit->setContextMenuPolicy(Qt::NoContextMenu);
    m_PassWordLEdit->setEchoMode(QLineEdit::Password);
    m_PassWordLEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    PassWordHLay->addWidget(m_PassWordLEdit);

    QHBoxLayout *RePassWordHLay = new QHBoxLayout;
    RePassWordHLay->setSpacing(0);
    RePassWordHLay->setMargin(0);
    MainVLay->addLayout(RePassWordHLay);
    QLabel *RePassWordLab = new QLabel(MainWidget);
    RePassWordLab->setObjectName("RePassWordLab");
    RePassWordLab->setText(tr("Input again:"));
    RePassWordLab->setFixedSize(AccountLab->size());
    RePassWordHLay->addWidget(RePassWordLab);
    m_RePassWordLEdit = new QLineEdit(MainWidget);
    m_RePassWordLEdit->setObjectName("RePassWordLEdit");
    m_RePassWordLEdit->setContextMenuPolicy(Qt::NoContextMenu);
    m_RePassWordLEdit->setEchoMode(QLineEdit::Password);
    m_RePassWordLEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    RePassWordHLay->addWidget(m_RePassWordLEdit);

    QHBoxLayout *VerifierHLay = new QHBoxLayout;
    VerifierHLay->setSpacing(0);
    VerifierHLay->setMargin(0);
    MainVLay->addLayout(VerifierHLay);
    QLabel *VerifierLab = new QLabel(MainWidget);
    VerifierLab->setObjectName("VerifierLab");
    VerifierLab->setText(tr("Verification:"));
    VerifierLab->setFixedSize(AccountLab->size());
    VerifierHLay->addWidget(VerifierLab);
    m_VerifierLEdit = new QLineEdit(MainWidget);
    m_VerifierLEdit->setObjectName("VerifierLEdit");
    m_VerifierLEdit->setContextMenuPolicy(Qt::NoContextMenu);
    m_VerifierLEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    VerifierHLay->addWidget(m_VerifierLEdit);
    m_VerifierLab = new QLabel(MainWidget);
    m_VerifierLab->setObjectName("VerifierLab");
    m_VerifierLab->setFixedWidth(100);
    VerifierHLay->addWidget(m_VerifierLab);
    m_ChangeVerifierBtn = new QToolButton(MainWidget);
    m_ChangeVerifierBtn->setObjectName("ChangeVerifierBtn");
    m_ChangeVerifierBtn->setFixedSize(AccountLab->height(), AccountLab->height());
    VerifierHLay->addWidget(m_ChangeVerifierBtn);
    connect(m_ChangeVerifierBtn, SIGNAL(clicked()), this, SLOT(OnChangeVerifierCode()));

    QHBoxLayout *ReturnBtnHLay = new QHBoxLayout;
    ReturnBtnHLay->setSpacing(0);
    ReturnBtnHLay->setMargin(0);
    MainVLay->addLayout(ReturnBtnHLay);
    m_AcceptBtn = new QToolButton(MainWidget);
    m_AcceptBtn->setObjectName("RegisterAcceptBtn");
    m_AcceptBtn->setText(tr("Submit"));
    m_AcceptBtn->setFixedSize(100, AccountLab->height());
    ReturnBtnHLay->addWidget(m_AcceptBtn);
    connect(m_AcceptBtn, SIGNAL(clicked()), this, SLOT(OnCheckInformation()));
    m_RejectBtn = new QToolButton(MainWidget);
    m_RejectBtn->setObjectName("RegisterRejectBtn");
    m_RejectBtn->setText(tr("Cancel"));
    m_RejectBtn->setFixedSize(m_AcceptBtn->size());
    ReturnBtnHLay->addWidget(m_RejectBtn);
    connect(m_RejectBtn, SIGNAL(clicked()), this, SLOT(reject()));

    OnChangeVerifierCode();
}

void FreyaRegisterDialog::RegisterError(int code)
{
    switch (code)
    {
    case 409:
        RegisterResultShow(false, tr("Error:User name already exists!"));
        break;
    default:
        RegisterResultShow(false, tr("Error:Unknow error!"));
        break;
    }
}

void FreyaRegisterDialog::RegisterResult()
{
    RegisterResultShow(true, tr("Register successed!"));
}

void FreyaRegisterDialog::RegisterResultShow(bool isSuccess, const QString &infor)
{
    QString showInfor;
    if(isSuccess)
    {
        showInfor = "<p style='color:green'>%1 </p>";
    }
    else
    {
        showInfor = "<p style='color:red'>%1 </p>";
    }
    m_RegisterInforLab->setTextFormat(Qt::RichText);
    m_RegisterInforLab->setText(showInfor.arg(infor));
}

void FreyaRegisterDialog::Execute()
{
}

void FreyaRegisterDialog::Execute(const quint64 &/*command*/)
{
}

void FreyaRegisterDialog::Execute(FreyaBaseData *pData)
{
    if(FREYA_CMD_REGISTRESULT == pData->command)
    {
        if(pData->dataID == m_DataId)
        {
            if(pData->arguments.type() == QVariant::String)
            {
                RegisterResultShow(false, pData->arguments.toString());
            }
            else
            {
                QXmppIq *pIQ = VARIANTTOCUSTOMCLS(pData->arguments, QXmppIq);
                if(pIQ && pIQ->id() == m_DataId)
                {
                    switch (pIQ->type())
                    {
                    case QXmppIq::Error:
                        RegisterError(pIQ->error().code());
                        break;
                    case QXmppIq::Result:
                        RegisterResult();
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
}

void FreyaRegisterDialog::OnChangeVerifierCode()
{
    m_VerifierCode.clear();
    m_VerifierLab->setPixmap(Verifier::VerifierCodePixmap(m_VerifierCode, m_VerifierLab->size()));
}

void FreyaRegisterDialog::OnCheckInformation()
{
    FreyaBaseData data;
    data.command = FREYA_CMD_TRAYMESSAGE;
    QVariantMap messageMap;
    messageMap.insert(FREYA_INT_TRAYMSGSTYLE, QSystemTrayIcon::Critical);
    if(m_AccountLEdit->text().isEmpty())
    {
        messageMap.insert(FREYA_STR_TRAYMSGCONTENT, tr("Account cannot be empty!"));
    }
    else if(m_EmailLEdit->text().isEmpty())
    {
        messageMap.insert(FREYA_STR_TRAYMSGCONTENT, tr("Email cannot be empty!"));
    }
    else if(m_PassWordLEdit->text().isEmpty())
    {
        messageMap.insert(FREYA_STR_TRAYMSGCONTENT, tr("Password cannot be empty!"));
    }
    else if(m_VerifierLEdit->text().isEmpty())
    {
        messageMap.insert(FREYA_STR_TRAYMSGCONTENT, tr("Verification cannot be empty!"));
    }
    else if(m_PassWordLEdit->text() != m_RePassWordLEdit->text())
    {
        messageMap.insert(FREYA_STR_TRAYMSGCONTENT, tr("Twice password is inconsistencies!"));
    }
    else if(m_VerifierLEdit->text() != m_VerifierCode)
    {
        messageMap.insert(FREYA_STR_TRAYMSGCONTENT, tr("Verification is wrong!"));
    }
    else
    {
        m_RegisterInforWidget->show();
        m_RegisterInforWidget->setGeometry(MainBackWidget()->geometry());
        m_RegisterMovie->start();

        FreyaBaseData RegisterData;
        m_DataId = RegisterData.dataID;
        QXmppRegisterIq xmppRegisterIq;
        xmppRegisterIq.setUsername(m_AccountLEdit->text());
        xmppRegisterIq.setEmail(m_EmailLEdit->text());
        xmppRegisterIq.setPassword(QCryptographicHash::hash(m_PassWordLEdit->text().toUtf8(),
                                                               QCryptographicHash::Md5).toHex());
        xmppRegisterIq.setType(QXmppIq::Set);
        xmppRegisterIq.setId(m_DataId);
        RegisterData.command = FREYA_CMD_REGISTREQUEST;
        RegisterData.arguments = CUSTOMCLSTOVARIANT(&xmppRegisterIq);
        FREYA_REQUESTEXECUTION(&RegisterData);
        OnChangeVerifierCode();
        return;
    }
    data.arguments = messageMap;
    FREYA_REQUESTEXECUTION(&data);
    OnChangeVerifierCode();
}
