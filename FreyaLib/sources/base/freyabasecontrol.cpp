#include "freyabasecontrol.h"
#include "freyabaseaction.h"
#include "freyapublicregister.h"
#include <QDateTime>

FreyaBaseControl::FreyaBaseControl() :
    QObject(NULL), FreyaAbstractControl(), m_CMDDistribution(this)
{
    connect(this, SIGNAL(ToRequestExecution(FreyaData, void*)), &m_CMDDistribution, SLOT(OnRequestExecution(FreyaData, void*)), Qt::QueuedConnection);
}

FreyaBaseControl::~FreyaBaseControl()
{
    qDebug()<<"FreyaLib > "<<"Destructor"<<this;
}

QVariantMap FreyaBaseControl::GetConfigFromFile(const QString &filePath)
{
    return m_FreyaPublicRegister.GetConfigFromFile(filePath);
}

bool FreyaBaseControl::SetConfigToFile(const QString &filePath, const QVariantMap &varmap)
{
    return m_FreyaPublicRegister.SetConfigToFile(filePath, varmap);
}

QVariant FreyaBaseControl::GetConfig(const QStringList &configPath)
{
    return m_FreyaPublicRegister.GetConfig(configPath);
}

bool FreyaBaseControl::SetConfig(const QStringList &configPath, const QVariant &var)
{
    return m_FreyaPublicRegister.SetConfig(configPath, var);
}

bool FreyaBaseControl::InsertConfig(const QStringList &configPath, const QVariant &var)
{
    return m_FreyaPublicRegister.InsertConfig(configPath, var);
}

bool FreyaBaseControl::RemoveConfig(const QStringList &configPath)
{
    return m_FreyaPublicRegister.RemoveConfig(configPath);
}

bool FreyaBaseControl::RegisterObject(FreyaBaseAction *actObject, const char *objectName)
{
    return m_FreyaPublicRegister.RegisterObject(actObject, objectName);
}

bool FreyaBaseControl::UnRegisterObject(const QString &objectName)
{
    return m_FreyaPublicRegister.UnRegisterObject(objectName);
}

bool FreyaBaseControl::UnRegisterObject(FreyaBaseAction *actObject)
{
    return m_FreyaPublicRegister.UnRegisterObject(GetActionObjectName(actObject));
}

bool FreyaBaseControl::RegisterCommand(FreyaBaseAction *actObject, QList<quint64> commandList)
{
    qDebug()<<"FreyaLib > "<<"RegisterCommand object:"<< GetActionObjectName(actObject) << hex <<"commands:"<<commandList;
    return m_FreyaPublicRegister.RegisterCommand(actObject, commandList);
}

bool FreyaBaseControl::UnRegisterCommand(FreyaBaseAction *actObject)
{
    qDebug()<<"FreyaLib > "<<"UnRegisterCommand object:"<< GetActionObjectName(actObject);
    return m_FreyaPublicRegister.UnRegisterCommand(actObject);
}

FreyaBaseAction *FreyaBaseControl::GetActionObject(const QString &objectName)
{
    return m_FreyaPublicRegister.GetObject(objectName);
}

QString FreyaBaseControl::GetActionObjectName(FreyaBaseAction *actObject)
{
    return m_FreyaPublicRegister.GetObjectName(actObject);
}

void FreyaBaseControl::DeleteAllAction(const QList<FreyaBaseAction *> &except)
{
    QList<FreyaBaseAction *> ActionList = m_FreyaPublicRegister.AllRegisterAction().values();
    foreach (FreyaBaseAction *pAction, ActionList)
    {
        if(!except.contains(pAction))
        {
            delete pAction;
        }
    }
}

void FreyaBaseControl::DeleteAllAction(const QStringList &except)
{
    QList<FreyaBaseAction *> ActionList;
    foreach (const QString &ActionName, except)
    {
        ActionList.append(m_FreyaPublicRegister.GetObject(ActionName));
    }
    DeleteAllAction(ActionList);
}

bool FreyaBaseControl::InsertFreyaData(const FreyaData pData)
{
    return m_FreyaPublicRegister.InsertFreyaData(pData);
}

FreyaData FreyaBaseControl::FindFreyaData(const QString &dataID)
{
    return m_FreyaPublicRegister.FindFreyaData(dataID);
}

FreyaData FreyaBaseControl::TakeFreyaData(const QString &dataID)
{
    return m_FreyaPublicRegister.TakeFreyaData(dataID);
}

bool FreyaBaseControl::RequestExecution(void *pRequester)
{
    qDebug() << "FreyaLib > " << "Execution:" << "without arguments" << "From:" << pRequester;
    bool r = false;
    m_pRequester = pRequester;
    QHashIterator<QString, FreyaBaseAction*> ActionIt(m_FreyaPublicRegister.AllRegisterAction());
    while (ActionIt.hasNext())
    {
        ActionIt.next();
        r = true;
        FreyaBaseAction *pAction = ActionIt.value();
        if(pAction)
        {
            pAction->Execute();
        }
    }
    m_pRequester = NULL;
    return r;
}

void FreyaBaseControl::RequestExecution(const quint64 &command, void *pRequester)
{
    return RequestExecution(FreyaBaseData::CreateDate(command), pRequester);
}

void FreyaBaseControl::RequestExecution(const FreyaData BaseData, void *pRequester)
{
    emit ToRequestExecution(BaseData, pRequester);
}

void FreyaBaseControl::ActionExecution(FreyaBaseAction* pAction, const FreyaData BaseData)
{
    pAction->Execute(BaseData);
}
