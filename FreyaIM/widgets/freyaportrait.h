#ifndef FREYAPORTRAIT_H
#define FREYAPORTRAIT_H

#include <QFile>
#include <QLabel>
#include <QToolButton>
#include <QBoxLayout>
#include <QPixmap>
#include <QPainter>
#include <QBitmap>
#include <QCryptographicHash>
#include <QResizeEvent>
#include <QDebug>

class FreyaPortrait : public QToolButton
{
    Q_OBJECT
public:
    explicit FreyaPortrait(QWidget *parent = 0);
    void SetDefault(const QString &filepath);
    void SetPortrait(const QString &filepath);
    void SetPortraitByBareJid(const QString &BareJid);
    static QString GetPortraitPathByBareJid(const QString &BareJid);
    QString GetPortrait(){return m_currentPath.isEmpty()?m_defaultPath:m_currentPath;}

protected:
    void resizeEvent(QResizeEvent *);

private:
    QString                 m_defaultPath;
    QString                 m_currentPath;
};

#endif // FREYAPORTRAIT_H
