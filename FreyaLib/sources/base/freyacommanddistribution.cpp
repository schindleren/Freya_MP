#include "freyacommanddistribution.h"
#include "freyabasecontrol.h"
#include "freyabaseactionex.h"

FreyaCommandDistribution::FreyaCommandDistribution(FreyaBaseControl *pControl) :
    QObject(NULL), m_pControl(pControl)
{
    moveToThread(&m_DistributionThread);
    m_DistributionThread.start();
}

void FreyaCommandDistribution::OnRequestExecution(const FreyaData BaseData, void *pRequester)
{
    m_pControl->InsertFreyaData(BaseData);
    m_pControl->m_pRequester = pRequester;
//    FreyaBaseAction *reqAction = static_cast<FreyaBaseAction*>(pRequester);

    qDebug() << "FreyaLib > " << "Execution:" << "DataID:" << BaseData->dataID
             << "Command:" << hex << BaseData->command << dec << "Arguments:" << BaseData->GetArgument() << "From:" << m_pControl->m_pRequester;
    QHashIterator<QString, FreyaBaseAction*> ActionIt(m_pControl->m_FreyaPublicRegister.AllRegisterAction());
    while (ActionIt.hasNext())
    {
        ActionIt.next();
        FreyaBaseAction *pAction = ActionIt.value();
        if(pAction && m_pControl->m_FreyaPublicRegister.CheckObjectCommand(pAction, BaseData->command))
        {
//            if(!(reqAction && reqAction == pAction))
            {
                QMetaObject::invokeMethod(m_pControl, "ActionExecution", Qt::BlockingQueuedConnection, Q_ARG(FreyaBaseAction*, pAction), Q_ARG(FreyaData, BaseData));
            }
        }
    }
    m_pControl->m_pRequester = NULL;
}
