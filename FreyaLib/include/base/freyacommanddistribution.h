#ifndef FREYACOMMANDDISTRIBUTION_H
#define FREYACOMMANDDISTRIBUTION_H

#include "freyalib_global.h"
#include "freyapublicregister.h"
#include <QThread>

class FreyaBaseControl;
class FreyaCommandDistribution : public QThread
{
    Q_OBJECT
public:
    explicit FreyaCommandDistribution(FreyaBaseControl *pControl);

    QStringList DataList();
    bool InsertData(FreyaData data);
    FreyaData FindData(const QString &dataId);

protected:
    virtual void run();

private:
    FreyaBaseControl                *m_pControl;
    QStringList                     m_DataList;
    QHash<QString, FreyaData>       m_DataQueue;
};

#endif // FREYACOMMANDDISTRIBUTION_H
