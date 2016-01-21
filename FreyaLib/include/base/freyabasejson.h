#ifndef FREYABASEJSON_H
#define FREYABASEJSON_H

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QDebug>

class FreyaBaseJson
{
public:
    explicit FreyaBaseJson();
    ~FreyaBaseJson();

    static QByteArray VariantToByteArray(const QVariantMap &varmap);
    static QVariantMap ByteArrayToVariant(const QByteArray &ba);
};

#endif // FREYABASEJSON_H
