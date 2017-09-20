#ifndef LOCALCLIENTS_H
#define LOCALCLIENTS_H

#include "freyabaseactionex.h"
#include <QObject>

class LocalClientS : public FreyaBaseActionEx
{
    Q_OBJECT

public:
    LocalClientS();

protected slots:
    virtual void OnExecuteEx(const quint64 &command);
    virtual void OnExecuteEx(FreyaData data);
};

#endif // LOCALCLIENTS_H
