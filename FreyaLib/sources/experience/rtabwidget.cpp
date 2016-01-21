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

/***************************************************
  Author: RenDan
  Date: 2015-07-23
  Description: Set tab close btn size
  Input: QSize
  Output: void
  Return: void
****************************************************/
void RTabButton::SetCloseBtnSize(const QSize &size)
{
    m_CloseBtn->setFixedSize(size);
}

/***************************************************
  Author: RenDan
  Date: 2015-07-23
  Description: Set tab close able
  Input: bool
  Output: void
  Return: void
****************************************************/
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

/***************************************************
  Author: RenDan
  Date: 2013-01-08
  Description: Set object name
  Input: QString
  Output: void
  Return: void
****************************************************/
void RTabWidget::SetTabBackObjectName( const QString &ObjName )
{
    m_TabBackWidget->setObjectName(ObjName);
}

/***************************************************
  Author: RenDan
  Date: 2013-05-10
  Description: Set width of tab back widget
  Input: int
  Output: void
  Return: void
****************************************************/
void RTabWidget::SetTabBKWidth(const int &w)
{
    m_nTabBKWidth = w;
    m_TabBackWidget->setFixedWidth(w);
}

/***************************************************
  Author: RenDan
  Date: 2013-05-10
  Description: Set height of tab back widget
  Input: int
  Output: void
  Return: void
****************************************************/
void RTabWidget::SetTabBKHeight(const int &h)
{
    m_nTabBKHeight = h;
    m_TabBackWidget->setFixedHeight(h);
}

/***************************************************
  Author: RenDan
  Date: 2015-08-20
  Description: Set width of tab back widget
  Input: int
  Output: void
  Return: void
****************************************************/
void RTabWidget::SetTabWidth(const int &w)
{
    m_nTabWidth = w;
    QList<QAbstractButton*> btnList = m_TabBtnGroup->buttons();
    foreach (QAbstractButton *btn, btnList)
    {
        if(btn)
        {
            btn->setFixedHeight(m_nTabWidth);
        }
    }
}

/***************************************************
  Author: RenDan
  Date: 2015-08-20
  Description: Set height of tab back widget
  Input: int
  Output: void
  Return: void
****************************************************/
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

/***************************************************
  Author: RenDan
  Date: 2015-02-04
  Description: Set position of tab
  Input: RTabWidget::site
  Output: void
  Return: void
****************************************************/
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

/***************************************************
  Author: RenDan
  Date: 2015-02-04
  Description: Set tab enable or disable
  Input: int, bool
  Output: void
  Return: void
****************************************************/
void RTabWidget::SetTabEnable(const int &index, bool enabled)
{
    QAbstractButton *btn = m_TabBtnGroup->button(index);
    if(NULL != btn)
    {
        btn->setEnabled(enabled);
    }
}

/***************************************************
  Author: RenDan
  Date: 2013-05-10
  Description: Fix tab position
  Input: int, int
  Output: void
  Return: void
****************************************************/
void RTabWidget::SetTabMarginAndSpacing(const int &Margin, const int &Spacing)
{
    m_TabLay->setMargin(Margin);
    m_TabLay->setSpacing(Spacing);
}

/***************************************************
  Author: RenDan
  Date: 2015-07-23
  Description: Tab is need close btn or not
  Input: bool
  Output: void
  Return: void
****************************************************/
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

/***************************************************
  Author: RenDan
  Date: 2015-07-23
  Description: Tab‘s close btn size
  Input: const QSize &
  Output: void
  Return: void
****************************************************/
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

/***************************************************
  Author: RenDan
  Date: 2013-05-10
  Description: Add widget before tab
  Input: QWidget*
  Output: void
  Return: void
****************************************************/
void RTabWidget::InsertWidgetBeforeTab(QWidget *widget)
{
    m_TabLay->insertWidget(0, widget);
}

/***************************************************
  Author: RenDan
  Date: 2013-05-10
  Description: Add widget after tab
  Input: QWidget*
  Output: void
  Return: void
****************************************************/
void RTabWidget::InsertWidgetAfterTab(QWidget *widget)
{
    m_TabLay->addWidget(widget);
}

/***************************************************
  Author: RenDan
  Date: 2013-01-08
  Description: init widget
  Input: void
  Output: void
  Return: void
****************************************************/
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

/***************************************************
  Author: RenDan
  Date: 2013-01-08
  Description: Add a tab
  Input: QWidget*, QString, QString, QIcon
  Output: void
  Return: void
****************************************************/
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

/***************************************************
  Author: RenDan
  Date: 2015-07-24
  Description: Remove a tab by index(not delete the widget)
  Input: int
  Output: void
  Return: void
****************************************************/
void RTabWidget::RemoveTab(int index)
{
    QAbstractButton *pBtn = m_TabBtnGroup->button(index);
    m_TabLay->removeWidget(pBtn);
    m_TabBtnGroup->removeButton(pBtn);
    pBtn->deleteLater();
    QWidget *pWidget = Widget(index);
    m_PageWidgetStack->removeWidget(pWidget);
}

/***************************************************
  Author: RenDan
  Date: 2013-01-08
  Description: Get current index of tab
  Input: void
  Output: void
  Return: int
****************************************************/
int RTabWidget::CurrentTabPage()
{
    return m_PageWidgetStack->currentIndex();
}

/***************************************************
  Author: RenDan
  Date: 2015-02-05
  Description: Get widget of index
  Input: int
  Output: void
  Return: QWidget*
****************************************************/
QWidget *RTabWidget::Widget(const int &index)
{
    return ((m_PageWidgetStack->count() > index) ? m_PageWidgetStack->widget(index) : 0);
}

/***************************************************
  Author: RenDan
  Date: 2015-02-05
  Description: Get index of widget
  Input: QWidget*
  Output: void
  Return: int
****************************************************/
int RTabWidget::Index(QWidget *widget)
{
    return m_PageWidgetStack->indexOf(widget);
}

/***************************************************
  Author: RenDan
  Date: 2014-10-20
  Description: Change tab
  Input: int
  Output: void
  Return: void
****************************************************/
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

/***************************************************
  Author: RenDan
  Date: 2015-07-24
  Description: when click close button on tab
  Input: void
  Output: void
  Return: void
****************************************************/
void RTabWidget::OnCloseBtnClicked()
{
    QAbstractButton *pBtn = qobject_cast<QAbstractButton*>(sender());
    if(pBtn)
    {
        emit ToCloseBtnClicked(m_TabBtnGroup->id(pBtn));
    }
}
