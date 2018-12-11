#include "freyabaseactionex.h"

FreyaBaseActionEx::FreyaBaseActionEx(FreyaBaseControl *pControl, const char *objectName) :
    QObject(), FreyaBaseAction(pControl, objectName), m_FreyaBaseControl(pControl)
{
    connect(this, SIGNAL(ToExecute(FreyaData)), SLOT(OnExecuteEx(FreyaData)), Qt::QueuedConnection);
    m_FreyaBAExThread = new QThread;
    moveToThread(m_FreyaBAExThread);
    m_FreyaBAExThread->start();
}

FreyaBaseActionEx::~FreyaBaseActionEx()
{
    qDebug() << "FreyaLib > " << m_FreyaBaseControl->GetActionObjectName(this) << this << "terminate";
    m_FreyaBAExThread->terminate();
    m_FreyaBAExThread->wait();
    delete m_FreyaBAExThread;
}

void FreyaBaseActionEx::Execute(const FreyaData data)
{
    emit ToExecute(data);
}

void FreyaBaseActionEx::OnExecuteEx(const FreyaData data)
{
    qDebug() << "FreyaLib > " << "ActionEx_Execute:" << hex << data->command << data->dataID;
}

void FreyaBaseActionEx::OnRegisterCommands()
{
    m_FreyaBaseControl->RegisterCommand(this, m_CommandList);
}

void FreyaBaseActionEx::RegisterCommands()
{
    // Register command to control
    FreyaData registerData = FreyaBaseData::CreateDate(FREYALIB_CMD_CMDREGREQUEST);
    this->Execute(registerData);
    QTimer::singleShot(3000, this, SLOT(OnRegisterCommands()));
}
