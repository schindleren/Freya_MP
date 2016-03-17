#ifndef FREYABASEEXTENSION_H
#define FREYABASEEXTENSION_H

#include "freyabaseaction.h"
#include "freyabasecontrol.h"
#include "freyacryptogram.h"
#include "freyalib_global.h"

#include <QLocalServer>
#include <QLocalSocket>

/*
 * Types of support
 * (See document about Qt5.4 - QJsonValue::fromVariant)
 */

class FreyaPluginPusher :public QLocalSocket
{
    Q_OBJECT
public:
    explicit FreyaPluginPusher(QString PluginID, FreyaBaseControl *pControl = NULL, QObject *parent = 0);

    void PusherExcute(const quint64 &command);
    void PusherExcute(FreyaBaseData *pData);

signals:
    void ToDisconnected();

private slots:
    void OnReadyRead();
    void OnStateChanged(QLocalSocket::LocalSocketState state);

private:
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
    void DefineAuthCode(const QStringList &MsgAuth, const QStringList &CmdAuth);

protected:
    virtual void Execute(const quint64 &command);
    virtual void Execute(FreyaBaseData *pData);

private slots:
    void OnPluginRequest();
    void OnReadyRead();
    void OnPusherDisconnected();

private:
    QList<FreyaPluginPusher*>           m_PusherList;
    QString                             m_CurrentPluginID;
};

#endif // FREYABASEEXTENSION_H
