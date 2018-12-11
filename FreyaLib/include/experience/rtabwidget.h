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
 007     Ren Dan       2018-12-08   Add tab text direction
*******************************************************************************/
#ifndef RTABWIDGET_H
#define RTABWIDGET_H

#include "freyalib_global.h"
#include <QWidget>
#include <QBoxLayout>
#include <QStackedWidget>
#include <QButtonGroup>
#include <QToolButton>
#include <QIcon>

namespace RTabSite
{
    enum site
    {
        eSiteLeft = 0,
        eSiteTop = 1,
        eSiteRight = 2,
        eSiteBottom = 3
    };

    enum direction
    {
        eHorizontal = 0,
        eVertical = 1,
    };
}

class RTabButton : public QToolButton
{
    Q_OBJECT

public:
    explicit RTabButton(QWidget *parent = 0);

/***************************************************
  Author: RenDan
  Date: 2015-07-23
  Description: Set tab close btn size
  Input: QSize
  Output: void
  Return: void
****************************************************/
    void SetCloseBtnSize(const QSize &size);

/***************************************************
  Author: RenDan
  Date: 2015-07-23
  Description: Set tab close able
  Input: bool
  Output: void
  Return: void
****************************************************/
    void SetCloseable(bool closeable);

signals:
    void ToCloseBtnClicked();

private:
    QToolButton         *m_CloseBtn;
};

class FREYALIBSHARED_EXPORT RTabWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RTabWidget(QWidget *parent = 0, bool isExpand = false);

/***************************************************
  Author: RenDan
  Date: 2013-01-08
  Description: Set object name
  Input: QString
  Output: void
  Return: void
****************************************************/
    void SetTabBackObjectName(const QString &ObjName);

/***************************************************
  Author: RenDan
  Date: 2013-05-10
  Description: Set tab background size
  Input: int
  Output: void
  Return: void
****************************************************/
    void SetTabBKWidth(const int &w);
    void SetTabBKHeight(const int &h);

/***************************************************
  Author: RenDan
  Date: 2015-08-20
  Description: Set width of tab
  Input: int
  Output: void
  Return: void
****************************************************/
    void SetTabWidth(const int &w);
    int TabWidth(){return m_nTabWidth;}

/***************************************************
  Author: RenDan
  Date: 2015-08-20
  Description: Set height of tab
  Input: int
  Output: void
  Return: void
****************************************************/
    void SetTabHeight(const int &h);
    int TabHeight(){return m_nTabHeight;}

/***************************************************
  Author: RenDan
  Date: 2015-02-04
  Description: Set position of tab
  Input: RTabWidget::site
  Output: void
  Return: void
****************************************************/
    void SetTabSite(const RTabSite::site &site);

/***************************************************
  Author: RenDan
  Date: 2018-12-08
  Description: Set direction of tab text
  Input: RTabWidget::direction
  Output: void
  Return: void
****************************************************/
    void SetTabDirection(const RTabSite::direction &direction);

/***************************************************
  Author: RenDan
  Date: 2015-02-04
  Description: Set tab enable or disable
  Input: int, bool
  Output: void
  Return: void
****************************************************/
    void SetTabEnable(const int &index, bool enabled);

/***************************************************
  Author: RenDan
  Date: 2013-05-10
  Description: Fix tab position
  Input: int, int
  Output: void
  Return: void
****************************************************/
    void SetTabMarginAndSpacing(const int &Margin, const int &Spacing);

/***************************************************
  Author: RenDan
  Date: 2015-07-23
  Description: Tab is need close btn or not
  Input: bool
  Output: void
  Return: void
****************************************************/
    void SetTabsCloseable(bool closeable);

/***************************************************
  Author: RenDan
  Date: 2015-07-23
  Description: Tab`s close btn size
  Input: const QSize &
  Output: void
  Return: void
****************************************************/
    void SetTabsCloseBtnSize(const QSize &size);

/***************************************************
  Author: RenDan
  Date: 2013-05-10
  Description: Add widget before tab
  Input: QWidget*
  Output: void
  Return: void
****************************************************/
    void InsertWidgetBeforeTab(QWidget *widget);

/***************************************************
  Author: RenDan
  Date: 2013-05-10
  Description: Add widget after tab
  Input: QWidget*
  Output: void
  Return: void
****************************************************/
    void InsertWidgetAfterTab(QWidget *widget);

/***************************************************
  Author: RenDan
  Date: 2013-01-08
  Description: Add a tab
  Input: QWidget*, QString, QString, QIcon
  Output: void
  Return: void
****************************************************/
    void AddTabAndWidget(QWidget* tabWidget, const QString &objName = "", const QString &tabTitle = "", const QIcon &tabIcon = QIcon());

/***************************************************
  Author: RenDan
  Date: 2015-07-24
  Description: Remove a tab by index(not delete the widget)
  Input: int
  Output: void
  Return: void
****************************************************/
    void RemoveTab(int index);

/***************************************************
  Author: RenDan
  Date: 2013-01-08
  Description: Get current index of tab
  Input: void
  Output: void
  Return: int
****************************************************/
    int CurrentTabPage();

/***************************************************
  Author: RenDan
  Date: 2015-02-05
  Description: Get widget of index
  Input: int
  Output: void
  Return: QWidget*
****************************************************/
    QWidget *Widget(const int &index);

/***************************************************
  Author: RenDan
  Date: 2015-02-05
  Description: Get index of widget
  Input: QWidget*
  Output: void
  Return: int
****************************************************/
    int Index(QWidget *widget);

private:

/***************************************************
  Author: RenDan
  Date: 2013-01-08
  Description: init widget
  Input: void
  Output: void
  Return: void
****************************************************/
    void InitWidget();

signals:
    void ToTabChanged(int);
    void ToCloseBtnClicked(int);

public slots:

/***************************************************
  Author: RenDan
  Date: 2014-10-20
  Description: Change tab
  Input: int
  Output: void
  Return: void
****************************************************/
    void OnCurrentTab(int);

private slots:

/***************************************************
  Author: RenDan
  Date: 2015-07-24
  Description: when click close button on tab
  Input: void
  Output: void
  Return: void
****************************************************/
    void OnCloseBtnClicked();

private:
    bool                    m_isExpand;
    int                     m_nTabBKWidth;
    int                     m_nTabBKHeight;
    int                     m_nTabWidth;
    int                     m_nTabHeight;

    QWidget                 *m_TabBackWidget;
    QBoxLayout              *m_TabWidgetBkLay;
    QBoxLayout              *m_TabLay;
    QHBoxLayout             *m_PageLay;
    QStackedWidget          *m_PageWidgetStack;
    QButtonGroup            *m_TabBtnGroup;

    bool                    m_isTabCloseable;
    QSize                   m_CloseBtnSize;

    RTabSite::direction     m_TabTextDirection;
};

#endif // RTABWIDGET_H
