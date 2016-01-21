#include "freyabasejson.h"

FreyaBaseJson::FreyaBaseJson()
{
}

FreyaBaseJson::~FreyaBaseJson()
{
}

QByteArray FreyaBaseJson::VariantToByteArray(const QVariantMap &varmap)
{
    QJsonDocument BaseDoc(QJsonObject::fromVariantMap(varmap));
    return BaseDoc.toJson();
}

QVariantMap FreyaBaseJson::ByteArrayToVariant(const QByteArray &ba)
{
    QJsonObject BaseObject = QJsonDocument::fromJson(ba).object();
    return BaseObject.toVariantMap();
}
