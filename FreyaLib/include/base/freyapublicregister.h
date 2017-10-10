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
#include <QSharedPointer>

#include "freyalib_global.h"

enum ConfModType{
    eCMADD = 0,
    eCMDEL = 1,
    eCMMOD = 2
};

class FreyaBaseJson;
class FreyaAbstractAction;
struct FreyaBaseData;

typedef QSharedPointer<FreyaBaseData> FreyaData;

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
    ~FreyaBaseData()
    {
        qDebug()<<"FreyaLib > "<<"delete freya data. ID: "<<dataID;
    }
    static QByteArray Serialize(const FreyaData data);
    static FreyaData Unserialize(const QByteArray & ba);

    static FreyaData CreateDate()
    {
        FreyaData pDate(new FreyaBaseData());
        qDebug()<<"FreyaLib > "<<"Create freya data. ID: "<<pDate->dataID;
        return pDate;
    }
    static FreyaData CreateDate(const FreyaBaseData & other)
    {
        FreyaData pDate(new FreyaBaseData(other));
        qDebug()<<"FreyaLib > "<<"Create freya data. ID: "<<pDate->dataID;
        return pDate;
    }
};

class FreyaPublicRegister
{
public:
    explicit FreyaPublicRegister();

    bool InsertFreyaData(const FreyaData pData);
    FreyaData FindFreyaData(const QString &dataID);
    FreyaData TakeFreyaData(const QString &dataID);

    bool CheckFreyaLibConfig(const QString &filePath, const QString &configKey);
    QVariantMap GetConfigFromFile(const QString &filePath);
    bool SetConfigToFile(const QString &filePath, const QVariantMap &varmap);

    QVariant GetConfig(const QStringList &configPath);
    bool SetConfig(const QStringList &configPath, const QVariant &var);
    bool InsertConfig(const QStringList &configPath, const QVariant &var);
    bool RemoveConfig(const QStringList &configPath);

    bool RegisterObject(FreyaAbstractAction *actObject, const QString &objectName);
    bool UnRegisterObject(const QString &objectName);
    FreyaAbstractAction *GetObject(const QString &objectName);
    QString GetObjectName(FreyaAbstractAction *actObject);

    QMap<QString, FreyaAbstractAction*> &AllRegisterAction();

private:
    bool ConfigModifyRecursion(QVariantMap &varMap, const QStringList &configPath, const ConfModType &type, const QVariant &var = QVariant());

private:
    QMap<QString, FreyaData>            m_FreyaDataMap;
    QPair<QString, QVariantMap>         m_FreyaConfigPair;
    QMap<QString, FreyaAbstractAction*> m_FreyaActObjectMap;
    QMap<quint64, QString>              m_FreyaCmdMap;
};

#endif // FREYAPUBLICREGISTER_H
