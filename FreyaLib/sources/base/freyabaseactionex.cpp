#include "freyabaseactionex.h"

FreyaBaseActionEx::FreyaBaseActionEx(FreyaBaseControl *pControl, const char *objectName) :
    QObject(), FreyaBaseAction(pControl, objectName), m_FreyaBaseControl(pControl)
{
    connect(this, SIGNAL(ToExecute(quint64)), SLOT(OnExecuteEx(quint64)), Qt::QueuedConnection);
    connect(this, SIGNAL(ToExecute(QSharedPointer<FreyaBaseData>)), SLOT(OnExecuteEx(QSharedPointer<FreyaBaseData>)), Qt::QueuedConnection);
    m_FreyaBAExThread = new QThread;
    moveToThread(m_FreyaBAExThread);
    m_FreyaBAExThread->start();
}

FreyaBaseActionEx::~FreyaBaseActionEx()
{
    qDebug()<<"FreyaLib > "<<this<<"terminate";
    m_FreyaBAExThread->terminate();
    delete m_FreyaBAExThread;
}

void FreyaBaseActionEx::Execute(const quint64 &command)
{
    emit ToExecute(command);
}

void FreyaBaseActionEx::Execute(const FreyaBaseData &data)
{
    QSharedPointer<FreyaBaseData> pSharedData(new FreyaBaseData(data));
    emit ToExecute(pSharedData);
}

void FreyaBaseActionEx::OnExecuteEx(const quint64 &command)
{
    qDebug()<<"FreyaLib > "<<"ActionEx_Execute:" << hex << command;
}

void FreyaBaseActionEx::OnExecuteEx(QSharedPointer<FreyaBaseData> data)
{
    qDebug()<<"FreyaLib > "<<"ActionEx_Execute:"<< hex << data->command << data->dataID;
}
