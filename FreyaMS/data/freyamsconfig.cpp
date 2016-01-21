#include "freyamsconfig.h"
#include "freyabasecontrol.h"
#include "QXmppConfiguration.h"
#include "freyacryptogram.h"

FreyaMSConfig::FreyaMSConfig(QObject *parent)
    : QObject(parent)
{
}

FreyaMSConfig::~FreyaMSConfig()
{
}

void FreyaMSConfig::InitConfig()
{
    InitGlobalData();
}

void FreyaMSConfig::InitGlobalData()
{
    QFile globalConfigFile(FREYAMS_PTH_GLOBALCONFIGPATH);
    if(!globalConfigFile.exists() || globalConfigFile.size() <= 0)
    {
        QVariantMap globalConfigMap;
        globalConfigMap.insert(FREYAMS_CFG_CONFIDSERVER,   FREYAMS_INF_DEFCONFSERVER);
        globalConfigMap.insert(FREYAMS_CFG_CONFIDPORT,     FREYAMS_INF_DEFCONFPORT);
        globalConfigMap.insert(FREYAMS_CFG_CONFIDDOMAIN,   FREYAMS_INF_DEFCONFDOMAIN);
        globalConfigMap.insert(FREYAMS_CFG_CONFIDRESOURCE, FREYAMS_INF_DEFCONFRESOURCE);
        QVariantMap defaultConfig;
        defaultConfig.insert(FREYAMS_CFG_CONFIDDEFAULT, globalConfigMap);
        FreyaBaseControl::GetFreyaControl()->SetConfigToFile(FREYAMS_PTH_GLOBALCONFIGPATH, defaultConfig);
    }
}

void FreyaMSConfig::InsertGlobalData(const QXmppConfiguration &config)
{
    QVariantMap globalConfigMap;
    globalConfigMap.insert(FREYAMS_CFG_CONFIDSERVER,   config.host());
    globalConfigMap.insert(FREYAMS_CFG_CONFIDPORT,     config.port());
    globalConfigMap.insert(FREYAMS_CFG_CONFIDDOMAIN,   config.domain());
    globalConfigMap.insert(FREYAMS_CFG_CONFIDRESOURCE, FREYAMS_INF_DEFCONFRESOURCE);
    globalConfigMap.insert(FREYAMS_CFG_CONFIDACCOUND,  config.jidBare().split("@").at(0));
    QVariantMap defaultConfig;
    defaultConfig.insert(FREYAMS_CFG_CONFIDDEFAULT, globalConfigMap);
    defaultConfig.insert(FREYAMS_CFG_CONFIDNONE, FreyaCryptogram::EnCryptogram(config.password()));
    FreyaBaseControl::GetFreyaControl()->SetConfigToFile(FREYAMS_PTH_GLOBALCONFIGPATH, defaultConfig);
}
