#ifndef FREYAXMPPCLIENT_H
#define FREYAXMPPCLIENT_H

#include "freyabaseaction.h"
#include "QXmppClient.h"
#include "QXmppRegisterIq.h"
#include "QXmppRosterManager.h"
#include "QXmppVCardManager.h"
#include "QXmppVCardIq.h"
#include "QXmppMessage.h"
#include "QXmppPresence.h"
#include "QXmppLogger.h"

class FreyaXmppClient : public QXmppClient, public FreyaBaseAction
{
    Q_OBJECT
public:
    explicit FreyaXmppClient(QObject *parent = 0);
    ~FreyaXmppClient();

protected:
    virtual void Execute();
    virtual void Execute(const quint64 &command);
    virtual void Execute(FreyaBaseData*pData);

private:
    void SaveVcardToLocal(const QXmppVCardIq &vCardIq, const QString &bareJid);
    void SaveVcardToServer();

private slots:
    void OnClientStateChanged(QXmppClient::State state);
    void OnIqReceived(const QXmppIq &iq);
    void OnRosterReceived();
    void OnMessageReceived(const QXmppMessage &message);
    void OnPresenceReceived (const QXmppPresence &presence);
    void OnClientError (QXmppClient::Error error);
    void OnLoggerChanged(QXmppLogger *logger);
    void OnClientVCardReceived();
    void OnVCardReceived(const QXmppVCardIq &vCardIq);
    void OnSubscriptionReceived(const QString &bareJid);
};

#endif // FREYAXMPPCLIENT_H
