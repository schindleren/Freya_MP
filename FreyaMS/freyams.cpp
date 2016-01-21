#include "freyamsdialog.h"
#include "freyamsconfig.h"
#include "freyamsclient.h"
#include "freyams.h"
#include "freyams_global.h"

FreyaMS::FreyaMS(QObject *parent)
    : QObject(parent), FreyaBaseAction(FreyaBaseControl::GetFreyaControl(), FREYAMS_OBJ_FREYAMS)
{
}

FreyaMS::~FreyaMS()
{
}

void FreyaMS::InitApplication()
{
    LoadStyle(FREYAMS_PTH_DEFAULTSTYLEPATH);
    InitObject();
    InitWidget();
}

void FreyaMS::Execute(const quint64 &/*command*/)
{
}

void FreyaMS::Execute(FreyaBaseData */*pData*/)
{
}

void FreyaMS::InitObject()
{
    FreyaMSConfig nFreyaConfigData;
    nFreyaConfigData.InitConfig();

    m_FreyaMSClient = new FreyaMSClient(this);
}

void FreyaMS::InitWidget()
{
    m_FreyaMSDialog = new FreyaMSDialog;
    m_FreyaMSDialog->SetObjectName("FreyaMSDialog");
    m_FreyaMSDialog->show();
}

void FreyaMS::LoadStyle(const QString &filepath)
{
    QFile file(filepath);
    if(file.open(QFile::ReadOnly | QFile::Text))
    {
        qApp->setStyleSheet(file.readAll());
        file.close();
    }
}
