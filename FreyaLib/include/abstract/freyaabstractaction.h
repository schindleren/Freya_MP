#ifndef FREYAABSTRACTACTION_H
#define FREYAABSTRACTACTION_H

#include "freyapublicregister.h"

class FreyaAbstractAction
{
public:
    virtual ~FreyaAbstractAction(){}
    virtual void Execute()=0;
    virtual void Execute(const quint64 &command)=0;
    virtual void Execute(const FreyaBaseData &data)=0;
};

#endif // FREYAABSTRACTACTION_H
