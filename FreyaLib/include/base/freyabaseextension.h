#ifndef FREYABASEEXTENSION_H
#define FREYABASEEXTENSION_H

#include "freyabaseaction.h"
#include "freyabasecontrol.h"
#include "freyacryptogram.h"
#include "freyalib_global.h"

#include <QLocalServer>
#include <QLocalSocket>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QSharedPointer>

/*
 * Types of support
 * (See document about Qt5.4 - QJsonValue::fromVariant)
 */

class FreyaPluginPusher : public QObject
{
    Q_OBJECT
public:
    explicit FreyaPluginPusher(QString PluginID, FreyaBaseControl *pControl = NULL, QObject *parent = 0);

    void PusherExecute(const FreyaData data);

    QString &FreyaPluginID();

signals:
    void ToPluginRequest(const FreyaData pData);
    void ToDisconnected();

private slots:
    void OnReadyRead();
    void OnStateChanged(QLocalSocket::LocalSocketState state);

private:
    QLocalSocket                        *m_Pusher;
    int                                 m_MsgAuth;
    int                                 m_CmdAuth;
    QString                             m_PluginID;
    FreyaBaseControl                    *m_FreyaControl;
};

class FREYALIBSHARED_EXPORT FreyaBaseExtension : public QLocalServer, public FreyaBaseAction
{
    Q_OBJECT
public:
    explicit FreyaBaseExtension(QString PlatformID, FreyaBaseControl *pControl = NULL, const char *objectName = NULL);
    ~FreyaBaseExtension();
    bool DefineAuthCode(const QStringList &MsgAuth, const QStringList &CmdAuth);

protected:
    virtual void Execute(const FreyaData data);

private slots:
    void OnPluginRequest();
    void OnReadyRead();
    void OnPusherDisconnected();
    void OnPuserRequest(const FreyaData data);

private:
    QList<FreyaPluginPusher*>           m_PusherList;
    QStringList                         m_WaitPluginIDList;
    FreyaBaseControl                    *m_FreyaControl;
    bool                                m_isListening;
    QMutex                              m_PluginListMutex;
    QThread                             *m_Thread;
};

#endif // FREYABASEEXTENSION_H
