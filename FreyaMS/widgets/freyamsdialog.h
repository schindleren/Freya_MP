#ifndef FREYAMSDIALOG_H
#define FREYAMSDIALOG_H

#include "freyawindow.h"
#include <QStackedWidget>
#include <QLabel>
#include <QToolButton>
#include <QTimer>
#include <QBoxLayout>

struct FreyaInfor;
class FreyaMSLoginWidget;
class FreyaMSWidget;
class FreyaMSDialog : public FreyaWindow
{
    Q_OBJECT
public:
    explicit FreyaMSDialog(FreyaWindow *parent = 0);

protected:
    virtual void Execute(const quint64 &command);
    virtual void Execute(FreyaBaseData *pData);

    void closeEvent(QCloseEvent *);

private:
    void InitWidget();
    void ShowInforWithTimer(const FreyaInfor &infor);

private slots:
    void OnStateTimer();

private:
    QWidget             *m_BackWidget;
    QLabel              *m_LogoLab;
    QLabel              *m_TitleLab;
    QToolButton         *m_MinSizeBtn;
    QToolButton         *m_MaxSizeBtn;
    QToolButton         *m_CloseBtn;
    QStackedWidget      *m_BackStackedWidget;
    QLabel              *m_StateLab;
    QTimer              *m_StateTimer;

    FreyaMSLoginWidget  *m_LoginWidget;
    FreyaMSWidget       *m_MSWidget;
};

#endif // FREYAMSDIALOG_H
