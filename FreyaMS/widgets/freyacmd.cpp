#include "freyams_global.h"
#include "freyacmd.h"

FreyaCMD::FreyaCMD(QString ResourceName, QWidget *parent) :
    QWidget(parent), FreyaBaseAction(FreyaBaseControl::GetFreyaControl(), FREYAMS_OBJ_MSCMDWIDGET),
    m_ResourceName(ResourceName)
{
    InitWidget();
}

FreyaCMD::~FreyaCMD()
{
}

void FreyaCMD::Execute(const quint64 &/*command*/)
{

}

void FreyaCMD::Execute(FreyaBaseData *pData)
{
    if(FREYAMS_CMD_RECEIVECMD == pData->command)
    {
        QVariantMap commandMap = pData->arguments.toMap();
        if(commandMap.value(FREYAMS_STR_RESOURCENAME) == m_ResourceName)
        {
            if(commandMap.contains(FREYAMS_STR_CMDRECEIVE))
            {
                m_CommandTEdit->append(commandMap.value(FREYAMS_STR_CMDRECEIVE).toString());
            }
            else if(commandMap.contains(FREYAMS_STR_CMDRECEIVEBEGIN))
            {
                m_WorkPath = commandMap.value(FREYAMS_STR_CMDRECEIVEBEGIN).toString();
                m_CommandTEdit->append(m_WorkPath + "$ ");
            }
            else if(commandMap.contains(FREYAMS_STR_CMDRECEIVEFINISH))
            {
                m_CommandLEdit->setDisabled(false);
                m_CommandLEdit->setFocus();
            }
        }
    }
}

void FreyaCMD::keyPressEvent(QKeyEvent *e)
{
    if((e->key() == Qt::Key_C) && (e->modifiers() == Qt::ControlModifier))
    {
        ExcuteCommand(FREYAMS_STR_CMDSTOPCURRENTCMD);
    }
    QWidget::keyPressEvent(e);
}

void FreyaCMD::InitWidget()
{
    setFocusPolicy(Qt::StrongFocus);
    QVBoxLayout *pMainVLay = new QVBoxLayout(this);
    pMainVLay->setSpacing(10);
    pMainVLay->setMargin(0);
    m_CommandTEdit = new QTextEdit(this);
    m_CommandTEdit->setReadOnly(true);
    m_CommandTEdit->setFocusPolicy(Qt::NoFocus);
    pMainVLay->addWidget(m_CommandTEdit);
    m_CommandLEdit = new QLineEdit(this);
    pMainVLay->addWidget(m_CommandLEdit);
    connect(m_CommandLEdit, SIGNAL(returnPressed()), this, SLOT(OnReturned()));
}

void FreyaCMD::ExcuteCommand(const QString &command)
{
    if(!command.compare("cls", Qt::CaseInsensitive) || !command.compare("exit", Qt::CaseInsensitive))
    {
        m_CommandTEdit->clear();
        m_CommandTEdit->append(m_WorkPath + "$ ");
        m_CommandLEdit->setDisabled(false);
        m_CommandLEdit->setFocus();
    }
    else
    {
        FreyaBaseData pData;
        pData.command = FREYAMS_CMD_EXCUTECMD;
        QVariantMap commandMap;
        commandMap.insert(FREYAMS_STR_RESOURCENAME, m_ResourceName);
        if(!m_WorkPath.isEmpty())
        {
            commandMap.insert(FREYAMS_STR_COMMANDPATH, m_WorkPath);
        }
        commandMap.insert(FREYAMS_STR_COMMANDLINE, command);
        pData.arguments = commandMap;
        FREYA_REQUESTEXECUTION(&pData);
    }
}

void FreyaCMD::OnReturned()
{
    if(!m_CommandLEdit->text().isEmpty())
    {
        m_CommandTEdit->append(m_WorkPath + "$ " + m_CommandLEdit->text());

        m_CommandLEdit->setDisabled(true);
        ExcuteCommand(m_CommandLEdit->text());
        m_CommandLEdit->clear();
    }
}
