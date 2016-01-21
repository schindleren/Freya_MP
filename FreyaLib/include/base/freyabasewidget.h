#ifndef FREYABASEWIDGET_H
#define FREYABASEWIDGET_H

#include "freyalib_global.h"
#include "freyaabstractwidget.h"
#include <QDialog>

class FREYALIBSHARED_EXPORT FreyaBaseWidget : public QDialog, public FreyaAbstractWidget
{
    Q_OBJECT
public:
    explicit FreyaBaseWidget(QWidget *parent = 0);
};

#endif // FREYABASEWIDGET_H
