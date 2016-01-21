#ifndef FREYADEFAULTCLIENT_H
#define FREYADEFAULTCLIENT_H

#include "freyabaseaction.h"
#include "QXmppClient.h"
#include "QXmppRegisterIq.h"
#include "QXmppRosterManager.h"
#include "QXmppMessage.h"
#include "QXmppPresence.h"
#include "QXmppLogger.h"
#include <QProcess>
#include <QSettings>

class FreyaDefaultCMD;
class FreyaScreenCMD;
class FreyaDefaultClient : public QXmppClient, public FreyaBaseAction
{
    Q_OBJECT
public:
    explicit FreyaDefaultClient(QObject *parent = 0);
    ~FreyaDefaultClient();

protected:
    virtual void Execute();
    virtual void Execute(const quint64 &command);
    virtual void Execute(FreyaBaseData *pData);

private:
    void initStaticData();
    QString figoutDiskID();

private slots:
    void OnClientStateChanged(QXmppClient::State state);
    void OnRosterReceived();
//    void OnDiscoveryInfoReceived(const QXmppDiscoveryIq &discoveryIq);
    void OnIqReceived(const QXmppIq &iq);
    void OnMessageReceived(const QXmppMessage &message);
    void OnPresenceReceived (const QXmppPresence &presence);
    void OnClientError (QXmppClient::Error error);
    void OnLoggerChanged(QXmppLogger *logger);

    void OnCMDReceive(const QVariantMap &output);

private:
    QString                     m_FreyaMSJid;
    FreyaDefaultCMD             *m_FreyaDefaultCMD;
    FreyaScreenCMD              *m_FreyaScreenCMD;
};

#endif // FREYADEFAULTCLIENT_H

/*
QString ComputerInfo::SystemInfo()
{
    QSysInfo sysinfo;
    //QHostInfo hostinfo = QHostInfo::fromName(QHostInfo::localHostName());

    QString infoName;
    infoName += "HostName:" + QHostInfo::localHostName() +
             + "\r\nCPU当前架构:" + sysinfo.currentCpuArchitecture()
             + "\r\nCPU实际架构:" + sysinfo.buildCpuArchitecture()
             + "\r\nProductName:" + sysinfo.prettyProductName()
             + "\r\nBuildAbi:" + sysinfo.buildAbi()
             + "\r\nProductVersion:" + sysinfo.productVersion()
             + "\r\nProductName:" + sysinfo.productType();
    if(sysinfo.windowsVersion() == QSysInfo::WV_6_3) //win8.1
    {
        infoName += "\r\nSystemVersion:" + QString("Win8.1");
    }
    else if(sysinfo.windowsVersion() == QSysInfo::WV_6_2) //win8
    {
        infoName += "\r\nSystemVersion:" + QString("Win8");
    }
    else if(sysinfo.windowsVersion() == QSysInfo::WV_6_1) //win7
    {
        infoName += "\r\nSystemVersion:" + QString("Win7");
    }
    else if(sysinfo.windowsVersion() == QSysInfo::WV_6_0) //winVista
    {
        infoName += "\r\nSystemVersion:" + QString("WinVista");
    }
    else
    {
        infoName += "\r\nSystemVersion:" + QString("Other");
    }
    return infoName;
}

QString ComputerInfo::NetWorkInfo()
{
//    QHostInfo hostinfo(QHostInfo::fromName(QHostInfo::localHostName()));
//    QList<QHostAddress> hostAddress = hostinfo.addresses();

    QString netInfo = "";
    foreach(QNetworkInterface interfacetmp, QNetworkInterface::allInterfaces())
    {
        netInfo += "-------------------------\nDevice:" + interfacetmp.name() + "\n" +
                "Hardware Address:" + interfacetmp.hardwareAddress() + "\n";
        foreach(QNetworkAddressEntry addressEntry, interfacetmp.addressEntries())
        {
            if(addressEntry.ip().protocol() == QAbstractSocket::IPv4Protocol)
            {
                netInfo += "IPv4 Address:" + addressEntry.ip().toString()+ "\n";
            }
            else if(addressEntry.ip().protocol() == QAbstractSocket::IPv6Protocol)
            {
                netInfo += "IPv6 Address:" + addressEntry.ip().toString()+ "\n";
            }
            else
            {}
            netInfo += "Broadcast:" + addressEntry.broadcast().toString() + "\n" +
                "Mask:" + addressEntry.netmask().toString() + "\n===\n";
        }
    }

    return netInfo;
}
 */
