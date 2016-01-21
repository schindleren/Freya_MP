#include "freyams_global.h"
#include "freyamswidget.h"
#include "freyamsfuntab.h"
#include "QXmppConfiguration.h"

FreyaMSWidget::FreyaMSWidget(QWidget *parent) :
    QWidget(parent), FreyaBaseAction(FreyaBaseControl::GetFreyaControl(), FREYAMS_OBJ_MSWIDGET)
{
    InitWidget();
}

void FreyaMSWidget::Execute(const quint64 &/*command*/)
{
}

void FreyaMSWidget::Execute(FreyaBaseData *pData)
{
    if(FREYAMS_CMD_PRESENCE == pData->command)
    {
        QStringList ResourcesList = pData->arguments.toStringList();
        if(ResourcesList.size() > 0)
        {
            m_ListModel->clear();
            foreach (const QString &Res, ResourcesList)
            {
                QStandardItem *item = new QStandardItem(Res);
                if(Res.startsWith(FREYAMS_INF_DEFCONFRESOURCE))
                {
                    m_ListModel->insertRow(0, item);
                }
                else
                {
                    m_ListModel->appendRow(item);
                }
            }
        }
    }
}

void FreyaMSWidget::keyPressEvent(QKeyEvent *e)
{
    if((e->key() == Qt::Key_L) && (e->modifiers() == Qt::ControlModifier))
    {
        FREYA_REQUESTEXECUTION(FREYAMS_CMD_LOCKED);
    }
    QWidget::keyPressEvent(e);
}

void FreyaMSWidget::showEvent(QShowEvent *e)
{
    setFocus();
    QWidget::showEvent(e);
}

void FreyaMSWidget::InitWidget()
{
    setMouseTracking(true);
    QHBoxLayout *MainHLay = new QHBoxLayout(this);
    MainHLay->setSpacing(0);
    MainHLay->setMargin(0);
    m_ListSplit = new QSplitter(this);
    m_ListSplit->setObjectName("FreyaMSListSplit");
    m_ListSplit->setChildrenCollapsible(false);
    m_ListSplit->setMouseTracking(true);
    MainHLay->addWidget(m_ListSplit);

    m_UserList = new QListView(m_ListSplit);
    m_UserList->setObjectName("FreyaMSUserList");
    m_UserList->setMouseTracking(true);
    m_UserList->setEditTriggers(QListView::NoEditTriggers);
    m_ListSplit->addWidget(m_UserList);
    connect(m_UserList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(OnActiveSession(QModelIndex)));

    m_ListModel = new QStandardItemModel(m_UserList);
    m_UserList->setModel(m_ListModel);

    m_UserFunTab = new RTabWidget(m_ListSplit, true);
    m_UserFunTab->setObjectName("FreyaMSUserFunTab");
    m_UserFunTab->SetTabHeight(40);
    m_UserFunTab->setMouseTracking(true);
    m_UserFunTab->SetTabSite(RTabSite::eSiteTop);
    m_UserFunTab->SetTabsCloseable(true);
    m_UserFunTab->SetTabsCloseBtnSize(QSize(40, 40));
    m_UserFunTab->setMinimumWidth(260);
    connect(m_UserFunTab, SIGNAL(ToCloseBtnClicked(int)), this, SLOT(OnCloseFunTab(int)));
    m_ListSplit->addWidget(m_UserFunTab);
    m_ListSplit->setSizes(QList<int>()<<1<<2);
}

void FreyaMSWidget::OnActiveSession(const QModelIndex &index)
{
    QString resource = m_ListModel->data(index).toString();
    if(!m_ActiveSessionMap.contains(resource))
    {
        FreyaMSFunTab *pFunTab = new FreyaMSFunTab(resource, m_UserFunTab);
        m_UserFunTab->AddTabAndWidget(pFunTab, "FreyaMSFunTab", resource);
        m_UserFunTab->OnCurrentTab(m_UserFunTab->Index(pFunTab));
        m_ActiveSessionMap.insert(resource, pFunTab);
    }
    else
    {
        m_UserFunTab->OnCurrentTab(m_UserFunTab->Index(m_ActiveSessionMap.value(resource)));
    }
}

void FreyaMSWidget::OnCloseFunTab(int index)
{
    QWidget *pFunTab = m_UserFunTab->Widget(index);
    m_ActiveSessionMap.remove(m_ActiveSessionMap.key(pFunTab));
    m_UserFunTab->RemoveTab(index);
    pFunTab->deleteLater();
}
