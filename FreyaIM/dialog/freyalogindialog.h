#ifndef FREYALOGIN_H
#define FREYALOGIN_H

#include "freyawindow.h"
#include "freyabasewidget.h"
#include "freyaim_global.h"
#include "QXmppClient.h"
#include <QMenu>
#include <QPushButton>
#include <QToolButton>
#include <QBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QApplication>
#include <QComboBox>
#include <QLineEdit>
#include <QMovie>
#include <QCheckBox>

class FreyaPortrait;
class FreyaStateBtn;
class FreyaLoginDialog : public FreyaWindow
{
    Q_OBJECT
public:
    explicit FreyaLoginDialog(FreyaWindow *parent = 0);

protected:
    void keyPressEvent(QKeyEvent *e);
    void closeEvent(QCloseEvent *e);
    virtual void Execute();
    virtual void Execute(const quint64 &command);
    virtual void Execute(FreyaBaseData *pData);

private:
    void InitWidget();
    void InitData();
    void SaveGlobalConfig();
    bool CheckLoginInfor();

private slots:
    void OnLoginIn();
    void OnLoginInCancel();
    void OnAccoundComboChanged(const QString &account);
    void OnRegisterAccound();

private:
    QLabel              *m_LoginPublishLab;
    FreyaBaseWidget     *m_LoginWidget;
    QToolButton         *m_LoginSetBtn;
    QToolButton         *m_LoginMinBtn;
    QToolButton         *m_LoginCloseBtn;

    FreyaPortrait       *m_LoginPortrait;
    QComboBox           *m_LoginAccountCombo;
    QLineEdit           *m_LoginPasswordLEdit;
    FreyaStateBtn       *m_LoginStateBtn;
    QCheckBox           *m_LoginKeepCheck;
    QToolButton         *m_LoginRegisterBtn;
    QToolButton         *m_LoginFindPSWBtn;
    QToolButton         *m_LoginInBtn;

    FreyaBaseWidget     *m_LoginSetWidget;
    QLineEdit           *m_LoginHostLEdit;
    QLineEdit           *m_LoginPortLEdit;
    QLineEdit           *m_LoginDomainLEdit;
    QLineEdit           *m_LoginResourceLEdit;

    QLabel              *m_LoginTipsLab;
    QMovie              *m_LoginMovie;
    QToolButton         *m_LoginInCancelBtn;
};

#endif // FREYALOGIN_H
