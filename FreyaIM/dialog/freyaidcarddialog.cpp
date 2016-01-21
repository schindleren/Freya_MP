#include "freyaim_global.h"
#include "freyaidcarddialog.h"
#include "freyaportrait.h"
#include "calendaredit.h"

FreyaIDCardDialog::FreyaIDCardDialog(const QString &bareJid, FreyaWindow *parent) :
    FreyaBackWindow(parent, QString(FREYA_OBJ_IDCARDDIALOG + bareJid).toUtf8().data())
{
    m_BareJid = bareJid;
    InitWidget();
    LoadVCard();
    FREYA_REQUESTEXECUTION(FREYA_CMD_CONFIGNREQUEST);

    FreyaBaseData vCardDate;
    vCardDate.command = FREYA_CMD_VCARDREQUEST;
    vCardDate.arguments = m_BareJid;
    FREYA_REQUESTEXECUTION(&vCardDate);
}

FreyaIDCardDialog::~FreyaIDCardDialog()
{
}

void FreyaIDCardDialog::Execute()
{
}

void FreyaIDCardDialog::Execute(const quint64 &/*command*/)
{
}

void FreyaIDCardDialog::Execute(FreyaBaseData *pData)
{
    if(FREYA_CMD_VCARDRESULT == pData->command)
    {
        if(pData->arguments.toString() == m_BareJid)
        {
            LoadVCard();
        }
    }
    else if(FREYA_CMD_CONFIGNRESULT == pData->command)
    {
        QXmppConfiguration *xmppConfig = VARIANTTOCUSTOMCLS(pData->arguments, QXmppConfiguration);
        if(xmppConfig && xmppConfig->jidBare() == m_BareJid)
        {
            m_IDCEditBtn->setDisabled(false);
        }
    }
}

