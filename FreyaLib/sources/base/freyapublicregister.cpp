#include "freyapublicregister.h"
#include "freyabaseaction.h"
#include "freyabasejson.h"
#include "freyacryptogram.h"

FreyaPublicRegister::FreyaPublicRegister()
{
//    qRegisterMetaType<FreyaBaseData>("FreyaBaseData");
    qRegisterMetaType<QSharedPointer<FreyaBaseData> >("QSharedPointer<FreyaBaseData>");
    qRegisterMetaType<FreyaData>("FreyaData");
    qRegisterMetaType<FreyaBaseAction>("FreyaBaseAction");
    qRegisterMetaType<FreyaBaseAction*>("FreyaBaseAction*");
}

bool FreyaPublicRegister::InsertFreyaData(const FreyaData pData)
{
    if(m_FreyaDataHash.contains(pData->dataID))
    {
        return false;
    }
    m_FreyaDataHash.insert(pData->dataID, pData);
    return true;
}

FreyaData FreyaPublicRegister::FindFreyaData(const QString &dataID)
{
    return m_FreyaDataHash.value(dataID, FreyaData(NULL));
}

FreyaData FreyaPublicRegister::TakeFreyaData(const QString &dataID)
{
    if(m_FreyaDataHash.contains(dataID))
        return m_FreyaDataHash.take(dataID);
    else
        return FreyaData(NULL);
}

QVariantMap FreyaPublicRegister::GetConfigFromFile(const QString &filePath)
{
    QFileInfo fileinfo(filePath);
    QString AbsFilePath = fileinfo.absoluteFilePath();
    if(m_FreyaConfigPair.first == AbsFilePath)
    {
        return m_FreyaConfigPair.second;
    }
    else
    {
        QVariantMap varmap;
        QFile file(AbsFilePath);
        if(file.open(QFile::ReadOnly | QFile::Text))
        {
            varmap = FreyaBaseJson::ByteArrayToVariant(file.readAll());
            file.close();
        }
        m_FreyaConfigPair.first = AbsFilePath;
        m_FreyaConfigPair.second = varmap;
        return varmap;
    }
}

