#include "freyabaseaction.h"

FreyaBaseAction::FreyaBaseAction(FreyaBaseControl *pControl, const char *objectName) :
    FreyaAbstractAction(), m_FreyaBaseControl(pControl)
{
    if(NULL != objectName)
    {
        if(NULL != m_FreyaBaseControl /*&& NULL != m_FreyaBaseControl->parent()*/)
        {
            m_FreyaBaseControl->RegisterObject(this, objectName);
        }
    }
}

FreyaBaseAction::~FreyaBaseAction()
{
    if(NULL != m_FreyaBaseControl /*&& NULL != m_FreyaBaseControl->parent()*/)
    {
        m_FreyaBaseControl->UnRegisterObject(this);
    }
}

void FreyaBaseAction::Execute()
{
}

void FreyaBaseAction::Execute(const quint64 &/*command*/)
{
}

void FreyaBaseAction::Execute(FreyaBaseData */*pData*/)
{
}
