#ifndef FREYA_GLOBAL_H
#define FREYA_GLOBAL_H

#include <QStandardPaths>
#include <QCryptographicHash>
#include <QRect>

//Global path
#define FREYA_PTH_GLOBALCONFIGPATH                  "./FreyaConf/globalconfig.json"
#define FREYA_PTH_STATICCONFIGPATH                  "./FreyaConf/staticconfig.ini"
#define FREYA_PTH_GLOBALRESPATH                     "./FreyaRes/"
#define FREYA_PTH_PERSONCONFIGPATH                  "freyaconfig.json"
#define FREYA_PTH_DEFPORTRAITPATH                   "freyaportrait.png"
#define FREYA_PTH_DEFAULTSTYLEPATH                  "./FreyaRes/freyaimstyle.qss"
#define FREYA_PTH_DEFAULTTRANSPATH                  "./FreyaRes/freyaimtrans.qm"
#define FREYA_PTH_LOGINGIFPATH                      "./FreyaRes/loading.gif"
#define FREUA_PTH_IMICON                            ":/icon/freyaico"

//Global config name
#define FREYA_CFG_CONFIDDEFAULT                     "FreyaCfg_Default"
#define FREYA_CFG_CONFIDSERVER                      "FreyaCfg_Server"
#define FREYA_CFG_CONFIDPORT                        "FreyaCfg_Port"
#define FREYA_CFG_CONFIDDOMAIN                      "FreyaCfg_Domain"
#define FREYA_CFG_CONFIDRESOURCE                    "FreyaCfg_Resource"
#define FREYA_CFG_CONFIDACCOUND                     "FreyaCfg_Accound"
#define FREYA_CFG_CONFIDUSERS                       "FreyaCfg_Users"
#define FREYA_CFG_CONFIDSTATES                      "FreyaCfg_States"

//Person config name
#define FREYA_CFG_CONFIDPERSON                      "FreyaCfg_Person"
#define FREYA_CFG_CONFIDPASSWORD                    "FreyaCfg_LoginPassword"
#define FREYA_CFG_CONFIDSTATE                       "FreyaCfg_LoginState"
#define FREYA_CFG_CONFIDGEOMETRY                    "FreyaCfg_MainWndGeometry"
#define FREYA_CFG_CONFIDGEOMETRY_X                  "FreyaCfg_MainWndX"
#define FREYA_CFG_CONFIDGEOMETRY_Y                  "FreyaCfg_MainWndY"
#define FREYA_CFG_CONFIDGEOMETRY_W                  "FreyaCfg_MainWndWidth"
#define FREYA_CFG_CONFIDGEOMETRY_H                  "FreyaCfg_MainWndHeight"
#define FREYA_CFG_VCARDBIRTHDAY                     "FreyaCfg_VCardBirthday"
#define FREYA_CFG_VCARDDESCRIPTION                  "FreyaCfg_VCardDescription"
#define FREYA_CFG_VCARDEMAIL                        "FreyaCfg_VCardEmail"
#define FREYA_CFG_VCARDFULLNAME                     "FreyaCfg_VCardFullname"
#define FREYA_CFG_VCARDNICKNAME                     "FreyaCfg_VCardNickname"
#define FREYA_CFG_VCARDURL                          "FreyaCfg_VCardUrl"
#define FREYA_CFG_VCARDADDRESSES                    "FreyaCfg_VCardAddresses"
#define FREYA_CFG_VCARDPHONE                        "FreyaCfg_VCardPhone"
#define FREYA_CFG_VCARDGENDER                       "FreyaCfg_VCardgender"

//Default config information
#define FREYA_INF_DEFCONFSERVER                     "127.0.0.1"         //"jesschindl.vicp.net"
#define FREYA_INF_DEFCONFPORT                       "5222"              //"54056"
#define FREYA_INF_DEFCONFDOMAIN                     "schindland"
#define FREYA_INF_DEFCONFRESOURCE                   "FreyaIM"
#define FREYA_INF_DEFCONFMAINRECT                   QRect(50, 100, 260, 600)
#define FREYA_INF_DEFPORTRAITSIZE                   QSize(500, 500)

//Define
#define FREYA_DEF_DATEFORMAT                        "yyyy-MM-dd"
#define FREYA_DEF_TIMEFORMAT                        "hh:mm:ss"
#define FREYA_DEF_GENDERMALE                        "FreyaDef_Male"
#define FREYA_DEF_GENDERFEMALE                      "FreyaDef_Female"

//Object register name
#define FREYA_OBJ_FREYAIM                           "FreyaObj_FreyaIM"
#define FREYA_OBJ_XMPPCLIENTNAME                    "FreyaObj_XmppClient"
#define FREYA_OBJ_DEFAULTCLIENT                     "FreyaObj_DefaultClient"
#define FREYA_OBJ_LOGINDIALOG                       "FreyaObj_LoginDialog"
#define FREYA_OBJ_MAINDIALOG                        "FreyaObj_MainDialog"
#define FREYA_OBJ_SYSTRAYICON                       "FreyaObj_SysTrayIcon"
#define FREYA_OBJ_REGISTERDIALOG                    "FreyaObj_RegisterDialog"
#define FREYA_OBJ_IDCARDDIALOG                      "FreyaObj_IDCardDialog"
#define FREYA_OBJ_CONTACTTREE                       "FreyaObj_ContactTree"
#define FREYA_OBJ_MESSAGEDIALOG                     "FreyaObj_MessageDialog"
#define FREYA_OBJ_FREYAPLUGIN                       "FreyaObj_FreyaPluginPlatform"

