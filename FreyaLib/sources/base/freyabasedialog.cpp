#include "freyabasedialog.h"

FreyaBaseDialog::FreyaBaseDialog(QWidget *parent) :
    DragPanel(parent, Qt::LeftButton),FreyaAbstractWidget()
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowSystemMenuHint | Qt::Window);
    resize(800, 600);
}
