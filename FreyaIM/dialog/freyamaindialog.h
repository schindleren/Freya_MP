#ifndef FREYAMAINDIALOG_H
#define FREYAMAINDIALOG_H

#include "freyawindow.h"
#include "QXmppClient.h"
#include "QXmppVCardIq.h"
#include "rtabwidget.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>
#include <QApplication>
#include <QSystemTrayIcon>
#include <QCryptographicHash>

class FreyaPortrait;
class FreyaContactTree;
class FreyaStateBtn;
class FreyaMainDialog : public FreyaWindow
{
    Q_OBJECT
public:
    explicit FreyaMainDialog(FreyaWindow *parent = 0);

protected:
    virtual void Execute();
    virtual void Execute(const quint64 &command);
    virtual void Execute(FreyaBaseData *pData);

    void resizeEvent(QResizeEvent *e);
    void moveEvent(QMoveEvent *e);

private:
    void InitWidget();
    void InitConfig(QXmppConfiguration *xmppConfig);
    void LoadLocalConfig();
    void LoadRoster();
    void PresenceMessage(const QXmppPresence &Presence);

private slots:
    void OnShowIDCard();
    void OnCliendStateChanged(int);

private:
    QWidget             *m_BackgroundWidget;
    QLabel              *m_TitleLab;
    QToolButton         *m_MinBtn;
    QToolButton         *m_CloseBtn;
    FreyaPortrait       *m_MainPortrait;
    QLabel              *m_NickNameLab;
    FreyaStateBtn       *m_ChangeStateBtn;
    QLineEdit           *m_DescriptionLEdit;
    QLineEdit           *m_ContactSearchLEdit;
    RTabWidget          *m_ContactTWidget;
    FreyaContactTree    *m_ContactTree;
    QToolButton         *m_MainMenuBtn;
    QString             m_BareJid;
};

#endif // FREYAMAINDIALOG_H
