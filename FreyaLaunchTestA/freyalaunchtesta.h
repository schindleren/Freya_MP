#ifndef FREYALAUNCHTESTA_H
#define FREYALAUNCHTESTA_H

#include "freyalaunchtesta_global.h"
#include "../FreyaLaunch/freya_global.h"

class FreyaLaunchTestA : public FreyaBaseActionEx
{
    Q_OBJECT
public:
    FreyaLaunchTestA();

protected slots:
    virtual void OnExecuteEx(const FreyaData data);

private slots:
    void OnSendTest();

private:
};

#endif // FREYALAUNCHTESTA_H
