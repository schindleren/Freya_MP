#include "freyastatebtn.h"

FreyaStateBtn::FreyaStateBtn(QWidget *parent)
    : QPushButton(parent)
{
    setObjectName("PresenceStateBtn");
    m_StateMenu = new QMenu(this);
    m_StateMenu->setObjectName("PresenceStateMenu");
    setMenu(m_StateMenu);
    connect(m_StateMenu, SIGNAL(triggered(QAction*)), this, SLOT(OnStateChanged(QAction*)));
}

FreyaStateBtn::~FreyaStateBtn()
{
}

void FreyaStateBtn::LoadStates(const QVariantMap &stateMap)
{
    m_actionGroup = new QActionGroup(m_StateMenu);
    QMapIterator<QString, QVariant> stateMapIT(stateMap);
    while (stateMapIT.hasNext())
    {
        int state = stateMapIT.next().value().toInt();
        QAction *stateAction = m_actionGroup->addAction(stateMap.key(state));
        stateAction->setCheckable(true);
        m_stateActionMap.insert(state, stateAction);
    }
    m_StateMenu->addActions(m_stateActionMap.values());
}

void FreyaStateBtn::SetState(int state)
{
    QAction* curAction = m_stateActionMap.value(state);
    if(curAction)
    {
        curAction->setChecked(true);
        StateChanged(curAction);
    }
}

int FreyaStateBtn::GetState()
{
    return m_stateActionMap.key(m_actionGroup->checkedAction());
}

void FreyaStateBtn::StateChanged(QAction *curAction)
{
    setText(curAction->text());
}

void FreyaStateBtn::OnStateChanged(QAction* curAction)
{
    StateChanged(curAction);
    emit ToStateChanged(m_stateActionMap.key(m_actionGroup->checkedAction()));
}