void FreyaIDCardDialog::InitWidget()
{
    SetShowMaxAble(false);
    SetResizeFrameWidth(0);
    needMaxBtn(false);
    setFixedSize(500, 260);
    SetTitle(tr("FreyaIDCardDialog"));

    QWidget *pMainBkWidget = MainBackWidget();
    QHBoxLayout *pMainHLay = new QHBoxLayout(pMainBkWidget);
    pMainHLay->setSpacing(6);
    pMainHLay->setMargin(6);

    QVBoxLayout *pPortraitVLay = new QVBoxLayout();
    pPortraitVLay->setSpacing(0);
    pPortraitVLay->setMargin(0);
    pMainHLay->addLayout(pPortraitVLay);

    m_IDCPortrait = new FreyaPortrait(pMainBkWidget);
    m_IDCPortrait->setFixedSize(100, 100);
    m_IDCPortrait->SetDefault(FREYA_PTH_GLOBALRESPATH FREYA_PTH_DEFPORTRAITPATH);
    m_IDCPortrait->SetPortraitByBareJid(m_BareJid);
    m_IDCPortrait->setDisabled(true);
    pPortraitVLay->addWidget(m_IDCPortrait);
    connect(m_IDCPortrait, SIGNAL(clicked()), this, SLOT(OnChangePotrait()));
    QLabel *pIDCIDNumLab = new QLabel(pMainBkWidget);
    pIDCIDNumLab->setObjectName("IDCIDNumLab");
    pIDCIDNumLab->setText(tr("IDNumber:"));
    pIDCIDNumLab->setFixedSize(m_IDCPortrait->width(), 30);
    pPortraitVLay->addWidget(pIDCIDNumLab);
    m_IDCNumLEdit = new QLineEdit(pMainBkWidget);
    m_IDCNumLEdit->setObjectName("IDCNumLEdit");
    m_IDCNumLEdit->setReadOnly(true);
    m_IDCNumLEdit->setText(m_BareJid.split('@').at(0));
    m_IDCNumLEdit->setFixedSize(pIDCIDNumLab->size());
    pPortraitVLay->addWidget(m_IDCNumLEdit);
    m_IDCEditBtn = new QToolButton(pMainBkWidget);
    m_IDCEditBtn->setObjectName("IDCEditBtn");
    m_IDCEditBtn->setFixedSize(pIDCIDNumLab->size());
    m_IDCEditBtn->setText(tr("Edit"));
    m_IDCEditBtn->setCheckable(true);
    pPortraitVLay->addWidget(m_IDCEditBtn);
    m_IDCEditBtn->setDisabled(true);
    connect(m_IDCEditBtn, SIGNAL(clicked(bool)), this, SLOT(OnEditModel(bool)));
///
    QVBoxLayout *pNickVLay = new QVBoxLayout();
    pNickVLay->setSpacing(10);
    pNickVLay->setMargin(0);
    pMainHLay->addLayout(pNickVLay);

    QHBoxLayout *pNickHLay = new QHBoxLayout();
    pNickHLay->setSpacing(0);
    pNickHLay->setMargin(0);
    pNickVLay->addLayout(pNickHLay);
    QLabel *pIDCNickLab = new QLabel(pMainBkWidget);
    pIDCNickLab->setObjectName("IDCNickLab");
    pIDCNickLab->setText(tr("NickName:"));
    pIDCNickLab->setFixedSize(60, pIDCIDNumLab->height());
    pNickHLay->addWidget(pIDCNickLab);
    m_IDCNickLEdit = new QLineEdit(pMainBkWidget);
    m_IDCNickLEdit->setObjectName("IDCNickLEdit");
    m_IDCNickLEdit->setReadOnly(true);
    m_IDCNickLEdit->setFixedHeight(m_IDCEditBtn->height());
    pNickHLay->addWidget(m_IDCNickLEdit);

    QHBoxLayout *pGenderHLay = new QHBoxLayout();
    pGenderHLay->setSpacing(0);
    pGenderHLay->setMargin(0);
    pNickVLay->addLayout(pGenderHLay);
    m_IDCGenderBtnG = new QButtonGroup(pMainBkWidget);
    QLabel *pIDCGenderLab = new QLabel(pMainBkWidget);
    pIDCGenderLab->setObjectName("IDCGenderLab");
    pIDCGenderLab->setText(tr("Gender:"));
    pIDCGenderLab->setFixedSize(pIDCIDNumLab->size());
    pGenderHLay->addWidget(pIDCGenderLab);
    m_IDCGenKeepBtn = new QToolButton(pMainBkWidget);
    m_IDCGenKeepBtn->setObjectName("IDCGenKeepBtn");
    m_IDCGenKeepBtn->setText("Secret");
    m_IDCGenKeepBtn->setCheckable(true);
    m_IDCGenKeepBtn->setDisabled(true);
    m_IDCGenKeepBtn->setFixedSize(pIDCIDNumLab->height()*2, pIDCIDNumLab->height());
    pGenderHLay->addWidget(m_IDCGenKeepBtn);
    m_IDCGenderBtnG->addButton(m_IDCGenKeepBtn);
    m_IDCMaleBtn = new QToolButton(pMainBkWidget);
    m_IDCMaleBtn->setObjectName("IDCMaleBtn");
    m_IDCMaleBtn->setText(tr("M"));
    m_IDCMaleBtn->setCheckable(true);
    m_IDCMaleBtn->setDisabled(true);
    m_IDCMaleBtn->setFixedSize(pIDCIDNumLab->height(), pIDCIDNumLab->height());
    pGenderHLay->addWidget(m_IDCMaleBtn);
    m_IDCGenderBtnG->addButton(m_IDCMaleBtn);
    m_IDCFemaleBtn = new QToolButton(pMainBkWidget);
    m_IDCFemaleBtn->setObjectName("IDCFemaleBtn");
    m_IDCFemaleBtn->setText(tr("W"));
    m_IDCFemaleBtn->setCheckable(true);
    m_IDCFemaleBtn->setDisabled(true);
    m_IDCFemaleBtn->setFixedSize(pIDCIDNumLab->height(), pIDCIDNumLab->height());
    pGenderHLay->addWidget(m_IDCFemaleBtn);
    m_IDCGenderBtnG->addButton(m_IDCFemaleBtn);

    QHBoxLayout *pBirthHLay = new QHBoxLayout();
    pBirthHLay->setSpacing(0);
    pBirthHLay->setMargin(0);
    pNickVLay->addLayout(pBirthHLay);
    QLabel *pIDCBirthLab = new QLabel(pMainBkWidget);
    pIDCBirthLab->setObjectName("IDCBirthLab");
    pIDCBirthLab->setFixedSize(pIDCNickLab->size());
    pIDCBirthLab->setText(tr("Birthday:"));
    pBirthHLay->addWidget(pIDCBirthLab);
    m_IDCBirthLEdit = new CalendarEdit(pMainBkWidget);
    m_IDCBirthLEdit->SetDateFormat(FREYA_DEF_DATEFORMAT);
    m_IDCBirthLEdit->setObjectName("IDCBirthLEdit");
    m_IDCBirthLEdit->SetDisabled(true);
    m_IDCBirthLEdit->setFixedHeight(pIDCIDNumLab->height());
    pBirthHLay->addWidget(m_IDCBirthLEdit);

    QHBoxLayout *pEMailHLay = new QHBoxLayout();
    pEMailHLay->setSpacing(0);
    pEMailHLay->setMargin(0);
    pNickVLay->addLayout(pEMailHLay);
    QLabel *pIDCEMailLab = new QLabel(pMainBkWidget);
    pIDCEMailLab->setObjectName("IDCEMailLab");
    pIDCEMailLab->setText(tr("E-Mail:"));
    pIDCEMailLab->setFixedSize(pIDCNickLab->size());
    pEMailHLay->addWidget(pIDCEMailLab);
    m_IDCEMailLEdit = new QLineEdit(pMainBkWidget);
    m_IDCEMailLEdit->setObjectName("IDCEMailLEdit");
    m_IDCEMailLEdit->setReadOnly(true);
    m_IDCEMailLEdit->setFixedHeight(m_IDCEditBtn->height());
    pEMailHLay->addWidget(m_IDCEMailLEdit);

    QHBoxLayout *pUrlHLay = new QHBoxLayout();
    pUrlHLay->setSpacing(0);
    pUrlHLay->setMargin(0);
    pNickVLay->addLayout(pUrlHLay);
    QLabel *pIDCUrlLab = new QLabel(pMainBkWidget);
    pIDCUrlLab->setObjectName("IDCUrlLab");
    pIDCUrlLab->setText(tr("URL:"));
    pIDCUrlLab->setFixedSize(pIDCNickLab->size());
    pUrlHLay->addWidget(pIDCUrlLab);
    m_IDCUrlLEdit = new QLineEdit(pMainBkWidget);
    m_IDCUrlLEdit->setObjectName("IDCUrlLEdit");
    m_IDCUrlLEdit->setReadOnly(true);
    m_IDCUrlLEdit->setFixedHeight(m_IDCEditBtn->height());
    pUrlHLay->addWidget(m_IDCUrlLEdit);

    QHBoxLayout *pAddHLay = new QHBoxLayout();
    pAddHLay->setSpacing(0);
    pAddHLay->setMargin(0);
    pNickVLay->addLayout(pAddHLay);
    QLabel *pIDCAddLab = new QLabel(pMainBkWidget);
    pIDCAddLab->setObjectName("IDCAddLab");
    pIDCAddLab->setText(tr("Address:"));
    pIDCAddLab->setFixedSize(pIDCNickLab->size());
    pAddHLay->addWidget(pIDCAddLab);
    m_IDCAddLEdit = new QLineEdit(pMainBkWidget);
    m_IDCAddLEdit->setObjectName("IDCAddLEdit");
    m_IDCAddLEdit->setReadOnly(true);
    m_IDCAddLEdit->setFixedHeight(m_IDCEditBtn->height());
    pAddHLay->addWidget(m_IDCAddLEdit);
///
    QVBoxLayout *pNameVLay = new QVBoxLayout();
    pNameVLay->setSpacing(pNickVLay->spacing());
    pNameVLay->setMargin(0);
    pMainHLay->addLayout(pNameVLay);

    QHBoxLayout *pNameHLay = new QHBoxLayout();
    pNameHLay->setSpacing(0);
    pNameHLay->setMargin(0);
    pNameVLay->addLayout(pNameHLay);
    QLabel *pIDCNameLab = new QLabel(pMainBkWidget);
    pIDCNameLab->setObjectName("IDCNameLab");
    pIDCNameLab->setText(tr("FullName:"));
    pIDCNameLab->setFixedSize(pIDCNickLab->size());
    pNameHLay->addWidget(pIDCNameLab);
    m_IDCNameLEdit = new QLineEdit(pMainBkWidget);
    m_IDCNameLEdit->setObjectName("IDCNameLEdit");
    m_IDCNameLEdit->setReadOnly(true);
    m_IDCNameLEdit->setFixedHeight(m_IDCEditBtn->height());
    pNameHLay->addWidget(m_IDCNameLEdit);

    QHBoxLayout *pPhoneHLay = new QHBoxLayout();
    pPhoneHLay->setSpacing(0);
    pPhoneHLay->setMargin(0);
    pNameVLay->addLayout(pPhoneHLay);
    QLabel *pIDCPhoneLab = new QLabel(pMainBkWidget);
    pIDCPhoneLab->setObjectName("IDCPhoneLab");
    pIDCPhoneLab->setText(tr("Phone:"));
    pIDCPhoneLab->setFixedSize(pIDCNickLab->size());
    pPhoneHLay->addWidget(pIDCPhoneLab);
    m_IDCPhoneLEdit = new QLineEdit(pMainBkWidget);
    m_IDCPhoneLEdit->setObjectName("IDCPhoneLEdit");
    m_IDCPhoneLEdit->setReadOnly(true);
    m_IDCPhoneLEdit->setFixedHeight(m_IDCEditBtn->height());
    pPhoneHLay->addWidget(m_IDCPhoneLEdit);

    QLabel *pIDCDesLab = new QLabel(pMainBkWidget);
    pIDCDesLab->setObjectName("IDCDesLab");
    pIDCDesLab->setFixedHeight(pIDCNickLab->height());
    pIDCDesLab->setText(tr("Description:"));
    pNameVLay->addWidget(pIDCDesLab);
    m_IDCDesPEdit = new QPlainTextEdit(pMainBkWidget);
    m_IDCDesPEdit->setObjectName("IDCDesPEdit");
    m_IDCDesPEdit->setReadOnly(true);
    pNameVLay->addWidget(m_IDCDesPEdit);
///
    pMainHLay->setStretch(0, 100);
    pMainHLay->setStretch(1, 220);
    pMainHLay->setStretch(2, 180);
}

