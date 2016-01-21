/*******************************************************************************
 File Name        : singleapplication.cpp
 Summary          : Limit application running count
 Create           : 2015-04-28
 Author           : RenDan
 Update history   :
 NO.     Author        Date         Content
 001     Ren Dan       2015-04-28   Create New
*******************************************************************************/
#include "singleapplication.h"

#define TIMERINTERVAL 1000

SysSemaphore::SysSemaphore(QString AppKey, int Count, QObject *parent) :
    QThread(parent), m_isGetResource(false)
{
    m_SystemSema = new QSystemSemaphore(AppKey, Count);
}

SysSemaphore::~SysSemaphore()
{
    m_SystemSema->release();
    delete m_SystemSema;
}

void SysSemaphore::run()
{
    m_isGetResource = m_SystemSema->acquire();
}

SingleApplication::SingleApplication(QString AppKey, int Count, QObject *parent) :
    QObject(parent)
{
    m_TimeLine = new QTimer(this);
    m_TimeLine->setSingleShot(true);
    connect(m_TimeLine, SIGNAL(timeout()), this, SLOT(OnReturnState()));
    m_SysResource = new SysSemaphore(AppKey, Count, this);
    m_SysResource->start();
    m_TimeLine->start(TIMERINTERVAL);
}

/***************************************************
  Author: RenDan
  Date: 2015-04-28
  Description: emit state of appkey registered
  Input: void
  Output: void
  Return: void
****************************************************/
void SingleApplication::OnReturnState()
{
    emit ToReturnState(m_SysResource->SemaphoreState());
    if(m_SysResource->isRunning())
    {
        disconnect(m_SysResource, SIGNAL(finished()), this, SLOT(OnReturnState()));
        m_SysResource->terminate();
    }
    if(m_TimeLine->isActive())
    {
        m_TimeLine->stop();
    }
}
