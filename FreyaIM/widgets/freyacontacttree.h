#ifndef FREYACONTACTTREE_H
#define FREYACONTACTTREE_H

#include "freyabaseaction.h"
#include "freyabasecontrol.h"
#include "QXmppRosterManager.h"
#include <QTreeView>
#include <QWidget>
#include <QMouseEvent>
#include <QStandardItem>
#include <QMenu>
#include <QStandardItemModel>

class FreyaContactDelegate;
class FreyaContactModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit FreyaContactModel(QObject *parent = 0);
    QStandardItem *FindItem(const QString &bareJid);
    void InsertItem(QStandardItem *item);
    void RemoveItem(QStandardItem *item);

private:
    QList<QStandardItem*>       m_ContactItem;
};

class FreyaContactTree : public QTreeView, public FreyaBaseAction
{
    Q_OBJECT
public:
    explicit FreyaContactTree(QWidget *parent = 0);
    QStandardItem *InsertParent(const QString &groupName);
    void InsertChild(const QXmppRosterIq::Item &item);
    void RemoveParent(const QString &groupName);
    void RemoveChild(const QString &BareJid);

protected:
    virtual void Execute(const quint64 &command);
    virtual void Execute(FreyaBaseData *pData);

    void mouseReleaseEvent(QMouseEvent *);

private:
    void InitWidget();

private slots:
    void OnContextMenu(const QPoint &pos);

private:
    FreyaContactModel                   *m_ContactTreeModel;
};

#endif // FREYACONTACTTREE_H
