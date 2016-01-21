#include "freyascreencmd.h"
#include "freyabasecontrol.h"
#include "QXmppPresence.h"

FreyaScreenCMD::FreyaScreenCMD(QObject *parent)
    : QObject(parent)
{
    m_ScreenTimer = new QTimer(this);
    m_ScreenTimer->setInterval(1000);
    m_Rect = qApp->desktop()->availableGeometry();
    connect(m_ScreenTimer, SIGNAL(timeout()), this, SLOT(OnOutput()));
}

void FreyaScreenCMD::ParseCommand(const QVariantMap &commandMap)
{
    if(commandMap.contains(FREYAMS_STR_CMDREQUESTSCREEN))
    {
        OnOutput();
    }
    else if(commandMap.contains(FREYAMS_STR_CMDAUTOSCREEN))
    {
        m_ScreenTimer->start();
    }
    else if(commandMap.contains(FREYAMS_STR_CMDSTOPSCREEN))
    {
        m_ScreenTimer->stop();
    }
}

void FreyaScreenCMD::OnOutput()
{
    QPixmap pix(qApp->primaryScreen()->grabWindow(qApp->desktop()->winId(), 0, 0, m_Rect.width(), m_Rect.height()).
                scaled(m_Rect.width() / 2, m_Rect.height() / 2, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    QByteArray ba;
    QDataStream ds(&ba, QIODevice::ReadWrite);
    ds<<pix;
    QByteArray ba2 = FreyaCryptogram::Compress(ba, 9);
    qDebug()<<"AAAAAA"<<ba.size();
    qDebug()<<"DDDDDD"<<ba2.size();
    QVariantMap output;
    output.insert(FREYAMS_STR_CMDRESULTSCREEN, ba2);
    emit ToOutput(output);
}
