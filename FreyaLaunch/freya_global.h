#ifndef FREYA_GLOBAL_H
#define FREYA_GLOBAL_H

#include "freyabaseactionex.h"
#include "freyabasecontrol.h"

#define MODULEDIR                   "./module"
#define MODULEDEPENDDIR             "./module/depend"
#define MODULECONFIGDIR             "./module/config"

/// command
#define FRYLAC_CMD_MODULELAUNCH     0x0000000001000000      // FreyaLaunch module launch signal
#define FRYLAC_CMD_MODULECMD        0x0000000002000000      // command form module
#define FRYLAC_CMD_REMOTECMD        0x0000000003000080      // command form remote
#define FRYLAC_CMD_REMOTEQUIT       0x0000000004000088      // quit command form remote.FreyaLaunch will quit after 1 second.
#define FRYLAC_CMD_MODULEQUIT       0x000000000500000f      // FreyaLaunch module quit signal

/// object name
#define FRYLAC_OBJ_LAUNCHER         "FreyaLaunch_obj_Launch"
#define FRYLAC_OBJ_PLUGINEXT        "FreyaLaunch_obj_PluginExtension"
#define FRYRMT_OBJ_RMTPLUGIN        "FreyaRemote_obj_RemotePlugin"

/// InfoKey
#define FRYLAC_STR_LAUNCHKEY        "LaunchKey"
#define FRYLAC_STR_MODULENAME       "ModuleName"
#define FRYLAC_STR_MODULEVER        "ModuleVerison"

/// plugin code
#define FRYLAC_COD_PLATFORMID       "{d8396237-87d4-4a7e-be93-4576ae5f09f2}"
#define FRYLAC_COD_MSG1             "{1eb4b443-8de7-4426-87ae-e9cc52df49ae}"
#define FRYLAC_COD_MSG2             "{0d80cc63-9527-4355-b3a1-a9105881ac0e}"
#define FRYLAC_COD_MSG4             "{7a8c2bf4-5763-4bdf-b214-01f518c65778}"
#define FRYLAC_COD_MSG8             "{511db26e-9c36-4ba1-993e-9cbecf93335b}"
#define FRYLAC_COD_CMD1             "{9efddce7-30de-4eb6-a67b-eabb9e7328fa}"
#define FRYLAC_COD_CMD2             "{e337ddf8-41ff-4699-ab85-9797ca2e75da}"
#define FRYLAC_COD_CMD4             "{9c6ef1cb-56d5-4655-9503-4a7f79878acb}"
#define FRYLAC_COD_CMD8             "{791e0949-4017-4562-a213-c69b33895866}"

#define FRYLAC_FUN_LAUNCHPORT(ClassName, EXPORT_FLAG, FreyaLaunchInfo)    ClassName *pLaunchApp = NULL; \
                                            extern "C" EXPORT_FLAG const char * FreyaModuleInfor() \
                                            { return FreyaLaunchInfo; } \
                                            extern "C" EXPORT_FLAG bool FreyaModulePreLaunch(FreyaBaseControl *pLaunchControl) \
                                            { return (FreyaBaseControl::GetFreyaControl(pLaunchControl) && (pLaunchApp = new ClassName())); }

#endif // FREYA_GLOBAL_H