//Command
#define FREYA_CMD_CLIENTLOGIN                       0x0001000000000000//"FreyaCmd_ClientLogin"
#define FREYA_CMD_CONNECTSTATE                      0x0002000000000000//"FreyaCmd_ConnectState"
#define FREYA_CMD_LOGINEDSTATE                      0x0003000000000000//"FreyaCmd_LoginedState"
#define FREYA_CMD_LOGOUTEDSTATE                     0x0004000000000000//"FreyaCmd_LogoutedState"
#define FREYA_CMD_TRAYMESSAGE                       0x0005000000000000//"FreyaCmd_TrayMessage"
#define FREYA_CMD_TRAYACTIVE                        0x0006000000000000//"FreyaCmd_TrayActive"
#define FREYA_CMD_REGISTREQUEST                     0x0007000000000000//"FreyaCmd_RegistRequest"
#define FREYA_CMD_REGISTRESULT                      0x0008000000000000//"FreyaCmd_RegistResult"
#define FREYA_CMD_VCARDREQUEST                      0x0009000000000000//"FreyaCmd_VCardRequest"
#define FREYA_CMD_VCARDRESULT                       0x000a000000000000//"FreyaCmd_VCardResult"
#define FREYA_CMD_CONFIGNREQUEST                    0x000b000000000000//"FreyaCmd_ConfigurationRequest"
#define FREYA_CMD_CONFIGNRESULT                     0x000c000000000000//"FreyaCmd_ConfigurationResult"
#define FREYA_CMD_VCARDMODREQUEST                   0x000d000000000000//"FreyaCmd_VCardModifyRequest"
#define FREYA_CMD_VCARDMODRESULT                    0x000e000000000000//"FreyaCmd_VCardModifyResult"
#define FREYA_CMD_SHOWIDCARD                        0x000f000000000000//"FreyaCmd_ShowIDCard"
#define FREYA_CMD_PRESENCECHANGED                   0x0010000000000000//"FreyaCmd_PresenceChanged"
#define FREYA_CMD_MODIFYPRESENCE                    0x0011000000000000//"FreyaCmd_ModifyChanged"
#define FREYA_CMD_ROSTERRECEIVED                    0x0012000000000000//"FreyaCmd_RosterReceived"
#define FREYA_CMD_SUBSCRIBEREQUEST                  0x0013000000000000//"FreyaCmd_SubscribeRequest"
#define FREYA_CMD_SUBSCRIBERESULT                   0x0014000000000000//"FreyaCmd_SubscribeResult"

//Type
#define FREYA_STR_TRAYMSGCONTENT                    "FreyaTyp_TrayMsgContent"
#define FREYA_INT_TRAYMSGSTYLE                      "FreyaTyp_TrayMsgStyle"
#define FREYA_STR_XMPPBAREJID                       "FreyaTyp_XmppBareJid"
#define FREYA_CLS_XMPPCONFIGURATION                 "FreyaTyp_XmppConfiguration"
#define FREYA_CLS_XMPPPRESENCESTATUS                "FreyaTyp_XmppPresenceStatus"
#define FREYA_CLS_XMPPVCARDIQ                       "FreyaTyp_XmppVCardIq"
#define FREYA_CLS_MESSAGEBOXDECISION                "FreyaTyp_MessageBoxDecision"

//MS Type
#define FREYAMS_STR_RESOURCENAME                    "FreyaMSTyp_ResourceName"
#define FREYAMS_STR_COMMANDPATH                     "FreyaMSTyp_CommandPath"
#define FREYAMS_STR_COMMANDLINE                     "FreyaMSTyp_CommandLine"
#define FREYAMS_STR_CMDRECEIVEBEGIN                 "FreyaMSTyp_CMDReceiveBegin"
#define FREYAMS_STR_CMDRECEIVE                      "FreyaMSTyp_CMDReceive"
#define FREYAMS_STR_CMDRECEIVEFINISH                "FreyaMSTyp_CMDReceiveFinish"
#define FREYAMS_STR_CMDSTOPCURRENTCMD               "FreyaMSTyp_CMDStopCurrentCMD"
#define FREYAMS_STR_CMDREQUESTSCREEN                "FreyaMSTyp_CMDRequestScreen"
#define FREYAMS_STR_CMDRESULTSCREEN                 "FreyaMSTyp_CMDResultScreen"
#define FREYAMS_STR_CMDAUTOSCREEN                   "FreyaMSTyp_CMDAutoScreen"
#define FREYAMS_STR_CMDSTOPSCREEN                   "FreyaMSTyp_CMDStopScreen"

//Funcation
#define FREYA_FUN_PERSONCONFIGPATH(BJID)            QStandardPaths::writableLocation(QStandardPaths::DataLocation)+"/"+\
                                                    QCryptographicHash::hash((BJID).toUtf8(),QCryptographicHash::Md5).toHex()+"/"

#endif
