#ifndef FREYAMSFUNTAB_H
#define FREYAMSFUNTAB_H

#include "freyabaseaction.h"
#include "freyacryptogram.h"
#include "rtabwidget.h"
#include <QWidget>
#include <QLabel>
#include <QToolButton>
#include <QLineEdit>
#include <QTextEdit>

class FreyaCMD;
class FreyaMSFunTab : public RTabWidget, public FreyaBaseAction
{
    Q_OBJECT
public:
    explicit FreyaMSFunTab(QString ResourceName, QWidget *parent = 0);

protected:
    virtual void Execute(const quint64 &command);
    virtual void Execute(FreyaBaseData *pData);

private:
    void InitWidget();

private slots:
    void OnTabChanged(int index);

private:
    QLabel              *m_RemoteInforLab;
    FreyaCMD            *m_CmdWidget;
    QTextEdit           *m_CmdTEdit;
    QLineEdit           *m_CmdLEdit;
    QLabel              *m_RemoteDesktopLab;
    QString             m_ResourceName;
};

#endif // FREYAMSFUNTAB_H