void FreyaIDCardDialog::LoadVCard()
{
    FreyaBaseControl *pControl = FreyaBaseControl::GetFreyaControl();
    pControl->GetConfigFromFile(FREYA_FUN_PERSONCONFIGPATH(m_BareJid) + FREYA_PTH_PERSONCONFIGPATH);
    m_IDCNickLEdit->setText(pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDNICKNAME).toString());
    m_IDCBirthLEdit->SetDate(pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDBIRTHDAY).toDate());
    m_IDCDesPEdit->setPlainText(pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDDESCRIPTION).toString());
    m_IDCEMailLEdit->setText(pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDEMAIL).toString());
    m_IDCNameLEdit->setText(pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDFULLNAME).toString());
    m_IDCUrlLEdit->setText(pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDURL).toString());
    m_IDCAddLEdit->setText(pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDADDRESSES).toString());
    m_IDCPhoneLEdit->setText(pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDPHONE).toString());
    QString GenMidName = pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDGENDER).toString();
    if(FREYA_DEF_GENDERMALE == GenMidName)
    {
        m_IDCMaleBtn->setChecked(true);
    }
    else if(FREYA_DEF_GENDERFEMALE == GenMidName)
    {
        m_IDCFemaleBtn->setChecked(true);
    }
    else
    {
        m_IDCGenKeepBtn->setChecked(true);
    }
    m_IDCPortrait->SetPortraitByBareJid(m_BareJid);
}

