#ifndef FREYABASEACTION_H
#define FREYABASEACTION_H

#include "freyalib_global.h"
#include "freyabasecontrol.h"
#include "freyaabstractaction.h"

// FREYALIB_CMD_CMDREGREQUEST == 0x0000000060000000 //FreyaLib_Cmd_CommandRegisterRequest
#define FREYACONNECT(cmd, func)    if(cmd == data->command) \
                                   { func; return; } \
                                   else if(0x0000000060000000 == data->command) \
                                   { m_CommandList.append(cmd); }

class FREYALIBSHARED_EXPORT FreyaBaseAction : public FreyaAbstractAction
{
public:
    explicit FreyaBaseAction(FreyaBaseControl *pControl = NULL, const char *objectName = NULL);
    virtual ~FreyaBaseAction();
    void *FreyaRequester(){return m_FreyaBaseControl->FreyaRequester();}

    virtual void Execute(){}
    virtual void Execute(const FreyaData /*data*/){}

protected:
    virtual void RegisterCommands();

protected:
    FreyaBaseControl    *m_FreyaBaseControl;
    QList<quint64>      m_CommandList;
};

#endif // FREYABASEACTION_H
