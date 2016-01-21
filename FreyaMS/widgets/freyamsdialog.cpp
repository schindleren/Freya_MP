#include "freyams_global.h"
#include "freyamsdialog.h"
#include "freyamsloginwidget.h"
#include "freyamswidget.h"
#include "QXmppClient.h"

FreyaMSDialog::FreyaMSDialog(FreyaWindow *parent) :
    FreyaWindow(parent, FreyaBaseControl::GetFreyaControl(), FREYAMS_OBJ_MSDIALOG)
{
    InitWidget();
}

void FreyaMSDialog::InitWidget()
{
    setMouseTracking(true);
    QVBoxLayout *BackVLay = new QVBoxLayout(this);
    BackVLay->setSpacing(0);
    BackVLay->setMargin(0);

    m_BackWidget = new QWidget(this);
    m_BackWidget->setObjectName("MSBackWidget");
    m_BackWidget->setMouseTracking(true);
    BackVLay->addWidget(m_BackWidget);

    QVBoxLayout *MainVLay = new QVBoxLayout(m_BackWidget);
    MainVLay->setSpacing(0);
    MainVLay->setMargin(0);

    m_TitleLab = new QLabel(m_BackWidget);
    m_TitleLab->setObjectName("MSTitleLab");
    m_TitleLab->setFixedHeight(26);
    m_TitleLab->setMouseTracking(true);
    m_TitleLab->setAlignment(Qt::AlignCenter);
    MainVLay->addWidget(m_TitleLab);

    QHBoxLayout *TitleHLay = new QHBoxLayout(m_TitleLab);
    TitleHLay->setSpacing(0);
    TitleHLay->setMargin(0);

    m_LogoLab = new QLabel(m_TitleLab);
    m_LogoLab->setObjectName("MSLogoLab");
    m_LogoLab->setFixedSize(m_TitleLab->height(), m_TitleLab->height());
    m_LogoLab->setMouseTracking(true);
    m_LogoLab->setAlignment(Qt::AlignCenter);
    TitleHLay->addWidget(m_LogoLab);
    TitleHLay->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

    m_MinSizeBtn = new QToolButton(m_TitleLab);
    m_MinSizeBtn->setObjectName("MSMinTBtn");
    m_MinSizeBtn->setFixedSize(m_TitleLab->height(), m_TitleLab->height());
    m_MinSizeBtn->setMouseTracking(true);
    TitleHLay->addWidget(m_MinSizeBtn);
    connect(m_MinSizeBtn, SIGNAL(clicked()), this, SLOT(showMinimized()));

    m_MaxSizeBtn = new QToolButton(m_TitleLab);
    m_MaxSizeBtn->setObjectName("MSMaxTBtn");
    m_MaxSizeBtn->setFixedSize(m_TitleLab->height(), m_TitleLab->height());
    m_MaxSizeBtn->setCheckable(true);
    m_MaxSizeBtn->setMouseTracking(true);
    TitleHLay->addWidget(m_MaxSizeBtn);
    connect(m_MaxSizeBtn, SIGNAL(clicked()), this, SLOT(OnShowMaximized()));
    connect(this, SIGNAL(ToShowMaximized(bool)), m_MaxSizeBtn, SLOT(setChecked(bool)));

    m_CloseBtn = new QToolButton(m_TitleLab);
    m_CloseBtn->setObjectName("MSCloseTBtn");
    m_CloseBtn->setFixedSize(m_TitleLab->height() * 1.5, m_TitleLab->height());
    m_CloseBtn->setMouseTracking(true);
    TitleHLay->addWidget(m_CloseBtn);
    connect(m_CloseBtn, SIGNAL(clicked()), this, SLOT(close()));

    m_BackStackedWidget = new QStackedWidget(m_BackWidget);
    m_BackStackedWidget->setObjectName("MSStackedWidget");
    m_BackStackedWidget->setMouseTracking(true);
    MainVLay->addWidget(m_BackStackedWidget);

    m_StateLab = new QLabel(m_BackWidget);
    m_StateLab->setObjectName("MSStateLab");
    m_StateLab->setFixedHeight(26);
    m_StateLab->setMouseTracking(true);
    MainVLay->addWidget(m_StateLab);

    m_StateTimer = new QTimer(m_StateLab);
    m_StateTimer->setSingleShot(true);
    connect(m_StateTimer, SIGNAL(timeout()), this, SLOT(OnStateTimer()));

    m_LoginWidget = new FreyaMSLoginWidget(m_BackStackedWidget);
    m_LoginWidget->setObjectName("FreyaMSLoginWidget");
    m_BackStackedWidget->addWidget(m_LoginWidget);

    m_MSWidget = new FreyaMSWidget(m_BackStackedWidget);
    m_MSWidget->setObjectName("FreyaMSWidget");
    m_BackStackedWidget->addWidget(m_MSWidget);

    m_BackStackedWidget->setCurrentWidget(m_LoginWidget);
}

void FreyaMSDialog::ShowInforWithTimer(const FreyaInfor &infor)
{
    if(m_StateTimer->isActive())
    {
        m_StateTimer->stop();
    }
    m_StateLab->setText(QString("<p style=\"color: %1\">%2</p>").arg(infor.color.name()).arg(infor.text));
    qDebug()<<m_StateLab->text();
    m_StateTimer->setInterval(infor.timer);
    m_StateTimer->start();
}

void FreyaMSDialog::OnStateTimer()
{
    m_StateLab->clear();
}

void FreyaMSDialog::Execute(const quint64 &command)
{
    if(FREYAMS_CMD_LOCKED == command)
    {
        m_BackStackedWidget->setCurrentWidget(m_LoginWidget);
    }
}

void FreyaMSDialog::Execute(FreyaBaseData *pData)
{
    if(FREYAMS_CMD_SHOWINFOR == pData->command)
    {
        FreyaInfor *pInfo = VARIANTTOCUSTOMCLS(pData->arguments, FreyaInfor);
        if(pInfo)
            ShowInforWithTimer(*pInfo);
    }
    else if(FREYAMS_CMD_CONNECTEDSTATE == pData->command)
    {
        switch (pData->arguments.toInt())
        {
        case QXmppClient::ConnectedState:
            m_BackStackedWidget->setCurrentWidget(m_MSWidget);
            break;
        case QXmppClient::DisconnectedState:
            m_BackStackedWidget->setCurrentWidget(m_LoginWidget);
            break;
        default:
            break;
        }
    }
}

void FreyaMSDialog::closeEvent(QCloseEvent *e)
{
    qApp->quit();
    FreyaWindow::closeEvent(e);
}
