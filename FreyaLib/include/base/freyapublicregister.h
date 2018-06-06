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
class FreyaBaseAction;
struct FreyaBaseData;

typedef QSharedPointer<FreyaBaseData> FreyaData;

class FREYALIBSHARED_EXPORT FreyaBaseData
{
public:
    QString     dataID;
    quint64     command;

private:
    QVariantMap arguments;

public:
    FreyaBaseData()
    {
        dataID = QUuid::createUuid().toString();
        command = 0x0;
        arguments = QVariantMap();
    }
    FreyaBaseData(const FreyaBaseData & other)
    {
        dataID = other.dataID;
        command = other.command;
        arguments = other.arguments;
    }
    ~FreyaBaseData()
    {
//        qDebug()<<"FreyaLib > "<<"Delete freya data. ID: "<<dataID;
    }

    QVariant GetArgument();
    void SetArgument(const QVariant &value);
    QVariant GetArgument(const QString &key);
    void SetArgument(const QString &key, const QVariant &value);

    static QByteArray Serialize(const FreyaData data);
    static FreyaData Unserialize(const QByteArray & ba);

    static FreyaData CreateDate(const quint64 &cmd = 0)
    {
        FreyaData pDate(new FreyaBaseData());
//        qDebug()<<"FreyaLib > "<<"Create freya data. ID: "<<pDate->dataID;
        pDate->command = cmd;
        return pDate;
    }
    static FreyaData CreateDate(const FreyaBaseData & other)
    {
        FreyaData pDate(new FreyaBaseData(other));
//        qDebug()<<"FreyaLib > "<<"Create freya data. ID: "<<pDate->dataID;
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

    QVariantMap GetConfigFromFile(const QString &filePath);
    bool SetConfigToFile(const QString &filePath, const QVariantMap &varmap);

    QVariant GetConfig(const QStringList &configPath);
    bool SetConfig(const QStringList &configPath, const QVariant &var);
    bool InsertConfig(const QStringList &configPath, const QVariant &var);
    bool RemoveConfig(const QStringList &configPath);

    bool RegisterObject(FreyaBaseAction *actObject, const QString &objectName);
    bool UnRegisterObject(const QString &objectName);
    bool RegisterCommand(FreyaBaseAction* actObject, QList<quint64> commandList);
    bool UnRegisterCommand(FreyaBaseAction* actObject);
    bool CheckObjectCommand(FreyaBaseAction* actObject, quint64 command);
    FreyaBaseAction *GetObject(const QString &objectName);
    QString GetObjectName(FreyaBaseAction *actObject);

    QHash<QString, FreyaBaseAction *> &AllRegisterAction();

private:
    bool ConfigModifyRecursion(QVariantMap &varMap, const QStringList &configPath, const ConfModType &type, const QVariant &var = QVariant());

private:
    QHash<QString, FreyaData>                           m_FreyaDataHash;
    QPair<QString, QVariantMap>                         m_FreyaConfigPair;
    QHash<QString, FreyaBaseAction*>                    m_FreyaActObjectHash;
    QHash<FreyaBaseAction*, QList<quint64> >            m_FreyaCmdHash;
};

#endif // FREYAPUBLICREGISTER_H
