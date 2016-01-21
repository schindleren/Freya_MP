#include "freyawindow.h"

FreyaWindow::FreyaWindow(FreyaWindow *parent, FreyaBaseControl *pControl, const char *objectName) :
    FreyaBaseDialog(0), FreyaBaseAction(pControl, objectName)
{
    m_Parent = parent;
    if(m_Parent)
    {
        m_Parent->appendToDestoryList(this);
    }
}

FreyaWindow::~FreyaWindow()
{
    if(m_Parent)
    {
        m_Parent->removeToDestoryList(this);
    }

    QListIterator<FreyaWindow*> destoryIt(m_DestoryList);
    while(destoryIt.hasNext())
    {
        delete destoryIt.next();
    }
}

void FreyaWindow::appendToDestoryList(FreyaWindow *window)
{
    m_DestoryList.append(window);
}

void FreyaWindow::removeToDestoryList(FreyaWindow *window)
{
    m_DestoryList.removeOne(window);
}

void FreyaWindow::PromptAnyWindow(QWidget *widget)
{
    widget->show();
    qApp->setActiveWindow(widget);
    widget->setFocus();
    widget->activateWindow();
}
