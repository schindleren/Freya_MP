#ifndef FREYAMSWIDGET_H
#define FREYAMSWIDGET_H

#include "freyabaseaction.h"
#include "rtabwidget.h"
#include <QWidget>
#include <QListView>
#include <QStandardItemModel>
#include <QKeyEvent>
#include <QBoxLayout>
#include <QMap>
#include <QSplitter>

class FreyaMSFunTab;
class FreyaMSWidget : public QWidget, public FreyaBaseAction
{
    Q_OBJECT
public:
    explicit FreyaMSWidget(QWidget *parent = 0);

protected:
    virtual void Execute(const quint64 &command);
    virtual void Execute(FreyaBaseData *pData);

    void keyPressEvent(QKeyEvent *);
    void showEvent(QShowEvent *);

private:
    void InitWidget();

private slots:
    void OnActiveSession(const QModelIndex &index);
    void OnCloseFunTab(int index);

private:
    QSplitter                       *m_ListSplit;
    QListView                       *m_UserList;
    QStandardItemModel              *m_ListModel;
    RTabWidget                      *m_UserFunTab;
    QMap<QString, QWidget*>         m_ActiveSessionMap;
};

#endif // FREYAMSWIDGET_H
