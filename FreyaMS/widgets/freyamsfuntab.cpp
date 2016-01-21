#include "freyams_global.h"
#include "freyamsfuntab.h"
#include "freyacmd.h"

FreyaMSFunTab::FreyaMSFunTab(QString ResourceName, QWidget *parent) :
    RTabWidget(parent, false),FreyaBaseAction(FreyaBaseControl::GetFreyaControl(), FREYAMS_OBJ_MSFUNTAB),
    m_ResourceName(ResourceName)
{
    InitWidget();
}

void FreyaMSFunTab::Execute(const quint64 &/*command*/)
{
}

void FreyaMSFunTab::Execute(FreyaBaseData *pData)
{
    if(FREYAMS_CMD_RECEIVECMD == pData->command)
    {
        QVariantMap commandMap = pData->arguments.toMap();
        if(commandMap.value(FREYAMS_STR_RESOURCENAME) == m_ResourceName)
        {
            if(commandMap.contains(FREYAMS_STR_CMDRESULTSCREEN))
            {
                QDataStream ds(FreyaCryptogram::Uncompress(commandMap.value(FREYAMS_STR_CMDRESULTSCREEN).toByteArray()));
                QPixmap pix;
                ds>>pix;
                m_RemoteDesktopLab->setPixmap(pix.scaled(m_RemoteDesktopLab->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
        }
    }
}

void FreyaMSFunTab::InitWidget()
{
    SetTabSite(RTabSite::eSiteRight);
    SetTabBKWidth(120);
    connect(this ,SIGNAL(ToTabChanged(int)), SLOT(OnTabChanged(int)));

    m_RemoteInforLab = new QLabel(this);
    m_RemoteInforLab->setObjectName("RemoteInforLab");
    AddTabAndWidget(m_RemoteInforLab, "RemoteInforLab", tr("Remote Info"));
    OnCurrentTab(0);

    m_CmdWidget = new FreyaCMD(m_ResourceName, this);
    AddTabAndWidget(m_CmdWidget, "FreyaCmdWidget", tr("Remote CMD"));

    m_RemoteDesktopLab = new QLabel(this);
    AddTabAndWidget(m_RemoteDesktopLab, "RemoteDesktopLab", tr("Remote Desktop"));
}

void FreyaMSFunTab::OnTabChanged(int index)
{
    QWidget *tabPage = Widget(index);
    if(m_RemoteDesktopLab == tabPage)
    {
        FreyaBaseData pData;
        pData.command = FREYAMS_CMD_EXCUTECMD;
        QVariantMap commandMap;
        commandMap.insert(FREYAMS_STR_RESOURCENAME, m_ResourceName);
        commandMap.insert(FREYAMS_STR_CMDAUTOSCREEN, "");
        pData.arguments = commandMap;
        FREYA_REQUESTEXECUTION(&pData);
    }
}
