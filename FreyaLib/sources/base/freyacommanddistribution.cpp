#include "freyacommanddistribution.h"
#include "freyabasecontrol.h"
#include "freyabaseactionex.h"

FreyaCommandDistribution::FreyaCommandDistribution(FreyaBaseControl *pControl) :
    QThread(NULL), m_pControl(pControl)
{
}

QStringList FreyaCommandDistribution::DataList()
{
    return m_DataList;
}

bool FreyaCommandDistribution::InsertData(FreyaData data)
{
    bool ret = m_DataList.contains(data->dataID);
    m_DataList.append(data->dataID);
    m_DataQueue.insert(data->dataID, data);
    return ret;
}

FreyaData FreyaCommandDistribution::FindData(const QString &dataId)
{
    return m_DataQueue.value(dataId, FreyaData(NULL));
}

void FreyaCommandDistribution::run()
{
    while(!m_DataList.isEmpty())
    {
        FreyaData BaseData = m_DataQueue.take(m_DataList.takeFirst());
        m_pControl->m_pRequester = VARIANTTOCUSTOMCLS(BaseData->GetArgument(FREYALIB_FLG_REQUESTER), void);

//        FreyaBaseAction *reqAction = static_cast<FreyaBaseAction*>(pRequester);

        qDebug() << "FreyaLib > " << "Execution:" << "DataID:" << BaseData->dataID
                 << "Command:" << hex << BaseData->command << dec << "Arguments:" << BaseData->GetArgument() << "From:" << m_pControl->m_pRequester;
        QHashIterator<QString, FreyaBaseAction*> ActionIt(m_pControl->m_FreyaPublicRegister.AllRegisterAction());
        while (ActionIt.hasNext())
        {
            ActionIt.next();
            FreyaBaseAction *pAction = ActionIt.value();
            if(pAction && m_pControl->m_FreyaPublicRegister.CheckObjectCommand(pAction, BaseData->command))
            {
//                if(!(reqAction && reqAction == pAction))
                {
                    QMetaObject::invokeMethod(m_pControl, "ActionExecution", Qt::QueuedConnection, Q_ARG(FreyaBaseAction*, pAction), Q_ARG(FreyaData, BaseData));
                }
            }
        }
        m_pControl->m_pRequester = NULL;
    }
}
