#ifndef FREYALAUNCHTESTB_H
#define FREYALAUNCHTESTB_H

#include "freyalaunchtestb_global.h"
#include "../FreyaLaunch/freya_global.h"

class FreyaTestB :  public FreyaBaseAction
{
public:
    FreyaTestB();

public:
    virtual void Execute(const FreyaData data);
};

class FREYALAUNCHTESTBSHARED_EXPORT FreyaLaunchTestB
{
public:
    FreyaLaunchTestB();
};

#endif // FREYALAUNCHTESTB_H
