#ifndef FREYADEFAULTCMD_H
#define FREYADEFAULTCMD_H

#include "freyaim_global.h"
#include <QObject>
#include <QDir>
#include <QTimer>
#include <QProcess>

class FreyaDefaultCMD : public QObject
{
    Q_OBJECT

public:
    FreyaDefaultCMD(QObject *parent = 0);
    void ParseCommand(const QVariantMap &commandMap);

private:
    void SendOutput(const QString &ReceiveState, const QString &text = "");
    void TurnToDir(const QString &targetDir);

signals:
    void ToOutput(const QVariantMap &output);

private slots:
    void OnOutput();
    void OnStopOutput();

private:
    QProcess            *m_CommandProcess;
    QTimer              *m_CommandTimer;
};

#endif // FREYADEFAULTCMD_H
