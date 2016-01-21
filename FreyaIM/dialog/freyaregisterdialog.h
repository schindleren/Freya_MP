#ifndef FREYAREGISTERDIALOG_H
#define FREYAREGISTERDIALOG_H

#include "freyabackwindow.h"
#include "QXmppRegisterIq.h"
#include <QLineEdit>
#include <QToolButton>
#include <QRegExpValidator>
#include <QMovie>
#include <QCryptographicHash>
#include <QSystemTrayIcon>

class FreyaRegisterDialog : public FreyaBackWindow
{
    Q_OBJECT
public:
    explicit FreyaRegisterDialog(FreyaBackWindow *parent = 0);
    ~FreyaRegisterDialog();

private:
    void InitWidget();
    void RegisterError(int code);
    void RegisterResult();
    void RegisterResultShow(bool isSuccess, const QString &infor);

protected:
    virtual void Execute();
    virtual void Execute(const quint64 &command);
    virtual void Execute(FreyaBaseData *pData);

private slots:
    void OnChangeVerifierCode();
    void OnCheckInformation();

private:
    QLineEdit           *m_AccountLEdit;
    QLineEdit           *m_EmailLEdit;
    QLineEdit           *m_PassWordLEdit;
    QLineEdit           *m_RePassWordLEdit;
    QLineEdit           *m_VerifierLEdit;
    QLabel              *m_VerifierLab;
    QToolButton         *m_ChangeVerifierBtn;
    QToolButton         *m_AcceptBtn;
    QToolButton         *m_RejectBtn;

    QWidget             *m_RegisterInforWidget;
    QLabel              *m_RegisterInforLab;
    QLabel              *m_RegisterMovieLab;
    QMovie              *m_RegisterMovie;
    QToolButton         *m_RegisterGoBtn;

    QString             m_VerifierCode;
    QString             m_DataId;
};

#endif // FREYAREGISTERDIALOG_H
