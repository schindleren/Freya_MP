#ifndef FREYACMD_H
#define FREYACMD_H

#include "freyabaseaction.h"
#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QProcess>
#include <QDir>
#include <QTimer>
#include <QBoxLayout>
#include <QKeyEvent>

class FreyaCMD : public QWidget, public FreyaBaseAction
{
    Q_OBJECT

public:
    FreyaCMD(QString ResourceName, QWidget *parent = 0);
    ~FreyaCMD();

protected:
    virtual void Execute(const quint64 &command);
    virtual void Execute(FreyaBaseData *pData);

    void keyPressEvent(QKeyEvent *);

private:
    void InitWidget();
    void ExcuteCommand(const QString &command);

public slots:
    void OnReturned();

private:
    QTextEdit           *m_CommandTEdit;
    QLineEdit           *m_CommandLEdit;
    QString             m_WorkPath;
    QString             m_ResourceName;
};

#endif // FREYACMD_H
