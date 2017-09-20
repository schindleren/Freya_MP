#ifndef FREYABASEACTION_H
#define FREYABASEACTION_H

#include "freyalib_global.h"
#include "freyabasecontrol.h"
#include "freyaabstractaction.h"

class FREYALIBSHARED_EXPORT FreyaBaseAction : public FreyaAbstractAction
{
public:
    explicit FreyaBaseAction(FreyaBaseControl *pControl = NULL, const char *objectName = NULL);
    virtual ~FreyaBaseAction();
    void *FreyaRequester(){return m_FreyaBaseControl->FreyaRequester();}

    virtual void Execute();
    virtual void Execute(const quint64 &command);
    virtual void Execute(const FreyaData data);

protected:
    FreyaBaseControl *m_FreyaBaseControl;
};

#endif // FREYABASEACTION_H
