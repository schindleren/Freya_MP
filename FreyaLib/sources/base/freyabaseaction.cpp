#include "freyabaseaction.h"

FreyaBaseAction::FreyaBaseAction(FreyaBaseControl *pControl, const char *objectName) :
    FreyaAbstractAction(), m_FreyaBaseControl(pControl), m_CommandList(QList<quint64>())
{
    // Register this action to control
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

void FreyaBaseAction::RegisterCommands()
{
    // Register command to control
    FreyaData registerData = FreyaBaseData::CreateDate(FREYALIB_CMD_CMDREGREQUEST);
    this->Execute(registerData);
    m_FreyaBaseControl->RegisterCommand(this, m_CommandList);
}
