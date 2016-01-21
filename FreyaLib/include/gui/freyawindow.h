#ifndef FREYAWINDOW_H
#define FREYAWINDOW_H

#include "freyalib_global.h"
#include "freyabasedialog.h"
#include "freyabaseaction.h"
#include "freyabasecontrol.h"
#include <QApplication>

class FREYALIBSHARED_EXPORT FreyaWindow : public FreyaBaseDialog, public FreyaBaseAction
{
    Q_OBJECT
public:
    explicit FreyaWindow(FreyaWindow *parent = 0, FreyaBaseControl *pControl = NULL, const char *objectName = NULL);
    ~FreyaWindow();
    void appendToDestoryList(FreyaWindow* window);
    void removeToDestoryList(FreyaWindow* window);
    static void PromptAnyWindow(QWidget *widget);

private:
    FreyaWindow             *m_Parent;
    QList<FreyaWindow*>     m_DestoryList;
};

#endif // FREYAWINDOW_H
