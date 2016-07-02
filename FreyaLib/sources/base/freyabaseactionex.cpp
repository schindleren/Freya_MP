#include "freyabaseactionex.h"

FreyaBaseActionEx::FreyaBaseActionEx(FreyaBaseControl *pControl, const char *objectName) :
    QObject(), FreyaBaseAction(pControl, objectName), m_FreyaBaseControl(pControl)
{
    connect(this, SIGNAL(ToExecute(quint64)), SLOT(OnExecuteEx(quint64)), Qt::QueuedConnection);
    connect(this, SIGNAL(ToExecute(FreyaBaseData*)), SLOT(OnExecuteEx(FreyaBaseData*)), Qt::QueuedConnection);
    m_thread = new QThread;
    moveToThread(m_thread);
    m_thread->start();
}

FreyaBaseActionEx::~FreyaBaseActionEx()
{
    m_thread->terminate();
    delete m_thread;
}

void FreyaBaseActionEx::Execute(const quint64 &command)
{
    qDebug()<<"Temp ==>" << "FreyaBaseActionEx::Execute" << hex << command;
    emit ToExecute(command);
}

void FreyaBaseActionEx::Execute(FreyaBaseData *pData)
{
    qDebug()<<"Temp ==>" << "FreyaBaseActionEx::Execute" << pData->dataID << hex << pData->command <<pData->arguments;
    emit ToExecute(pData);
}
