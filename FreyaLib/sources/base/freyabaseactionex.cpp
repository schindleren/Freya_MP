#include "freyabaseactionex.h"

FreyaBaseActionEx::FreyaBaseActionEx(FreyaBaseControl *pControl, const char *objectName) :
    QObject(), FreyaBaseAction(pControl, objectName), m_FreyaBaseControl(pControl)
{
    connect(this, SIGNAL(ToExecute(quint64)), SLOT(OnExecuteEx(quint64)), Qt::QueuedConnection);
    connect(this, SIGNAL(ToExecute(QString)), SLOT(OnExecuteEx(QString)), Qt::QueuedConnection);
    m_thread = new QThread;
    moveToThread(m_thread);
    m_thread->start();
}

FreyaBaseActionEx::~FreyaBaseActionEx()
{
    qDebug()<<"Temp=>"<<this<<"terminate";
    m_thread->terminate();
    delete m_thread;
}

void FreyaBaseActionEx::Execute(const quint64 &command)
{
    emit ToExecute(command);
}

void FreyaBaseActionEx::Execute(FreyaBaseData *pData)
{
    emit ToExecute(pData->dataID);
}

void FreyaBaseActionEx::OnExecuteEx(const quint64 &command)
{
    qDebug()<<"ActionEx_Execute:" << hex << command;
}

void FreyaBaseActionEx::OnExecuteEx(const QString &DataId)
{
    qDebug()<<"ActionEx_Execute:" << DataId;
}
