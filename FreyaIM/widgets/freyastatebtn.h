#ifndef FREYASTATEBTN_H
#define FREYASTATEBTN_H

#include <QPushButton>
#include <QActionGroup>
#include <QAction>
#include <QDebug>
#include <QMenu>

class FreyaStateBtn : public QPushButton
{
    Q_OBJECT
public:
    explicit FreyaStateBtn(QWidget *parent = 0);
    ~FreyaStateBtn();
    void LoadStates(const QVariantMap &stateMap);
    void SetState(int state);
    int GetState();

private:
    void StateChanged(QAction*);

signals:
    void ToStateChanged(int);

private slots:
    void OnStateChanged(QAction*);

private:
    QMenu                   *m_StateMenu;
    QMap<int, QAction*>     m_stateActionMap;
    QActionGroup            *m_actionGroup;
};

#endif // FREYASTATEBTN_H
