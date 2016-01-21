#ifndef FREYABACKWINDOW_H
#define FREYABACKWINDOW_H

#include "freyawindow.h"
#include <QLabel>
#include <QToolButton>
#include <QBoxLayout>

class FreyaBackWindow : public FreyaWindow
{
    Q_OBJECT
public:
    explicit FreyaBackWindow(FreyaWindow *parent = 0, const char *objectName = NULL,
                             FreyaBaseControl *pControl = FreyaBaseControl::GetFreyaControl());
    void SetTitle(const QString &title){m_TitleLab->setText(title); setWindowTitle(title);}
    QString GetTitle(){return m_TitleLab->text();}
    void SetBackObjectName(const QString &objectName){m_BackWidget->setObjectName(objectName);}
    QString GetBackObjectName(){return m_BackWidget->objectName();}
    void needMinBtn(bool isNeed){if(!isNeed){m_MinSizeBtn->hide();}}
    void needMaxBtn(bool isNeed){if(!isNeed){m_MaxSizeBtn->hide();}}
    QWidget* MainBackWidget(){return m_MainWidget;}

private:
    void InitWidget();

private:
    QWidget             *m_BackWidget;
    QLabel              *m_LogoLab;
    QLabel              *m_TitleLab;
    QToolButton         *m_MinSizeBtn;
    QToolButton         *m_MaxSizeBtn;
    QToolButton         *m_CloseBtn;
    QWidget             *m_MainWidget;
};

#endif // FREYABACKWINDOW_H
