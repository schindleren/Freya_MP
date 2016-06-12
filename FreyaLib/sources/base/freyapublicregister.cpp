#include "freyapublicregister.h"
#include "freyabaseaction.h"
#include "freyabasejson.h"
#include "freyacryptogram.h"

FreyaPublicRegister::FreyaPublicRegister()
{
    qRegisterMetaType<FreyaBaseData>("FreyaBaseData");
}

bool FreyaPublicRegister::InsertBaseData(FreyaBaseData *pData)
{
    if(m_FreyaDataMap.contains(pData->dataID))
    {
        return false;
    }
    m_FreyaDataMap.insert(pData->dataID, pData);
    return true;
}

FreyaBaseData* FreyaPublicRegister::FindBaseData(const QString &dataID)
{
    return m_FreyaDataMap.value(dataID, NULL);
}

FreyaBaseData* FreyaPublicRegister::TakeBaseData(const QString &dataID)
{
    if(m_FreyaDataMap.contains(dataID))
        return m_FreyaDataMap.take(dataID);
    else
        return NULL;
}

bool FreyaPublicRegister::CheckFreyaLibConfig(const QString &filePath, const QString &configKey)
{
    if(configKey.isEmpty())
    {
        return false;
    }
    GetConfigFromFile(filePath);
    if(configKey.toLower() != FreyaCryptogram::CheckSum(filePath))
    {
        QStringList VerList = GetConfig(QStringList()<<FREYALIB_KEY_LIBCONFIG<<FREYALIB_KEY_SUPVER).toStringList();
        if(!VerList.contains(configKey, Qt::CaseInsensitive))
        {
            return false;
        }
    }

    QVariantMap cmdMap = GetConfig(QStringList()<<FREYALIB_KEY_LIBCONFIG<<FREYALIB_KEY_CMDDEF).toMap();
    QMapIterator<QString, QVariant> cmdMapIT(cmdMap);
    while (cmdMapIT.hasNext())
    {
        QString key = cmdMapIT.next().key();
        QString value;
#ifdef QT_DEBUG
        value = cmdMapIT.value().toString();
#endif
        bool ok;
        m_FreyaCmdMap.insert(key.toULongLong(&ok, 16), value);
    }
    return true;
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
    if(m_FreyaActObjectMap.contains(objectName) || NULL == actObject)
    {
        return false;
    }
    else
    {
        qDebug()<<"Register Object:"<<objectName<<actObject;
        m_FreyaActObjectMap.insert(objectName, actObject);
        return m_FreyaActObjectMap.contains(objectName);
    }
}

bool FreyaPublicRegister::UnRegisterObject(const QString &objectName)
{
    qDebug()<<"Unregister Object:"<<objectName<<m_FreyaActObjectMap.value(objectName);
    m_FreyaActObjectMap.remove(objectName);
    return !m_FreyaActObjectMap.contains(objectName);
}

FreyaBaseAction *FreyaPublicRegister::GetObject(const QString &objectName)
{
    return m_FreyaActObjectMap.value(objectName, NULL);
}

QString FreyaPublicRegister::GetObjectName(FreyaBaseAction *actObject)
{
    return m_FreyaActObjectMap.key(actObject, "");
}

QMap<QString, FreyaBaseAction*> &FreyaPublicRegister::AllRegisterAction()
{
    return m_FreyaActObjectMap;
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

QByteArray FreyaBaseData::Serialize(const FreyaBaseData &data)
{
    QByteArray ba;
    QDataStream stream(&ba, QIODevice::WriteOnly);
    QVariantMap varMap;
    varMap.insert(FREYALIB_TYP_CMD, data.command);
    varMap.insert(FREYALIB_TYP_ARG, data.arguments);
    QJsonDocument jsonDoc(QJsonObject::fromVariantMap(varMap));
    stream<<data.dataID<<jsonDoc.toJson();
    return ba;
}

FreyaBaseData FreyaBaseData::Unserialize(const QByteArray &ba)
{
    FreyaBaseData data;
    QDataStream stream(ba);
    QByteArray varBa;
    stream>>data.dataID>>varBa;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(varBa);
    QVariantMap varMap = jsonDoc.object().toVariantMap();
    if(!varMap.isEmpty())
    {
        data.command = varMap.value(FREYALIB_TYP_CMD).toULongLong();
        data.arguments = varMap.value(FREYALIB_TYP_ARG);
    }
    return data;
}
