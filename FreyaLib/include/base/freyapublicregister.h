#ifndef FREYAPUBLICREGISTER_H
#define FREYAPUBLICREGISTER_H

#include <QDataStream>
#include <QVariant>
#include <QFile>
#include <QFileInfo>
#include <QPair>
#include <QDir>
#include <QUuid>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include "freyalib_global.h"

enum ConfModType{
    eCMADD = 0,
    eCMDEL = 1,
    eCMMOD = 2
};

class FreyaBaseJson;
class FreyaBaseAction;
struct FreyaBaseData
{
    QString     dataID;
    quint64     command;
    QVariant    arguments;

    FreyaBaseData()
    {
        dataID = QUuid::createUuid().toString();
        command = 0x0;
        arguments = QVariant();
    }
    FreyaBaseData(const FreyaBaseData & other)
    {
        dataID = other.dataID;
        command = other.command;
        arguments = other.arguments;
    }
    static QByteArray Serialize(const FreyaBaseData & data);

    static FreyaBaseData Unserialize(const QByteArray & ba);
};

class FreyaPublicRegister
{
public:
    explicit FreyaPublicRegister();

    bool InsertBaseData(FreyaBaseData* pData);
    FreyaBaseData* FindBaseData(const QString &dataID);
    FreyaBaseData* TakeBaseData(const QString &dataID);

    bool CheckFreyaLibConfig(const QString &filePath, const QString &configKey);
    QVariantMap GetConfigFromFile(const QString &filePath);
    bool SetConfigToFile(const QString &filePath, const QVariantMap &varmap);

    QVariant GetConfig(const QStringList &configPath);
    bool SetConfig(const QStringList &configPath, const QVariant &var);
    bool InsertConfig(const QStringList &configPath, const QVariant &var);
    bool RemoveConfig(const QStringList &configPath);

    bool RegisterObject(FreyaBaseAction *actObject, const QString &objectName);
    bool UnRegisterObject(const QString &objectName);
    FreyaBaseAction *GetObject(const QString &objectName);
    QString GetObjectName(FreyaBaseAction *actObject);

    QMap<QString, FreyaBaseAction*> &AllRegisterAction();

private:
    bool ConfigModifyRecursion(QVariantMap &varMap, const QStringList &configPath, const ConfModType &type, const QVariant &var = QVariant());

private:
    QMap<QString, FreyaBaseData*>       m_FreyaDataMap;
    QPair<QString, QVariantMap>         m_FreyaConfigPair;
    QMap<QString, FreyaBaseAction*>     m_FreyaActObjectMap;
    QMap<quint64, QString>              m_FreyaCmdMap;
};

#endif // FREYAPUBLICREGISTER_H
