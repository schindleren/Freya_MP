#include "freyaim_global.h"
#include "freyacontactdelegate.h"
#include "freyacontacttree.h"

FreyaContactModel::FreyaContactModel(QObject *parent)
    : QStandardItemModel(parent)
{
}

QStandardItem *FreyaContactModel::FindItem(const QString &bareJid)
{
    foreach (QStandardItem *item, m_ContactItem)
    {
        if(bareJid == item->data(FREYAROLE_USERID).toString())
        {
            return item;
        }
    }
    return NULL;
}

void FreyaContactModel::InsertItem(QStandardItem *item)
{
    m_ContactItem.append(item);
}

void FreyaContactModel::RemoveItem(QStandardItem *item)
{
    m_ContactItem.removeOne(item);
}

FreyaContactTree::FreyaContactTree(QWidget *parent)
    : QTreeView(parent), FreyaBaseAction(FreyaBaseControl::GetFreyaControl(), FREYA_OBJ_CONTACTTREE)
{
    InitWidget();
}

void FreyaContactTree::InitWidget()
{
    setFocusPolicy(Qt::NoFocus);
    setIndentation(0);
    setMouseTracking(true);
    setExpandsOnDoubleClick(false);
    setHeaderHidden(true);
    setAutoScroll(true);

    setContextMenuPolicy(Qt::CustomContextMenu);

//    setAcceptDrops(true);
//    setDropIndicatorShown(true);
//    setDragEnabled(true);
//    setDragDropMode(QAbstractItemView::DragDrop);

    setEditTriggers(QTreeView::NoEditTriggers);
    setVerticalScrollMode(QTreeView::ScrollPerPixel);
    m_ContactTreeModel = new FreyaContactModel(this);
    setModel(m_ContactTreeModel);
    FreyaContactDelegate *pDelegate = new FreyaContactDelegate(this);
    setItemDelegate(pDelegate);

    connect(this, SIGNAL(customContextMenuRequested(QPoint)), SLOT(OnContextMenu(QPoint)));
}

QStandardItem* FreyaContactTree::InsertParent(const QString &groupName)
{
    QList<QStandardItem*> parentList = m_ContactTreeModel->findItems(groupName);
    if(parentList.size() == 0)
    {
        QStandardItem* pParentItem = new QStandardItem;
        pParentItem->setData(groupName, Qt::DisplayRole);
        m_ContactTreeModel->appendRow(pParentItem);
        return pParentItem;
    }
    return parentList.at(0);
}

void FreyaContactTree::InsertChild(const QXmppRosterIq::Item &item)
{
    FreyaBaseControl* pControl = FreyaBaseControl::GetFreyaControl();
    pControl->GetConfigFromFile(FREYA_FUN_PERSONCONFIGPATH(item.bareJid()) + FREYA_PTH_PERSONCONFIGPATH);
    QString strDes = pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDDESCRIPTION).toString();

    if(item.groups().isEmpty())
    {
        QStandardItem* pParentItem = InsertParent(tr("Default Group"));
        QStandardItem* pItem = new QStandardItem;
        pItem->setData(item.bareJid(), FREYAROLE_USERID);
        pItem->setData(item.name().isEmpty()?item.bareJid():item.name(), FREYAROLE_USERNAME);
        pItem->setData(strDes, FREYAROLE_USERDES);
        pItem->setData(strDes, Qt::ToolTipRole);
        pParentItem->appendRow(pItem);
        m_ContactTreeModel->InsertItem(pItem);
    }
    else
    {
        foreach (const QString &groupName, item.groups())
        {
            QStandardItem* pParentItem = InsertParent(groupName);
            QStandardItem* pItem = new QStandardItem;
            pItem->setData(item.bareJid(), FREYAROLE_USERID);
            pItem->setData(item.name().isEmpty()?item.bareJid():item.name(), FREYAROLE_USERNAME);
            pItem->setData(strDes, FREYAROLE_USERDES);
            pItem->setData(strDes, Qt::ToolTipRole);
            pParentItem->appendRow(pItem);
            m_ContactTreeModel->InsertItem(pItem);
        }
    }
}

void FreyaContactTree::Execute(const quint64 &/*command*/)
{
}

void FreyaContactTree::Execute(FreyaBaseData *pData)
{
    if(FREYA_CMD_VCARDRESULT == pData->command)
    {
        QStandardItem *treeItem = m_ContactTreeModel->FindItem(pData->arguments.toString());
        if(treeItem)
        {
            FreyaBaseControl* pControl = FreyaBaseControl::GetFreyaControl();
            pControl->GetConfigFromFile(FREYA_FUN_PERSONCONFIGPATH(treeItem->data(FREYAROLE_USERID).toString()) + FREYA_PTH_PERSONCONFIGPATH);
            QString strName = pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDNICKNAME).toString();
            QString strDes = pControl->GetConfig(QStringList()<<FREYA_CFG_CONFIDPERSON<<FREYA_CFG_VCARDDESCRIPTION).toString();
            treeItem->setData(strName, FREYAROLE_USERNAME);
            treeItem->setData(strDes, FREYAROLE_USERDES);
            treeItem->setData(strDes, Qt::ToolTipRole);
        }
    }
}

void FreyaContactTree::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        QModelIndex index = indexAt(e->pos());
        setExpanded(index, !isExpanded(index));
    }
    QTreeView::mouseReleaseEvent(e);
}

void FreyaContactTree::OnContextMenu(const QPoint &pos)
{
    QModelIndex index = indexAt(pos);
    if(index == QModelIndex())
    {
        QMenu menu;
        menu.addAction("SPACE");
        menu.exec(mapToGlobal(pos));
    }
    else if(index.parent() == QModelIndex())
    {
        QMenu menu;
        menu.addAction("GROUP");
        menu.exec(mapToGlobal(pos));
    }
    else
    {
        QMenu menu;
        menu.addAction("CONTACT");
        menu.exec(mapToGlobal(pos));
    }
}
