/*******************************************************************************
 File Name        : calendaredit.cpp
 Summary          : LineEdit with CalendarWidget
 Create           : 2015-06-04
 Author           : RenDan
 Update history   :
 NO.     Author        Date         Content
 001     Ren Dan       2015-06-04   Create New
*******************************************************************************/
#include "calendaredit.h"

CalendarEdit::CalendarEdit(QWidget *parent) :
    QLineEdit(parent)
{
    m_CalendarFormat = "yyyy-MM-dd";
    setReadOnly(true);
    m_CalendarBtn = new QToolButton(this);
    m_CalendarBtn->setObjectName("CalendarBtn");
    connect(m_CalendarBtn, SIGNAL(clicked()), this, SLOT(OnShowCalendar()));
    m_CalendarWidget = new QCalendarWidget(this);
    connect(m_CalendarWidget, SIGNAL(clicked(QDate)) ,this, SLOT(OnCalendarClicked(QDate)));
    m_CalendarWidget->setWindowFlags(m_CalendarWidget->windowFlags() | Qt::Popup);
}

void CalendarEdit::resizeEvent(QResizeEvent *e)
{
    m_CalendarBtn->setGeometry(width() - height(), 0, height(), height());
    QLineEdit::resizeEvent(e);
}

void CalendarEdit::OnShowCalendar()
{
    m_CalendarWidget->setSelectedDate(QDate::fromString(text(), m_CalendarFormat));
    m_CalendarWidget->move(mapToGlobal(QPoint(0, height())));
    m_CalendarWidget->show();
}

void CalendarEdit::OnCalendarClicked(const QDate &date)
{
    setText(date.toString(m_CalendarFormat));
    m_CalendarWidget->hide();
}
