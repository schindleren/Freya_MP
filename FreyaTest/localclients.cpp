#include "localclients.h"
#include "freyatest_global.h"

LocalClientS::LocalClientS()
    : FreyaBaseActionEx(FreyaBaseControl::GetFreyaControl(), FYTS_OBJ_CLIENTS)
{
}

void LocalClientS::OnExecuteEx(const quint64 &command)
{
    if(command == FYTS_CMD_QUIT)
    {
        qDebug() << "LocalClientS: " << "FYTS_CMD_QUIT";
    }
    else if(command == FYTS_CMD_HAPPY)
    {
        qDebug() << "LocalClientS: " << "FYTS_CMD_HAPPY";
    }
    else if(command == FYTS_CMD_SAD)
    {
        qDebug() << "LocalClientS: " << "FYTS_CMD_SAD";
    }
    else if(command == FYTS_CMD_CRY)
    {
        qDebug() << "LocalClientS: " << "FYTS_CMD_CRY";
    }
}

void LocalClientS::OnExecuteEx(FreyaData data)
{
    quint64 cmd64 = data->command;
    if(cmd64 == FYTS_CMD_QUIT)
    {
        qDebug() << "LocalClientS: " << "FYTS_CMD_QUIT" << data->dataID;
    }
    else if(cmd64 == FYTS_CMD_HAPPY)
    {
        qDebug() << "LocalClientS: " << "FYTS_CMD_HAPPY" << data->dataID;
    }
    else if(cmd64 == FYTS_CMD_SAD)
    {
        qDebug() << "LocalClientS: " << "FYTS_CMD_SAD" << data->dataID;
    }
    else if(cmd64 == FYTS_CMD_CRY)
    {
        qDebug() << "LocalClientS: " << "FYTS_CMD_CRY" << data->dataID;
    }
}
