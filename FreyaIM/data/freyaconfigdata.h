#ifndef FREYACONFIGDATA_H
#define FREYACONFIGDATA_H

#include "freyaim_global.h"
#include <QObject>
#include <QDir>

class FreyaConfigData : public QObject
{
    Q_OBJECT

public:
    FreyaConfigData(QObject *parent = 0);
    ~FreyaConfigData();
    void InitConfig();

private:
    void InitGlobalData();

private:
};

#endif // FREYACONFIGDATA_H
