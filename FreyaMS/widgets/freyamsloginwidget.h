#ifndef FREYAMSLOGINWIDGET_H
#define FREYAMSLOGINWIDGET_H

#include "freyabaseaction.h"
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include <QBoxLayout>
#include <QKeyEvent>
#include <QDateTime>

class FreyaMSLoginWidget : public QWidget, public FreyaBaseAction
{
    Q_OBJECT
public:
    explicit FreyaMSLoginWidget(QWidget *parent = 0);

protected:
    virtual void Execute(const quint64 &command);
    virtual void Execute(FreyaBaseData *pData);

    void keyPressEvent(QKeyEvent *);
    void showEvent(QShowEvent *);

private:
    void InitWidget();

private slots:
    void OnLogin();
    void OnCancelLogin();

private:
    QWidget                 *m_FunWidget;
    QLineEdit               *m_HostLEdit;
    QLineEdit               *m_PortLEdit;
    QLineEdit               *m_DomianLEdit;
    QLineEdit               *m_ResourceLEdit;

    QLineEdit               *m_AccountLEdit;
    QLineEdit               *m_PassWDLEdit;

    QToolButton             *m_LoginBtn;
    QToolButton             *m_CancelLoginBtn;
};

#endif // FREYAMSLOGINWIDGET_H
