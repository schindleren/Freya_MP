#ifndef FREYASYSTEMTRAYICON_H
#define FREYASYSTEMTRAYICON_H

#include "freyabaseaction.h"
#include <QSystemTrayIcon>
#include <QMenu>
#include <QApplication>

class FreyaSystemTrayIcon : public QSystemTrayIcon, public FreyaBaseAction
{
    Q_OBJECT
public:
    explicit FreyaSystemTrayIcon(QObject *parent = 0);
    ~FreyaSystemTrayIcon();

private:
    void InitBaseMenu();
    void LoadTrayIcon(bool isGray);

protected:
    virtual void Execute();
    virtual void Execute(const quint64 &command);
    virtual void Execute(FreyaBaseData *pData);

private slots:
    void OnTrayActivated(const QSystemTrayIcon::ActivationReason &re);

private:
    QMenu           *m_TrayMenu;
};

#endif // FREYASYSTEMTRAYICON_H