bool FreyaPublicRegister::SetConfigToFile(const QString &filePath, const QVariantMap &varmap)
{
    QFileInfo fileinfo(filePath);
    QString AbsFilePath = fileinfo.absoluteFilePath();
    QDir dir(fileinfo.absolutePath());
    if(!dir.exists())
    {
        dir.mkpath(dir.absolutePath());
    }
    QFile file(AbsFilePath);
    if(file.open(QFile::WriteOnly | QFile::Text))
    {
        file.resize(0);
        qint64 result = file.write(FreyaBaseJson::VariantToByteArray(varmap));
        file.close();
        if(result > 0)
        {
            m_FreyaConfigPair.first = AbsFilePath;
            m_FreyaConfigPair.second = varmap;
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

QVariant FreyaPublicRegister::GetConfig(const QStringList &configPath)
{
    if(configPath.size() > 0)
    {
        QVariantMap varmap = m_FreyaConfigPair.second;
        QVariant var;
        QListIterator<QString> ConfigIT(configPath);
        while(1)
        {
            var = varmap.value(ConfigIT.next());
            if(!ConfigIT.hasNext())
            {
                break;
            }
            else if(var.type() == QVariant::Map)
            {
                varmap = var.toMap();
                continue;
            }
            var = QVariant();
            break;
        }
        return var;
    }
    else
    {
        return QVariant();
    }
}

bool FreyaPublicRegister::SetConfig(const QStringList &configPath, const QVariant &var)
{
    if(configPath.size() > 0)
    {
        QVariantMap newVarmap = m_FreyaConfigPair.second;
        if(ConfigModifyRecursion(newVarmap, configPath, eCMMOD, var))
        {
            return (newVarmap.isEmpty() ? false : (SetConfigToFile(m_FreyaConfigPair.first, newVarmap)));
        }
    }
    return false;
}

bool FreyaPublicRegister::InsertConfig(const QStringList &configPath, const QVariant &var)
{
    if(configPath.size() > 0)
    {
        QVariantMap newVarmap = m_FreyaConfigPair.second;
        if(ConfigModifyRecursion(newVarmap, configPath, eCMADD, var))
        {
            return (newVarmap.isEmpty() ? false : (SetConfigToFile(m_FreyaConfigPair.first, newVarmap)));
        }
    }
    return false;
}

bool FreyaPublicRegister::RemoveConfig(const QStringList &configPath)
{
    if(configPath.size() > 0)
    {
        QVariantMap newVarmap = m_FreyaConfigPair.second;
        if(ConfigModifyRecursion(newVarmap, configPath, eCMDEL))
        {
            return (newVarmap.isEmpty() ? false : (SetConfigToFile(m_FreyaConfigPair.first, newVarmap)));
        }
    }
    return false;
}

bool FreyaPublicRegister::RegisterObject(FreyaBaseAction *actObject, const QString &objectName)
{
    if(m_FreyaActObjectHash.contains(objectName) || NULL == actObject)
    {
        return false;
    }
    else
    {
        qDebug()<<"FreyaLib > "<<"Register Object:"<<objectName<<actObject;
        m_FreyaActObjectHash.insert(objectName, actObject);
        return m_FreyaActObjectHash.contains(objectName);
    }
}

bool FreyaPublicRegister::UnRegisterObject(const QString &objectName)
{
    qDebug()<<"FreyaLib > "<<"Unregister Object:"<<objectName<<m_FreyaActObjectHash.value(objectName);
    m_FreyaActObjectHash.remove(objectName);
    return !m_FreyaActObjectHash.contains(objectName);
}

bool FreyaPublicRegister::RegisterCommand(FreyaBaseAction* actObject, QList<quint64> commandList)
{
    if(actObject && commandList.size() > 0)
    {
        m_FreyaCmdHash.insert(actObject, commandList);
        return m_FreyaCmdHash.contains(actObject);
    }
    return false;
}

bool FreyaPublicRegister::UnRegisterCommand(FreyaBaseAction* actObject)
{
    m_FreyaCmdHash.remove(actObject);
    return !m_FreyaCmdHash.contains(actObject);
}

bool FreyaPublicRegister::CheckObjectCommand(FreyaBaseAction* actObject, quint64 command)
{
    if(m_FreyaCmdHash.contains(actObject))
    {
        return m_FreyaCmdHash.value(actObject).contains(command);
    }
    return true;
}

FreyaBaseAction *FreyaPublicRegister::GetObject(const QString &objectName)
{
    return m_FreyaActObjectHash.value(objectName, NULL);
}

QString FreyaPublicRegister::GetObjectName(FreyaBaseAction *actObject)
{
    return m_FreyaActObjectHash.key(actObject, "");
}

QHash<QString, FreyaBaseAction*> &FreyaPublicRegister::AllRegisterAction()
{
    return m_FreyaActObjectHash;
}

bool FreyaPublicRegister::ConfigModifyRecursion(QVariantMap &varMap, const QStringList &configPath, const ConfModType &type, const QVariant &var)
{
    if(configPath.size() > 0)
    {
        const QString &key = configPath.at(0);

        if(configPath.size() == 1)
        {
            switch (type)
            {
            case eCMADD:
            {
                varMap.insert(key, var);
                return varMap.contains(key);
            }
            case eCMDEL:
            {
                varMap.remove(key);
                return !varMap.contains(key);
            }
            case eCMMOD:
            {
                if(varMap.contains(key))
                {
                    varMap.insert(key, var);
                    return (varMap.value(key) == var);
                }
                break;
            }
            default:
                return false;
            }
        }
        else
        {
            QVariant newVar = varMap.value(key);
            if(newVar.type() == QVariant::Map || eCMADD == type)
            {
                QStringList newConfigPath = configPath;
                newConfigPath.removeAt(0);
                QVariantMap tempVarMap = newVar.toMap();
                if(ConfigModifyRecursion(tempVarMap, newConfigPath, type, var))
                {
                    varMap.insert(key, tempVarMap);
                    return true;
                }
            }
        }
    }
    return false;
}

QVariant FreyaBaseData::GetArgument()
{
    return arguments.value(FREYALIB_FLG_ARG, QVariant());
}

void FreyaBaseData::SetArgument(const QVariant &value)
{
    arguments.insert(FREYALIB_FLG_ARG, value);
}

QVariant FreyaBaseData::GetArgument(const QString &key)
{
    return arguments.value(key);
}

void FreyaBaseData::SetArgument(const QString &key, const QVariant &value)
{
    arguments.insert(key, value);
}

QByteArray FreyaBaseData::Serialize(const FreyaData data)
{
    QByteArray ba;
    QDataStream stream(&ba, QIODevice::WriteOnly);
    QVariantMap varMap;
    varMap.insert(FREYALIB_TYP_CMD, data->command);
    varMap.insert(FREYALIB_TYP_ARG, data->arguments);
    QJsonDocument jsonDoc(QJsonObject::fromVariantMap(varMap));
    stream<<data->dataID<<jsonDoc.toJson();
    return ba;
}

FreyaData FreyaBaseData::Unserialize(const QByteArray &ba)
{
    QString dataId;
    QDataStream stream(ba);
    QByteArray varBa;
    stream>>dataId>>varBa;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(varBa);
    QVariantMap varMap = jsonDoc.object().toVariantMap();
    if(dataId.isEmpty() || varMap.isEmpty())
    {
        return FreyaData(NULL);
    }
    else
    {
        FreyaData pData = FreyaBaseData::CreateDate(varMap.value(FREYALIB_TYP_CMD).toULongLong());
        pData->dataID = dataId;
        pData->arguments = varMap.value(FREYALIB_TYP_ARG).toMap();
        return pData;
    }
}
