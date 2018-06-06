#ifndef FREYAREMOTE_H
#define FREYAREMOTE_H

#include <QString>
#include <QObject>
#include <QTimer>
#include "freyabaseplugin.h"

class FreyaRemote: public QObject
{
    Q_OBJECT
public:
    FreyaRemote();
    ~FreyaRemote();
    void ConnectToLaunch(int argc,char *argv[]);

    static void ShowHelp(const char *appName);

private slots:
    void OnPluginConnected(bool isConnected);
    void OnSendCommand();

private:
    int                     m_argc;
    char                    **m_argv;
    FreyaBasePlugin         *m_FreyaPlugin;
    bool                    m_ConnectState;
};

#endif // FREYAREMOTE_H
