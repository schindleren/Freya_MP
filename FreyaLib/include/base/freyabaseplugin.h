#ifndef FREYABASEPLUGIN_H
#define FREYABASEPLUGIN_H

#include "freyabaseaction.h"
#include "freyapublicregister.h"
#include "freyacryptogram.h"
#include "freyabasecontrol.h"
#include "freyalib_global.h"

#include <QLocalSocket>
#include <QLocalServer>

class FREYALIBSHARED_EXPORT FreyaBasePlugin : public QLocalSocket, public FreyaBaseAction
{
    Q_OBJECT
public:
    explicit FreyaBasePlugin(QString PlatformID, FreyaBaseControl *pControl = NULL, const char *objectName = NULL);
    bool PluginConnected();
    bool ImportPluginAuth(const QStringList &MsgCode, const QStringList &CmdCode);

protected:
    virtual void Execute(const quint64 &command);
    virtual void Execute(FreyaBaseData *pData);

signals:
    void ToPluginConnected(bool);

private slots:
    void OnReadyRead();
    void OnPusherConnected();
    void OnPluginReadyRead();

private:
    QLocalServer                *m_PluginServer;
    QLocalSocket                *m_Pusher;
};

#endif // FREYABASEPLUGIN_H
