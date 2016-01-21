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
}

class RTabButton : public QToolButton
{
    Q_OBJECT

public:
    explicit RTabButton(QWidget *parent = 0);
    void SetCloseBtnSize(const QSize &size);
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
    void SetTabBackObjectName(const QString &ObjName);
    void SetTabBKWidth(const int &w);
    void SetTabBKHeight(const int &h);
    void SetTabWidth(const int &w);
    void SetTabHeight(const int &h);
    void SetTabSite(const RTabSite::site &site);
    void SetTabEnable(const int &index, bool enabled);
    void SetTabMarginAndSpacing(const int &Margin, const int &Spacing);
    void SetTabsCloseable(bool closeable);
    void SetTabsCloseBtnSize(const QSize &size);
    void InsertWidgetBeforeTab(QWidget *widget);
    void InsertWidgetAfterTab(QWidget *widget);
    void AddTabAndWidget(QWidget* tabWidget, const QString &objName = "", const QString &tabTitle = "", const QIcon &tabIcon = QIcon());
    void RemoveTab(int index);
    int CurrentTabPage();
    QWidget *Widget(const int &index);
    int Index(QWidget *widget);

private:
    void InitWidget();

signals:
    void ToTabChanged(int);
    void ToCloseBtnClicked(int);

public slots:
    void OnCurrentTab(int);

private slots:
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
};

#endif // RTABWIDGET_H
