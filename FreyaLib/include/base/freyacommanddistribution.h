#ifndef FREYACOMMANDDISTRIBUTION_H
#define FREYACOMMANDDISTRIBUTION_H

#include "freyalib_global.h"
#include "freyapublicregister.h"
#include <QThread>

class FreyaBaseControl;
class FreyaCommandDistribution : public QObject
{
    Q_OBJECT
public:
    explicit FreyaCommandDistribution(FreyaBaseControl *pControl);

public slots:
    void OnRequestExecution(const FreyaData BaseData, void *pRequester);

private:
    QThread             m_DistributionThread;
    FreyaBaseControl    *m_pControl;
};

#endif // FREYACOMMANDDISTRIBUTION_H
