#ifndef FREYAMSCONFIG_H
#define FREYAMSCONFIG_H

#include "freyams_global.h"
#include <QObject>
#include <QDir>

class QXmppConfiguration;
class FreyaMSConfig : public QObject
{
    Q_OBJECT

public:
    FreyaMSConfig(QObject *parent = 0);
    ~FreyaMSConfig();
    void InitConfig();
    void InsertGlobalData(const QXmppConfiguration &config);

private:
    void InitGlobalData();
};

#endif // FREYAMSCONFIG_H
