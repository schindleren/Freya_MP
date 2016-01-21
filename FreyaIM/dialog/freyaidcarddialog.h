#ifndef FREYAIDCARDDIALOG_H
#define FREYAIDCARDDIALOG_H

#include "freyabackwindow.h"
#include "QXmppConfiguration.h"
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QToolButton>
#include <QButtonGroup>
#include <QFileDialog>

class FreyaPortrait;
class CalendarEdit;
class FreyaIDCardDialog : public FreyaBackWindow
{
    Q_OBJECT
public:
    explicit FreyaIDCardDialog(const QString &bareJid, FreyaWindow *parent = 0);
    ~FreyaIDCardDialog();

protected:
    virtual void Execute();
    virtual void Execute(const quint64 &command);
    virtual void Execute(FreyaBaseData *pData);

private:
    void InitWidget();
    void LoadVCard();
    void SaveVCard();

private slots:
    void OnEditModel(bool);
    void OnChangePotrait();

private:
    FreyaPortrait           *m_IDCPortrait;
    QLineEdit               *m_IDCNumLEdit;
    QLineEdit               *m_IDCNickLEdit;
    QButtonGroup            *m_IDCGenderBtnG;
    QToolButton             *m_IDCGenKeepBtn;
    QToolButton             *m_IDCMaleBtn;
    QToolButton             *m_IDCFemaleBtn;
    QLineEdit               *m_IDCEMailLEdit;
    QLineEdit               *m_IDCAddLEdit;
    QLineEdit               *m_IDCUrlLEdit;
    CalendarEdit            *m_IDCBirthLEdit;
    QLineEdit               *m_IDCNameLEdit;
    QLineEdit               *m_IDCPhoneLEdit;
    QPlainTextEdit          *m_IDCDesPEdit;
    QToolButton             *m_IDCEditBtn;

    QString                 m_BareJid;
};

#endif // FREYAIDCARDDIALOG_H
