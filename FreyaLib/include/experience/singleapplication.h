/*******************************************************************************
 File Name        : singleapplication.h
 Summary          : Limit application running count
 Create           : 2015-04-28
 Author           : RenDan
 Update history   :
 NO.     Author        Date         Content
 001     Ren Dan       2015-04-28   Create New
*******************************************************************************/
#ifndef SINGLEAPPLICATION_H
#define SINGLEAPPLICATION_H

#include "freyalib_global.h"
#include <QThread>
#include <QTimer>
#include <QObject>
#include <QSystemSemaphore>

class SysSemaphore : public QThread
{
public:
    explicit SysSemaphore(QString AppKey, int Count = 1, QObject *parent = 0);
    ~SysSemaphore();
    bool SemaphoreState(){return m_isGetResource;}
    void run();

private:
    QSystemSemaphore            *m_SystemSema;
    bool                        m_isGetResource;
};

class FREYALIBSHARED_EXPORT SingleApplication : public QObject
{
    Q_OBJECT
public:
    explicit SingleApplication(QString AppKey, int Count = 1, QObject *parent = 0);

signals:
    void ToReturnState(bool);

private slots:
    void OnReturnState();

private:
    QTimer                      *m_TimeLine;
    SysSemaphore                *m_SysResource;
};

#endif // SINGLEAPPLICATION_H
