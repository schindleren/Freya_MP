#include "localclienta.h"
#include "freyatest_global.h"
#include <QApplication>

LocalClientA::LocalClientA(QWidget *parent)
    : QTextEdit(parent) , FreyaBaseAction(FreyaBaseControl::GetFreyaControl(), FYTS_OBJ_CLIENTA)
{
}

void LocalClientA::Execute(const quint64 &command)
{
    switch (command)
    {
    case FYTS_CMD_QUIT:
    {
        qApp->quit();
    }
        break;
    default:
        break;
    }
}

void LocalClientA::Execute(const FreyaData data)
{
    Q_UNUSED(data)
}
