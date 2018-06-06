#include "freyalaunchtestb.h"

FRYLAC_FUN_LAUNCHPORT(FreyaLaunchTestB, FREYALAUNCHTESTBSHARED_EXPORT, "{\"ModuleName\":\"B\",\"ModuleVerison\":\"0.0.0.1\"}")

FreyaLaunchTestB::FreyaLaunchTestB()
{
    qDebug()<<"FreyaLaunchTestB"<<"FreyaLaunchTestB";
    new FreyaTestB();
}

FreyaTestB::FreyaTestB()
    :FreyaBaseAction(FreyaBaseControl::GetFreyaControl(), "FreyaTestB")
{

}

void FreyaTestB::Execute(const FreyaData data)
{
    qDebug() << "FreyaTestB >" << hex << data->command << FreyaBaseControl::GetFreyaControl()->FreyaRequester();
}
