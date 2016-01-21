#ifndef FREYAMS_H
#define FREYAMS_H

#include "freyabaseaction.h"
#include <QFile>
#include <QObject>
#include <QApplication>

class FreyaMSConfig;
class FreyaMSDialog;
class FreyaMSClient;
class FreyaMS : public QObject, public FreyaBaseAction
{
    Q_OBJECT

public:
    FreyaMS(QObject *parent = 0);
    ~FreyaMS();
    void InitApplication();

protected:
    virtual void Execute(const quint64 &command);
    virtual void Execute(FreyaBaseData *pData);

private:
    void InitObject();
    void InitWidget();
    void LoadStyle(const QString &filepath);

private:
    FreyaMSDialog               *m_FreyaMSDialog;
    FreyaMSClient               *m_FreyaMSClient;
};

#endif // FREYAMS_H
