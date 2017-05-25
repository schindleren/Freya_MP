#ifndef FREYABASECONTROL_H
#define FREYABASECONTROL_H

#include "freyalib_global.h"
#include "freyaabstractcontrol.h"
#include <vector>
#include <QDebug>

#define CUSTOMCLSTOVARIANT(pObject)             QVariant::fromValue((void*)pObject)
#define VARIANTTOCUSTOMCLS(pObject, ClassName)  static_cast<ClassName*>(pObject.value<void*>())

#define FREYA_REQUESTEXECUTION_NON              FreyaBaseControl::GetFreyaControl()->RequestExecution(this)
#define FREYA_REQUESTEXECUTION(data)            FreyaBaseControl::GetFreyaControl()->RequestExecution(data, this)

struct FreyaBaseData;
class FreyaPublicRegister;
class FreyaAbstractAction;
class FREYALIBSHARED_EXPORT FreyaBaseControl : public FreyaAbstractControl
{
private:
    explicit FreyaBaseControl();

public:
    ~FreyaBaseControl();
    bool CheckFreyaLibConfig(const QString &filePath, const QString &configKey);

    QVariantMap GetConfigFromFile(const QString &filePath);
    bool SetConfigToFile(const QString &filePath, const QVariantMap &varmap);
    QVariant GetConfig(const QStringList &configPath);
    bool SetConfig(const QStringList &configPath, const QVariant &var);
    bool InsertConfig(const QStringList &configPath, const QVariant &var);
    bool RemoveConfig(const QStringList &configPath);

    bool RegisterObject(FreyaAbstractAction *actObject, const char *objectName);
    bool UnRegisterObject(const QString &objectName);
    bool UnRegisterObject(FreyaAbstractAction *actObject);
    FreyaAbstractAction *GetActionObject(const QString &objectName);
    QString GetActionObjectName(FreyaAbstractAction *actObject);
    void DeleteAllAction(const QList<FreyaAbstractAction*> &except = QList<FreyaAbstractAction*>());
    void DeleteAllAction(const QStringList &except);

    bool InsertBaseData(FreyaBaseData *pData);
    FreyaBaseData *FindBaseData(const QString &dataID);
    FreyaBaseData *TakeBaseData(const QString &dataID);

    bool RequestExecution(void *pRequester = NULL);
    bool RequestExecution(const quint64 &command, void *pRequester = NULL);
    bool RequestExecution(const FreyaBaseData &BaseData, void *pRequester = NULL);

    void *FreyaRequester(){return m_RequesterVec.size()>0?m_RequesterVec.back():NULL;}
private:
    std::vector<void*>          m_RequesterVec;
    FreyaPublicRegister         *m_FreyaPublicRegister;

public:
    static FreyaBaseControl *GetFreyaControl()
    {
        static FreyaBaseControl *pFreyaBaseControl = NULL;
        if(NULL == pFreyaBaseControl)
        {
            pFreyaBaseControl = new FreyaBaseControl();
            qDebug()<<"FreyaLib > "<<"Structure FreyaBaseControl"<<pFreyaBaseControl;
        }
        return pFreyaBaseControl;
    }
};

#endif // FREYABASECONTROL_H
