#ifndef FREYASCREENCMD_H
#define FREYASCREENCMD_H

#include "freyaim_global.h"
#include "freyacryptogram.h"
#include <QObject>
#include <QTimer>
#include <QScreen>
#include <QPixmap>
#include <QLabel>
#include <QApplication>
#include <QDesktopWidget>

class FreyaScreenCMD : public QObject
{
    Q_OBJECT

public:
    FreyaScreenCMD(QObject *parent = 0);
    void ParseCommand(const QVariantMap &commandMap);

private slots:
    void OnOutput();

signals:
    void ToOutput(const QVariantMap &output);

private:
    QTimer              *m_ScreenTimer;
    QRect               m_Rect;
};

#endif // FREYASCREENCMD_H
