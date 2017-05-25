#ifndef FREYABASEEXTENSION_H
#define FREYABASEEXTENSION_H

#include "freyabaseaction.h"
#include "freyabasecontrol.h"
#include "freyacryptogram.h"
#include "freyalib_global.h"

#include <QLocalServer>
#include <QLocalSocket>
#include <QThread>

/*
 * Types of support
 * (See document about Qt5.4 - QJsonValue::fromVariant)
 */

class FreyaPluginPusher : public QThread
{
    Q_OBJECT
public:
    explicit FreyaPluginPusher(QString PluginID, FreyaBaseControl *pControl = NULL, QObject *parent = 0);

    void PusherExcute(const quint64 &command);
    void PusherExcute(const FreyaBaseData &data);

protected:
    virtual void run();

signals:
    void ToDisconnected();
    void ToPluginRequest(FreyaBaseData pData);

private slots:
    void OnReadyRead();
    void OnStateChanged(QLocalSocket::LocalSocketState state);

private:
    QLocalSocket                        *m_Pusher;
    int                                 m_MsgAuth;
    int                                 m_CmdAuth;
    QString                             m_PluginID;
    FreyaBaseControl                    *m_FreyaControl;
    QStringList                         m_DataIDList;
};

class FREYALIBSHARED_EXPORT FreyaBaseExtension : public QLocalServer, public FreyaBaseAction
{
    Q_OBJECT
public:
    explicit FreyaBaseExtension(QString PlatformID, FreyaBaseControl *pControl = NULL, const char *objectName = NULL);
    void DefineAuthCode(const QStringList &MsgAuth, const QStringList &CmdAuth);

protected:
    virtual void Execute(const quint64 &command);
    virtual void Execute(const FreyaBaseData &data);

private slots:
    void OnPluginRequest();
    void OnReadyRead();
    void OnPusherDisconnected();
    void OnPuserRequest(FreyaBaseData data);

private:
    QList<FreyaPluginPusher*>           m_PusherList;
    QString                             m_CurrentPluginID;
    FreyaBaseControl                    *m_FreyaControl;
};

#endif // FREYABASEEXTENSION_H
