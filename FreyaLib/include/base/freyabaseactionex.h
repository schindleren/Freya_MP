#ifndef FREYABASEACTIONEX_H
#define FREYABASEACTIONEX_H

#include "freyalib_global.h"
#include "freyabasecontrol.h"
#include "freyabaseaction.h"
#include <QThread>
#include <QTimer>
#include <QSharedPointer>

class FREYALIBSHARED_EXPORT FreyaBaseActionEx : public QObject, public FreyaBaseAction
{
    Q_OBJECT
public:
    explicit FreyaBaseActionEx(FreyaBaseControl *pControl = NULL, const char *objectName = NULL);
    virtual ~FreyaBaseActionEx();

private:
    void Execute(const FreyaData data);

protected:
    virtual void RegisterCommands();

protected slots:
    virtual void OnExecuteEx(const FreyaData data);
    void OnRegisterCommands();

signals:
    void ToExecute(const FreyaData pData);

protected:
    FreyaBaseControl    *m_FreyaBaseControl;
    QThread             *m_FreyaBAExThread;
};

#endif // FREYABASEACTIONEX_H
