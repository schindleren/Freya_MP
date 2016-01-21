#ifndef FREYAIM_H
#define FREYAIM_H

#include "freyaconfigdata.h"
#include "freyabaseaction.h"
#include "freyawindow.h"
#include "freyabaseplugin.h"
#include <QFile>
#include <QObject>
#include <QApplication>

class FreyaLoginDialog;
class FreyaDefaultClient;
class FreyaXmppClient;
class FreyaMainDialog;
class FreyaSystemTrayIcon;
class FreyaIM : public QObject, public FreyaBaseAction
{
    Q_OBJECT

public:
    FreyaIM(QObject *parent = 0);
    ~FreyaIM();
    void InitApplication();

protected:
    virtual void Execute(const quint64 &command);
    virtual void Execute(FreyaBaseData *pData);

private:
    void InitObject();
    void InitWidget();
    void LoadStyle(const QString &filepath);

private slots:
    void OnPluginConnected(bool isConnected);

private:
    FreyaLoginDialog    *m_FreyaLoginDialog;
    FreyaXmppClient     *m_FreyaIMClient;
    FreyaDefaultClient  *m_FreyaDefaultClient;
    FreyaMainDialog     *m_FreyaMainDialog;
    FreyaSystemTrayIcon *m_FreyaSystemTrayIcon;
    FreyaBasePlugin     *m_Plugin;
};

#endif // FREYAIM_H
