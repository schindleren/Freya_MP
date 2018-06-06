/*******************************************************************************
 File Name        : rtabwidget.cpp
 Summary          : A Tab Widget with buttons and widgets
 Create           : 2013-01-08
 Author           : RenDan
 Update history   :
 NO.     Author        Date         Content
 001     Ren Dan       2013-01-08   Create New
 002     Ren Dan       2013-02-26   Modifed for tab at left
 003     Ren Dan       2013-05-10   Delete TabPage Class
 004     Ren Dan       2014-10-20   Add QButtonGroup and delete QList<QToolButton*>
 005     Ren Dan       2015-07-23   Remove parameter of tab direction at construct funcation
 006     Ren Dan       2015-07-23   Add tab close button
*******************************************************************************/
#include "rtabwidget.h"


RTabButton::RTabButton(QWidget *parent):
    QToolButton(parent)
{
    QHBoxLayout *pCloseHLay = new QHBoxLayout(this);
    pCloseHLay->setSpacing(0);
    pCloseHLay->setMargin(0);

    m_CloseBtn = new QToolButton(this);
    m_CloseBtn->setObjectName("RTab_CloseBtn");
    m_CloseBtn->setFixedSize(20, 20);
    pCloseHLay->addWidget(m_CloseBtn, 0, Qt::AlignHCenter | Qt::AlignRight);
    connect(m_CloseBtn, SIGNAL(clicked()), this, SIGNAL(ToCloseBtnClicked()));
}

void RTabButton::SetCloseBtnSize(const QSize &size)
{
    m_CloseBtn->setFixedSize(size);
}

void RTabButton::SetCloseable(bool closeable)
{
    m_CloseBtn->setVisible(closeable);
}

RTabWidget::RTabWidget(QWidget *parent, bool isExpand):
    QWidget(parent), m_isExpand(isExpand), m_nTabBKWidth(100), m_nTabBKHeight(30),
    m_nTabWidth(100), m_nTabHeight(30), m_isTabCloseable(false), m_CloseBtnSize(QSize(0, 0))
{
    InitWidget();
}

void RTabWidget::SetTabBackObjectName( const QString &ObjName )
{
    m_TabBackWidget->setObjectName(ObjName);
}

void RTabWidget::SetTabBKWidth(const int &w)
{
    m_nTabBKWidth = w;
    m_TabBackWidget->setFixedWidth(w);
}

void RTabWidget::SetTabBKHeight(const int &h)
{
    m_nTabBKHeight = h;
    m_TabBackWidget->setFixedHeight(h);
}

void RTabWidget::SetTabWidth(const int &w)
{
    m_nTabWidth = w;
    QList<QAbstractButton*> btnList = m_TabBtnGroup->buttons();
    foreach (QAbstractButton *btn, btnList)
    {
        if(btn)
        {
            btn->setFixedWidth(m_nTabWidth);
        }
    }
}

void RTabWidget::SetTabHeight(const int &h)
{
    m_nTabHeight = h;
    QList<QAbstractButton*> btnList = m_TabBtnGroup->buttons();
    foreach (QAbstractButton *btn, btnList)
    {
        if(btn)
        {
            btn->setFixedHeight(m_nTabHeight);
        }
    }
}

void RTabWidget::SetTabSite(const RTabSite::site &site)
{
    m_TabWidgetBkLay->removeWidget(m_TabBackWidget);
    m_TabWidgetBkLay->removeWidget(m_PageWidgetStack);
    switch (site)
    {
    case RTabSite::eSiteLeft:
        m_TabLay->setDirection(QBoxLayout::TopToBottom);
        m_TabWidgetBkLay->setDirection(QBoxLayout::LeftToRight);
        m_TabWidgetBkLay->addWidget(m_TabBackWidget);
        m_TabWidgetBkLay->addWidget(m_PageWidgetStack);
        break;
    case RTabSite::eSiteTop:
        m_TabLay->setDirection(QBoxLayout::LeftToRight);
        m_TabWidgetBkLay->setDirection(QBoxLayout::TopToBottom);
        m_TabWidgetBkLay->addWidget(m_TabBackWidget);
        m_TabWidgetBkLay->addWidget(m_PageWidgetStack);
        break;
    case RTabSite::eSiteRight:
        m_TabLay->setDirection(QBoxLayout::TopToBottom);
        m_TabWidgetBkLay->setDirection(QBoxLayout::LeftToRight);
        m_TabWidgetBkLay->addWidget(m_PageWidgetStack);
        m_TabWidgetBkLay->addWidget(m_TabBackWidget);
        break;
    case RTabSite::eSiteBottom:
        m_TabLay->setDirection(QBoxLayout::LeftToRight);
        m_TabWidgetBkLay->setDirection(QBoxLayout::TopToBottom);
        m_TabWidgetBkLay->addWidget(m_PageWidgetStack);
        m_TabWidgetBkLay->addWidget(m_TabBackWidget);
        break;
    default:
        break;
    }
}

void RTabWidget::SetTabEnable(const int &index, bool enabled)
{
    QAbstractButton *btn = m_TabBtnGroup->button(index);
    if(NULL != btn)
    {
        btn->setEnabled(enabled);
    }
}

void RTabWidget::SetTabMarginAndSpacing(const int &Margin, const int &Spacing)
{
    m_TabLay->setMargin(Margin);
    m_TabLay->setSpacing(Spacing);
}

