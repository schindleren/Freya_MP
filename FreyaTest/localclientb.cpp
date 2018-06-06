#include "localclientb.h"
#include "freyatest_global.h"

LocalClientB::LocalClientB()
    : QObject(NULL) , FreyaBaseAction(FreyaBaseControl::GetFreyaControl(), FYTS_OBJ_CLIENTB)
{
}

void LocalClientB::Execute(const quint64 &/*command*/)
{
}

void LocalClientB::Execute(const FreyaData /*data*/)
{
}
