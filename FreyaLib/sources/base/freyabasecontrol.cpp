#include "freyabasecontrol.h"
#include "freyabaseaction.h"
#include "freyapublicregister.h"
#include <QDateTime>

FreyaBaseControl::FreyaBaseControl() :
    FreyaAbstractControl(),m_FreyaPublicRegister(NULL)
{
    m_FreyaPublicRegister = new FreyaPublicRegister();
    qRegisterMetaType<FreyaBaseData>("FreyaBaseData");
}

FreyaBaseControl::~FreyaBaseControl()
{
    if(m_FreyaPublicRegister)
    {
        delete m_FreyaPublicRegister;
    }
    qDebug()<<"FreyaLib > "<<"Destructor"<<this;
}

bool FreyaBaseControl::CheckFreyaLibConfig(const QString &filePath, const QString &configKey)
{
    return m_FreyaPublicRegister->CheckFreyaLibConfig(filePath, configKey);
}

QVariantMap FreyaBaseControl::GetConfigFromFile(const QString &filePath)
{
    return m_FreyaPublicRegister->GetConfigFromFile(filePath);
}

bool FreyaBaseControl::SetConfigToFile(const QString &filePath, const QVariantMap &varmap)
{
    return m_FreyaPublicRegister->SetConfigToFile(filePath, varmap);
}

QVariant FreyaBaseControl::GetConfig(const QStringList &configPath)
{
    return m_FreyaPublicRegister->GetConfig(configPath);
}

bool FreyaBaseControl::SetConfig(const QStringList &configPath, const QVariant &var)
{
    return m_FreyaPublicRegister->SetConfig(configPath, var);
}

bool FreyaBaseControl::InsertConfig(const QStringList &configPath, const QVariant &var)
{
    return m_FreyaPublicRegister->InsertConfig(configPath, var);
}

bool FreyaBaseControl::RemoveConfig(const QStringList &configPath)
{
    return m_FreyaPublicRegister->RemoveConfig(configPath);
}

bool FreyaBaseControl::RegisterObject(FreyaAbstractAction *actObject, const char *objectName)
{
    return m_FreyaPublicRegister->RegisterObject(actObject, objectName);
}

bool FreyaBaseControl::UnRegisterObject(const QString &objectName)
{
    return m_FreyaPublicRegister->UnRegisterObject(objectName);
}

bool FreyaBaseControl::UnRegisterObject(FreyaAbstractAction *actObject)
{
    return m_FreyaPublicRegister->UnRegisterObject(GetActionObjectName(actObject));
}

FreyaAbstractAction *FreyaBaseControl::GetActionObject(const QString &objectName)
{
    return m_FreyaPublicRegister->GetObject(objectName);
}

QString FreyaBaseControl::GetActionObjectName(FreyaAbstractAction *actObject)
{
    return m_FreyaPublicRegister->GetObjectName(actObject);
}

void FreyaBaseControl::DeleteAllAction(const QList<FreyaAbstractAction *> &except)
{
    QList<FreyaAbstractAction *> ActionList = m_FreyaPublicRegister->AllRegisterAction().values();
    foreach (FreyaAbstractAction *pAction, ActionList)
    {
        if(!except.contains(pAction))
        {
            delete pAction;
        }
    }
}

void FreyaBaseControl::DeleteAllAction(const QStringList &except)
{
    QList<FreyaAbstractAction *> ActionList;
    foreach (const QString &ActionName, except)
    {
        ActionList.append(m_FreyaPublicRegister->GetObject(ActionName));
    }
    DeleteAllAction(ActionList);
}

bool FreyaBaseControl::InsertBaseData(FreyaBaseData *pData)
{
    return m_FreyaPublicRegister->InsertBaseData(pData);
}

FreyaBaseData *FreyaBaseControl::FindBaseData(const QString &dataID)
{
    return m_FreyaPublicRegister->FindBaseData(dataID);
}

FreyaBaseData *FreyaBaseControl::TakeBaseData(const QString &dataID)
{
    return m_FreyaPublicRegister->TakeBaseData(dataID);
}

bool FreyaBaseControl::RequestExecution(void *pRequester)
{
    qDebug()<<"FreyaLib > "<<"Execution:"<<"without arguments"<<"From:"<<pRequester;
    bool r = false;
    QStringList ActionNames = m_FreyaPublicRegister->AllRegisterAction().keys();
    m_RequesterVec.push_back(pRequester);
    foreach (const QString &ActionName, ActionNames)
    {
        r = true;
        FreyaAbstractAction *pAction = m_FreyaPublicRegister->AllRegisterAction().value(ActionName, NULL);
        if(pAction)
        {
            pAction->Execute();
        }
    }
    if(m_RequesterVec.size() > 0)
    {
        m_RequesterVec.pop_back();
    }
    return r;
}

bool FreyaBaseControl::RequestExecution(const quint64 &command, void *pRequester)
{
    qDebug()<<"FreyaLib > "<<"Execution:"<<"Command:"<<hex<<command<<dec<<"From:"<<pRequester;
    bool r = false;
    QStringList ActionNames = m_FreyaPublicRegister->AllRegisterAction().keys();
    m_RequesterVec.push_back(pRequester);
    foreach (const QString &ActionName, ActionNames)
    {
        r = true;
        FreyaAbstractAction *pAction = m_FreyaPublicRegister->AllRegisterAction().value(ActionName, NULL);
        if(pAction)
        {
            pAction->Execute(command);
        }
    }
    if(m_RequesterVec.size() > 0)
    {
        m_RequesterVec.pop_back();
    }
    return r;
}

bool FreyaBaseControl::RequestExecution(const FreyaBaseData &BaseData, void *pRequester)
{
    qDebug()<<"FreyaLib > "<<"Execution:"<<"DataID:"<<BaseData.dataID<<"Command:"<<hex<<BaseData.command<<dec<<"Arguments:"<<BaseData.arguments<<"From:"<<pRequester;
    bool r = false;
    QStringList ActionNames = m_FreyaPublicRegister->AllRegisterAction().keys();
    m_RequesterVec.push_back(pRequester);
    foreach (const QString &ActionName, ActionNames)
    {
        r = true;
        FreyaAbstractAction *pAction = m_FreyaPublicRegister->AllRegisterAction().value(ActionName, NULL);
        if(pAction)
        {
            pAction->Execute(BaseData);
        }
    }
    if(m_RequesterVec.size() > 0)
    {
        m_RequesterVec.pop_back();
    }
    return r;
}
