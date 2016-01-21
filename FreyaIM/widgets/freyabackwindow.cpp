#include "freyabackwindow.h"

FreyaBackWindow::FreyaBackWindow(FreyaWindow *parent, const char *objectName, FreyaBaseControl *pControl) :
    FreyaWindow(parent, pControl, objectName)
{
    InitWidget();
}

void FreyaBackWindow::InitWidget()
{
    setMouseTracking(true);
    QVBoxLayout *BackVLay = new QVBoxLayout(this);
    BackVLay->setSpacing(0);
    BackVLay->setMargin(0);

    m_BackWidget = new QWidget(this);
    m_BackWidget->setObjectName("FreyaBackWidget");
    m_BackWidget->setMouseTracking(true);
    BackVLay->addWidget(m_BackWidget);

    QVBoxLayout *MainVLay = new QVBoxLayout(m_BackWidget);
    MainVLay->setSpacing(0);
    MainVLay->setMargin(0);

    m_TitleLab = new QLabel(m_BackWidget);
    m_TitleLab->setObjectName("FreyaTitleLab");
    m_TitleLab->setFixedHeight(26);
    m_TitleLab->setMouseTracking(true);
    m_TitleLab->setAlignment(Qt::AlignCenter);
    MainVLay->addWidget(m_TitleLab);

    QHBoxLayout *TitleHLay = new QHBoxLayout(m_TitleLab);
    TitleHLay->setSpacing(0);
    TitleHLay->setMargin(0);

    m_LogoLab = new QLabel(m_TitleLab);
    m_LogoLab->setObjectName("FreyaLogoLab");
    m_LogoLab->setFixedSize(m_TitleLab->height(), m_TitleLab->height());
    m_LogoLab->setMouseTracking(true);
    m_LogoLab->setAlignment(Qt::AlignCenter);
    TitleHLay->addWidget(m_LogoLab);
    TitleHLay->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

    m_MinSizeBtn = new QToolButton(m_TitleLab);
    m_MinSizeBtn->setObjectName("FreyaMinTBtn");
    m_MinSizeBtn->setFixedSize(m_TitleLab->height(), m_TitleLab->height());
    m_MinSizeBtn->setMouseTracking(true);
    TitleHLay->addWidget(m_MinSizeBtn);
    connect(m_MinSizeBtn, SIGNAL(clicked()), this, SLOT(showMinimized()));

    m_MaxSizeBtn = new QToolButton(m_TitleLab);
    m_MaxSizeBtn->setObjectName("FreyaMaxTBtn");
    m_MaxSizeBtn->setFixedSize(m_TitleLab->height(), m_TitleLab->height());
    m_MaxSizeBtn->setCheckable(true);
    m_MaxSizeBtn->setMouseTracking(true);
    TitleHLay->addWidget(m_MaxSizeBtn);
    connect(m_MaxSizeBtn, SIGNAL(clicked()), this, SLOT(OnShowMaximized()));
    connect(this, SIGNAL(ToShowMaximized(bool)), m_MaxSizeBtn, SLOT(setChecked(bool)));

    m_CloseBtn = new QToolButton(m_TitleLab);
    m_CloseBtn->setObjectName("FreyaCloseTBtn");
    m_CloseBtn->setFixedSize(m_TitleLab->height() * 1.5, m_TitleLab->height());
    m_CloseBtn->setMouseTracking(true);
    TitleHLay->addWidget(m_CloseBtn);
    connect(m_CloseBtn, SIGNAL(clicked()), this, SLOT(close()));

    m_MainWidget = new QWidget(m_BackWidget);
    m_MainWidget->setObjectName("FreyaMainWidget");
    m_MainWidget->setMouseTracking(true);
    MainVLay->addWidget(m_MainWidget);
}