void RTabWidget::SetTabsCloseable(bool closeable)
{
    if(m_isTabCloseable != closeable)
    {
        m_isTabCloseable = closeable;
        QList<QAbstractButton*> btnList = m_TabBtnGroup->buttons();
        foreach (QAbstractButton *btn, btnList)
        {
            RTabButton *pRTabBtn = qobject_cast<RTabButton*>(btn);
            if(pRTabBtn)
            {
                pRTabBtn->SetCloseable(m_isTabCloseable);
            }
        }
    }
}

void RTabWidget::SetTabsCloseBtnSize(const QSize &size)
{
    if(m_CloseBtnSize != size)
    {
        m_CloseBtnSize = size;
        QList<QAbstractButton*> btnList = m_TabBtnGroup->buttons();
        foreach (QAbstractButton *btn, btnList)
        {
            RTabButton *pRTabBtn = qobject_cast<RTabButton*>(btn);
            if(pRTabBtn)
            {
                pRTabBtn->SetCloseBtnSize(m_CloseBtnSize);
            }
        }
    }
}


void RTabWidget::InsertWidgetBeforeTab(QWidget *widget)
{
    m_TabLay->insertWidget(0, widget);
}

void RTabWidget::InsertWidgetAfterTab(QWidget *widget)
{
    m_TabLay->addWidget(widget);
}

void RTabWidget::InitWidget()
{
    m_TabWidgetBkLay = new QVBoxLayout;
    m_TabLay = new QHBoxLayout;
    m_TabWidgetBkLay->setSpacing(0);
    m_TabWidgetBkLay->setMargin(0);
    m_TabLay->setSpacing(0);
    m_TabLay->setMargin(0);
    if (!m_isExpand)
    {
        m_TabLay->addSpacerItem(new QSpacerItem(0, 0,QSizePolicy::Expanding, QSizePolicy::Expanding));
    }
    setLayout(m_TabWidgetBkLay);
    m_TabBackWidget = new QWidget(this);
    m_TabBackWidget->setMouseTracking(true);
    m_TabBackWidget->setLayout(m_TabLay);
    m_TabWidgetBkLay->addWidget(m_TabBackWidget);
    m_TabBtnGroup = new QButtonGroup(m_TabBackWidget);
    connect(m_TabBtnGroup, SIGNAL(buttonClicked(int)), this, SLOT(OnCurrentTab(int)));
    m_PageWidgetStack = new QStackedWidget(this);
    m_PageWidgetStack->setMouseTracking(true);
    m_PageWidgetStack->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_TabWidgetBkLay->addWidget(m_PageWidgetStack);
}

void RTabWidget::AddTabAndWidget(QWidget* tabWidget, const QString &objName, const QString &tabTitle, const QIcon &tabIcon)
{
    RTabButton *nTabBtn = new RTabButton(this);
    nTabBtn->setObjectName(objName.isEmpty()?tabWidget->objectName():objName);
    nTabBtn->setMouseTracking(true);
    nTabBtn->setText(tabTitle);
    nTabBtn->setIcon(tabIcon);
    nTabBtn->setCheckable(true);
    nTabBtn->SetCloseable(m_isTabCloseable);
    nTabBtn->SetCloseBtnSize(m_CloseBtnSize);
    nTabBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(nTabBtn, SIGNAL(ToCloseBtnClicked()), this, SLOT(OnCloseBtnClicked()));
    if (!m_isExpand)
    {
        nTabBtn->setFixedSize(m_nTabWidth, m_nTabHeight);
    }
    m_TabLay->insertWidget((m_TabLay->count()-(m_isExpand?0:1)), nTabBtn);
    m_TabBtnGroup->addButton(nTabBtn, m_TabBtnGroup->buttons().size());

    tabWidget->setMouseTracking(true);
    tabWidget->setObjectName(nTabBtn->objectName());
    m_PageWidgetStack->addWidget(tabWidget);
}

void RTabWidget::RemoveTab(int index)
{
    QAbstractButton *pBtn = m_TabBtnGroup->button(index);
    m_TabLay->removeWidget(pBtn);
    m_TabBtnGroup->removeButton(pBtn);
    pBtn->deleteLater();
    QWidget *pWidget = Widget(index);
    m_PageWidgetStack->removeWidget(pWidget);
}

int RTabWidget::CurrentTabPage()
{
    return m_PageWidgetStack->currentIndex();
}

QWidget *RTabWidget::Widget(const int &index)
{
    return ((m_PageWidgetStack->count() > index) ? m_PageWidgetStack->widget(index) : 0);
}

int RTabWidget::Index(QWidget *widget)
{
    return m_PageWidgetStack->indexOf(widget);
}

void RTabWidget::OnCurrentTab(int index)
{
    if(index < m_PageWidgetStack->count())
    {
        m_PageWidgetStack->setCurrentIndex(index);
        if(!m_TabBtnGroup->button(index)->isChecked())
        {
            m_TabBtnGroup->button(index)->setChecked(true);
        }
        emit ToTabChanged(index);
    }
}

void RTabWidget::OnCloseBtnClicked()
{
    QAbstractButton *pBtn = qobject_cast<QAbstractButton*>(sender());
    if(pBtn)
    {
        emit ToCloseBtnClicked(m_TabBtnGroup->id(pBtn));
    }
}
