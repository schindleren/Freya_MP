#ifndef FREYABASEDIALOG_H
#define FREYABASEDIALOG_H

#include "freyalib_global.h"
#include "dragpanel.h"
#include "freyaabstractwidget.h"

class FREYALIBSHARED_EXPORT FreyaBaseDialog : public DragPanel, public FreyaAbstractWidget
{
    Q_OBJECT
public:
    explicit FreyaBaseDialog(QWidget *parent = 0);
};

#endif // FREYABASEDIALOG_H
