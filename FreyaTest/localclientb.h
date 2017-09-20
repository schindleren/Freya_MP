#ifndef LOCALCLIENTB_H
#define LOCALCLIENTB_H

#include "freyabaseaction.h"

#include <QObject>

class LocalClientB : public QObject, public FreyaBaseAction
{
    Q_OBJECT
public:
    LocalClientB();

protected:
    virtual void Execute(const quint64 &command);
    virtual void Execute(const FreyaData data);

};

#endif // LOCALCLIENTB_H
