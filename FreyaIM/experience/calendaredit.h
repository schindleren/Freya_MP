/*******************************************************************************
 File Name        : calendaredit.h
 Summary          : LineEdit with CalendarWidget
 Create           : 2015-06-04
 Author           : RenDan
 Update history   :
 NO.     Author        Date         Content
 001     Ren Dan       2015-06-04   Create New
*******************************************************************************/
#ifndef CALENDAREDIT_H
#define CALENDAREDIT_H

#include <QLineEdit>
#include <QToolButton>
#include <QBoxLayout>
#include <QCalendarWidget>
#include <QResizeEvent>

class CalendarEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit CalendarEdit(QWidget *parent = 0);
    void SetDateFormat(const QString &format){m_CalendarFormat = format;}
    QString GetDateFormat(){return m_CalendarFormat;}
    void SetDate(const QDate &date){setText(date.toString(m_CalendarFormat));}
    QDate GetDate(){return QDate::fromString(text(), m_CalendarFormat);}
    void SetDisabled(bool isDisable){m_CalendarBtn->setDisabled(isDisable);}

protected:
    void resizeEvent(QResizeEvent *);

private slots:
    void OnShowCalendar();
    void OnCalendarClicked(const QDate &date);

private:
    QToolButton             *m_CalendarBtn;
    QCalendarWidget         *m_CalendarWidget;
    QString                 m_CalendarFormat;
};

#endif // CALENDAREDIT_H
