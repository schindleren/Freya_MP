#include "freyabasewidget.h"

FreyaBaseWidget::FreyaBaseWidget(QWidget *parent) :
    QDialog(parent), FreyaAbstractWidget()
{
    setWindowFlags(Qt::Widget);
    setMouseTracking(true);
}
