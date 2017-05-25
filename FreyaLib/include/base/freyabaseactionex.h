#ifndef FREYABASEACTIONEX_H
#define FREYABASEACTIONEX_H

#include "freyalib_global.h"
#include "freyabasecontrol.h"
#include "freyabaseaction.h"
#include <QThread>
#include <QSharedPointer>

class FREYALIBSHARED_EXPORT FreyaBaseActionEx : public QObject, public FreyaBaseAction
{
    Q_OBJECT
public:
    explicit FreyaBaseActionEx(FreyaBaseControl *pControl = NULL, const char *objectName = NULL);
    virtual ~FreyaBaseActionEx();

private:
    void Execute(const quint64 &command);
    void Execute(const FreyaBaseData &data);

protected slots:
    virtual void OnExecuteEx(const quint64 &command);
    virtual void OnExecuteEx(QSharedPointer<FreyaBaseData> data);

signals:
    void ToExecute(const quint64 &command);
    void ToExecute(QSharedPointer<FreyaBaseData> pData);

protected:
    FreyaBaseControl    *m_FreyaBaseControl;
    QThread             *m_FreyaBAExThread;
};

#endif // FREYABASEACTIONEX_H
