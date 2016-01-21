#ifndef FREYAMS_GLOBAL_H
#define FREYAMS_GLOBAL_H

#include <QStandardPaths>
#include <QCryptographicHash>
#include <QRect>
#include <QColor>

struct FreyaInfor
{
    QString text;
    QColor  color;
    int     timer;
    FreyaInfor()
    {
        text = "";
        color = Qt::black;
        timer = 10000;
    }
};

//Global path
#define FREYAMS_PTH_GLOBALCONFIGPATH                "./FreyaMSConf/globalconfig.json"
#define FREYAMS_PTH_STATICCONFIGPATH                "./FreyaMSConf/staticconfig.ini"
#define FREYAMS_PTH_GLOBALRESPATH                   "./FreyaMSRes/"
#define FREYAMS_PTH_PERSONCONFIGPATH                "freyaconfig.json"
#define FREYAMS_PTH_DEFPORTRAITPATH                 "freyaportrait.png"
#define FREYAMS_PTH_DEFAULTSTYLEPATH                "./FreyaMSRes/freyamsstyle.qss"
#define FREYAMS_PTH_DEFAULTTRANSPATH                "./FreyaMSRes/freyamstrans.qm"
#define FREYAMS_PTH_LOGINGIFPATH                    "./FreyaMSRes/loading.gif"
#define FREUAMS_PTH_IMICON                          ":/icon/freyaico"

//Global config name
#define FREYAMS_CFG_CONFIDDEFAULT                   "FreyaCfg_Default"
#define FREYAMS_CFG_CONFIDSERVER                    "FreyaCfg_Server"
#define FREYAMS_CFG_CONFIDPORT                      "FreyaCfg_Port"
#define FREYAMS_CFG_CONFIDDOMAIN                    "FreyaCfg_Domain"
#define FREYAMS_CFG_CONFIDRESOURCE                  "FreyaCfg_Resource"
#define FREYAMS_CFG_CONFIDACCOUND                   "FreyaCfg_Accound"
#define FREYAMS_CFG_CONFIDNONE                      "FreyaCfg_None"

//Default config information
#define FREYAMS_INF_DEFCONFSERVER                   "127.0.0.1"         //"jesschindl.vicp.net"
#define FREYAMS_INF_DEFCONFPORT                     "5222"              //"54056"
#define FREYAMS_INF_DEFCONFDOMAIN                   "schindland"
#define FREYAMS_INF_DEFCONFRESOURCE                 "FreyaMS"

//Define
#define FREYAMS_DEF_DATEFORMAT                      "yyyy-MM-dd"
#define FREYAMS_DEF_TIMEFORMAT                      "hh:mm:ss"

//Object register name
#define FREYAMS_OBJ_FREYAMS                         "FreyaMSObj_FreyaMS"
#define FREYAMS_OBJ_MSDIALOG                        "FreyaMSObj_MSDialog"
#define FREYAMS_OBJ_LOGINWIDGET                     "FreyaMSObj_LoginWidget"
#define FREYAMS_OBJ_MSWIDGET                        "FreyaMSObj_MSWidget"
#define FREYAMS_OBJ_XMPPCLIENT                      "FreyaMSObj_xmppClient"
#define FREYAMS_OBJ_MSFUNTAB                        "FreyaMSObj_MSFunTab"
#define FREYAMS_OBJ_MSCMDWIDGET                     "FreyaMSObj_MSCMDWidget"

//Command
#define FREYAMS_CMD_LOGINDEFAULT                    0x0001000000000000//"FreyaMSCmd_LoginDefault"
#define FREYAMS_CMD_CONNECTEDSTATE                  0x0002000000000000//"FreyaMSCmd_ConnectedState"
#define FREYAMS_CMD_LOGINCANCEL                     0x0003000000000000//"FreyaMSCmd_LoginCancel"
#define FREYAMS_CMD_LOCKED                          0x0004000000000000//"FreyaMSCmd_Locked"
#define FREYAMS_CMD_SHOWINFOR                       0x0005000000000000//"FreyaMSCmd_ShowInformation"
#define FREYAMS_CMD_PRESENCE                        0x0006000000000000//"FreyaMSCmd_Presence"
#define FREYAMS_CMD_EXCUTECMD                       0x0007000000000000//"FreyaMSCmd_ExcuteCmd"
#define FREYAMS_CMD_RECEIVECMD                      0x0008000000000000//"FreyaMSCmd_ReceiveCmd"

//Type
#define FREYAMS_STR_RESOURCENAME                    "FreyaMSTyp_ResourceName"
#define FREYAMS_STR_COMMANDPATH                     "FreyaMSTyp_CommandPath"
#define FREYAMS_STR_COMMANDLINE                     "FreyaMSTyp_CommandLine"
#define FREYAMS_STR_CMDSTOPCURRENTCMD               "FreyaMSTyp_CMDStopCurrentCMD"
#define FREYAMS_STR_CMDRECEIVEBEGIN                 "FreyaMSTyp_CMDReceiveBegin"
#define FREYAMS_STR_CMDRECEIVE                      "FreyaMSTyp_CMDReceive"
#define FREYAMS_STR_CMDRECEIVEFINISH                "FreyaMSTyp_CMDReceiveFinish"
#define FREYAMS_STR_CMDREQUESTSCREEN                "FreyaMSTyp_CMDRequestScreen"
#define FREYAMS_STR_CMDRESULTSCREEN                 "FreyaMSTyp_CMDResultScreen"
#define FREYAMS_STR_CMDAUTOSCREEN                   "FreyaMSTyp_CMDAutoScreen"
#define FREYAMS_STR_CMDSTOPSCREEN                   "FreyaMSTyp_CMDStopScreen"

//Funcation
#define FREYA_FUN_PERSONCONFIGPATH(BJID)            QStandardPaths::writableLocation(QStandardPaths::DataLocation)+"/"+\
                                                    QCryptographicHash::hash((BJID).toUtf8(),QCryptographicHash::Md5).toHex()+"/"

#endif //FREYAMS_GLOBAL_H
