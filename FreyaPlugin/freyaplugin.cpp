#include "freyaplugin.h"

FreyaPlugin::FreyaPlugin(QWidget *parent)
    : QTextEdit(parent), FreyaBaseAction(FreyaBaseControl::GetFreyaControl(), "FreyaPlugin")
{
    m_plugin = new FreyaBasePlugin(FYTS_ID_EXT, FreyaBaseControl::GetFreyaControl(), "Plugin");
    connect(m_plugin, SIGNAL(ToPluginConnected(bool)), this, SLOT(OnPluginConnected()));
}

FreyaPlugin::~FreyaPlugin()
{
}

void FreyaPlugin::Execute(const quint64 &command)
{
    switch (command)
    {
    case FYTS_CMD_QUIT:
        append("FreyaTest is sample Quit");
        break;
    case FYTS_CMD_HAPPY:
        append("FreyaTest is sample Happy");
        break;
    case FYTS_CMD_SAD:
        append("FreyaTest is sample Sad");
        break;
    case FYTS_CMD_CRY:
        append("FreyaTest is sample cry");
        break;
    default:
        break;
    }
}

void FreyaPlugin::Execute(const FreyaData data)
{
    quint64 cmd64 = data->command;
    if(cmd64 == FYTS_CMD_QUIT)
    {
        append("FreyaTest is Quit");
        append(data->dataID);
        append(data->arguments.toString());
//        m_plugin->PluginWrite(FYTS_CMD_QUIT);
    }
    else if(cmd64 == FYTS_CMD_HAPPY)
    {
        append("FreyaTest is Happy");
        append(data->dataID);
        append(data->arguments.toString());
//        m_plugin->PluginWrite(FYTS_CMD_HAPPY);
    }
    else if(cmd64 == FYTS_CMD_SAD)
    {
        append("FreyaTest is Sad");
        append(data->dataID);
        QVariantMap varMap = data->arguments.toMap();
        append("    T_1: " + varMap.value("T_1").toString());
        append("    T_2: " + varMap.value("T_2").toString());
        append("    T_3: " + QString::number(varMap.value("T_3").toInt()));

//        m_plugin->PluginWrite(data);
    }
    else if(cmd64 == FYTS_CMD_CRY)
    {
        append("FreyaTest is Cry");
        append(data->dataID);
        append(data->arguments.toString());

//        m_plugin->PluginWrite(data);
    }
}

void FreyaPlugin::OnPluginConnected()
{
    m_plugin->ImportPluginAuth(QStringList() << FYTS_AUTH_OUT_1 << FYTS_AUTH_OUT_4,
                               QStringList() << FYTS_AUTH_IN_1 << FYTS_AUTH_IN_2);
}
