#include "freyaconfigdata.h"
#include "freyabasecontrol.h"
#include "QXmppPresence.h"

FreyaConfigData::FreyaConfigData(QObject *parent)
    : QObject(parent)
{
}

FreyaConfigData::~FreyaConfigData()
{
}

void FreyaConfigData::InitConfig()
{
    InitGlobalData();
}

void FreyaConfigData::InitGlobalData()
{
    QFile globalConfigFile(FREYA_PTH_GLOBALCONFIGPATH);
    if(!globalConfigFile.exists() || globalConfigFile.size() <= 0)
    {
        QStringList usersConfigList;
        QVariantMap loginConfigState;
        loginConfigState.insert(tr("Online"),       QXmppPresence::Online);
        loginConfigState.insert(tr("Chat"),         QXmppPresence::Chat);
        loginConfigState.insert(tr("Away"),         QXmppPresence::Away);
        loginConfigState.insert(tr("XA"),           QXmppPresence::XA);
        loginConfigState.insert(tr("DND"),          QXmppPresence::DND);
        loginConfigState.insert(tr("Invisible"),    QXmppPresence::Invisible);
        QVariantMap globalConfigMap;
        globalConfigMap.insert(FREYA_CFG_CONFIDSERVER,   FREYA_INF_DEFCONFSERVER);
        globalConfigMap.insert(FREYA_CFG_CONFIDPORT,     FREYA_INF_DEFCONFPORT);
        globalConfigMap.insert(FREYA_CFG_CONFIDDOMAIN,   FREYA_INF_DEFCONFDOMAIN);
        globalConfigMap.insert(FREYA_CFG_CONFIDRESOURCE, FREYA_INF_DEFCONFRESOURCE);
        globalConfigMap.insert(FREYA_CFG_CONFIDUSERS,    usersConfigList);
        globalConfigMap.insert(FREYA_CFG_CONFIDSTATES,   loginConfigState);
        QVariantMap defaultConfig;
        defaultConfig.insert(FREYA_CFG_CONFIDDEFAULT, globalConfigMap);
        FreyaBaseControl::GetFreyaControl()->SetConfigToFile(FREYA_PTH_GLOBALCONFIGPATH, defaultConfig);
    }
}
