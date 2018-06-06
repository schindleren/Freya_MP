#include "freyalaunchtesta.h"

FRYLAC_FUN_LAUNCHPORT(FreyaLaunchTestA, FREYALAUNCHTESTASHARED_EXPORT, "FreyaLaunchTestA:info")

FreyaLaunchTestA::FreyaLaunchTestA():
    FreyaBaseActionEx(FreyaBaseControl::GetFreyaControl(), "FreyaLaunchTestA")
{
    RegisterCommands();

    QTimer::singleShot(5000, this, SLOT(OnSendTest()));
    FREYA_REQUESTEXECUTION(FRYTA_CMD_TEST_1);
    FREYA_REQUESTEXECUTION(FRYTA_CMD_TEST_2);
}

void FreyaLaunchTestA::OnExecuteEx(const FreyaData data)
{
    qDebug() << "FreyaLaunchTestA >" << hex << data->command << FreyaBaseControl::GetFreyaControl()->FreyaRequester();
    auto fun1 = [=]{
        qDebug() << "FreyaLaunchTestA >" << "FreyaLaunchTestA : FRYTA_CMD_TEST_1";
    };

    FREYACONNECT(FRYTA_CMD_TEST_1, fun1();)
}

void FreyaLaunchTestA::OnSendTest()
{
    FREYA_REQUESTEXECUTION(FRYTA_CMD_TEST_1);
    FREYA_REQUESTEXECUTION(FRYTA_CMD_TEST_2);
}
