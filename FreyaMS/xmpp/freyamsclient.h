#ifndef FREYAMSCLIENT_H
#define FREYAMSCLIENT_H

#include "freyabaseaction.h"
#include "QXmppClient.h"
#include "QXmppRegisterIq.h"
#include "QXmppRosterManager.h"
#include "QXmppMessage.h"
#include "QXmppPresence.h"
#include "QXmppLogger.h"
#include <QProcess>
#include <QSettings>
#include <QDataStream>
#include <QByteArray>

class FreyaMSClient : public QXmppClient, public FreyaBaseAction
{
    Q_OBJECT
public:
    explicit FreyaMSClient(QObject *parent = 0);
    ~FreyaMSClient();

protected:
    virtual void Execute();
    virtual void Execute(const quint64 &command);
    virtual void Execute(FreyaBaseData *pData);

private:

private slots:
    void OnClientStateChanged(QXmppClient::State state);
    void OnRosterReceived();
//    void OnDiscoveryInfoReceived(const QXmppDiscoveryIq &discoveryIq);
    void OnIqReceived(const QXmppIq &iq);
    void OnMessageReceived(const QXmppMessage &message);
    void OnPresenceReceived (const QXmppPresence &presence);
    void OnClientError (QXmppClient::Error error);
    void OnLoggerChanged(QXmppLogger *logger);
};

#endif // FREYAMSCLIENT_H