void FreyaIDCardDialog::SaveVCard()
{
    FreyaBaseControl *pControl = FreyaBaseControl::GetFreyaControl();
    pControl->GetConfigFromFile(FREYA_FUN_PERSONCONFIGPATH(m_BareJid) + FREYA_PTH_PERSONCONFIGPATH);
    pControl->InsertConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDNICKNAME,
                           m_IDCNickLEdit->text());
    pControl->InsertConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDBIRTHDAY,
                           m_IDCBirthLEdit->GetDate());
    pControl->InsertConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDDESCRIPTION,
                           m_IDCDesPEdit->toPlainText());
    pControl->InsertConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDEMAIL,
                           m_IDCEMailLEdit->text());
    pControl->InsertConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDFULLNAME,
                           m_IDCNameLEdit->text());
    pControl->InsertConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDURL,
                           m_IDCUrlLEdit->text());
    pControl->InsertConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDADDRESSES,
                           m_IDCAddLEdit->text());
    pControl->InsertConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDPHONE,
                           m_IDCPhoneLEdit->text());
    pControl->InsertConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDGENDER,
                           m_IDCGenKeepBtn->isChecked()?"":(m_IDCMaleBtn->isChecked()?FREYA_DEF_GENDERMALE:FREYA_DEF_GENDERFEMALE));
    QImage potrait(m_IDCPortrait->GetPortrait());
    if(potrait.width() > FREYA_INF_DEFPORTRAITSIZE.width() ||
       potrait.height() > FREYA_INF_DEFPORTRAITSIZE.height())
    {
        potrait = potrait.scaled(FREYA_INF_DEFPORTRAITSIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    potrait.save(FreyaPortrait::GetPortraitPathByBareJid(m_BareJid));

    FreyaBaseData vCardData;
    vCardData.command = FREYA_CMD_VCARDMODREQUEST;
    vCardData.arguments = m_BareJid;
    FREYA_REQUESTEXECUTION(&vCardData);
}

void FreyaIDCardDialog::OnEditModel(bool isEdit)
{
    m_IDCPortrait->setDisabled(!isEdit);
    m_IDCNickLEdit->setReadOnly(!isEdit);
    m_IDCGenKeepBtn->setDisabled(!isEdit);
    m_IDCMaleBtn->setDisabled(!isEdit);
    m_IDCFemaleBtn->setDisabled(!isEdit);
    m_IDCEMailLEdit->setReadOnly(!isEdit);
    m_IDCAddLEdit->setReadOnly(!isEdit);
    m_IDCUrlLEdit->setReadOnly(!isEdit);
    m_IDCBirthLEdit->SetDisabled(!isEdit);
    m_IDCNameLEdit->setReadOnly(!isEdit);
    m_IDCPhoneLEdit->setReadOnly(!isEdit);
    m_IDCDesPEdit->setReadOnly(!isEdit);
    m_IDCEditBtn->setText(isEdit?tr("Save"):tr("Edit"));
    if(!isEdit)
    {
        SaveVCard();
    }
}

void FreyaIDCardDialog::OnChangePotrait()
{
    QString imageName = QFileDialog::getOpenFileName(this, tr("Select Potrait"),
                                 QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
                                 tr("Images (*.png *.jpg)"));
    QImage potrait(imageName);
    if(!imageName.isEmpty() && !potrait.isNull())
    {
        m_IDCPortrait->SetPortrait(imageName);
    }
}
