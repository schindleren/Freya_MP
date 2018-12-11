#include <QCoreApplication>
#include "freyaremote.h"
#include "freyabasecontrol.h"
#include "../FreyaLaunch/freya_global.h"

FreyaRemote::FreyaRemote() :
    QObject(NULL)
{
    qDebug() << "FreyaRemote > " << "Freya Version:" << FreyaBaseControl::GetFreyaControl()->FreyaVersion();
}

FreyaRemote::~FreyaRemote()
{
}

void FreyaRemote::ConnectToLaunch(int argc, char *argv[])
{
    m_argc = argc;
    m_argv = argv;
    m_FreyaPlugin = new FreyaBasePlugin(FRYLAC_COD_PLATFORMID, FreyaBaseControl::GetFreyaControl(), FRYRMT_OBJ_RMTPLUGIN);
    connect(m_FreyaPlugin, SIGNAL(ToPluginConnected(bool)), this, SLOT(OnPluginConnected(bool)), Qt::QueuedConnection);
}

void FreyaRemote::ShowHelp(const char *appName)
{
    fprintf(stderr, "Remote for application base FreyaLib`s launcher Ver:%s\n", FREYAREMOTE_VER);

    fprintf(stderr, "Usage: %s [options] \n\n"
                    "  h, help                 display this help and exit\n"
                    "  q, quit                 stop FreyaLaunch and exit\n"
                    "  e, expansion            expansion command and exit\n"
            "\n", appName);
}

void FreyaRemote::OnPluginConnected(bool isConnected)
{
    if(isConnected && m_FreyaPlugin->ImportPluginAuth(QStringList(),
                                                       QStringList()<<FRYLAC_COD_CMD1<<FRYLAC_COD_CMD2<<FRYLAC_COD_CMD4<<FRYLAC_COD_CMD8))
    {
        QTimer::singleShot(1000, this, SLOT(OnSendCommand()));
    }
    else
    {
        qDebug() << "FreyaRemote > " <<"Cannot get FreyaLaunch's message and command auth!";
    }
}

void FreyaRemote::OnSendCommand()
{
    int arg = 1;
    if (arg < m_argc)
    {
        QString opt = QString::fromLocal8Bit(m_argv[arg]);
        qDebug() << "FreyaRemote > "  << "Send command +++ " << opt << " +++ to FreyaLaunch";
        if (opt == QLatin1String("q") || opt == QLatin1String("quit"))
        {
            m_FreyaPlugin->PluginWrite(FRYLAC_CMD_REMOTEQUIT);
        }
        else
        {
            FreyaData pData = FreyaBaseData::CreateDate(FRYLAC_CMD_REMOTECMD);
            QStringList optList;
            for(; arg < m_argc; ++arg)
            {
                optList.append(QString::fromLocal8Bit(m_argv[arg]));
            }
            pData->SetArgument(optList);
            m_FreyaPlugin->PluginWrite(pData);
        }
    }
    else
    {
        ShowHelp("FreyaRemote");
    }

    m_FreyaPlugin->deleteLater();
    QTimer::singleShot(1000, qApp, SLOT(quit()));
}
