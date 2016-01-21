#include "freyaim_global.h"
#include "freyaportrait.h"

#define BORDERWIDTH 4

FreyaPortrait::FreyaPortrait(QWidget *parent) :
    QToolButton(parent)
{
    setObjectName("PortraitBtn");
}

void FreyaPortrait::SetDefault(const QString &filepath)
{
    m_defaultPath = filepath;
}

void FreyaPortrait::SetPortrait(const QString &filepath)
{
    m_currentPath = QFile::exists(filepath) ? filepath : m_defaultPath;
    QIcon icon;
    icon.addPixmap(QPixmap(m_currentPath), QIcon::Normal, QIcon::On);
    icon.addPixmap(QPixmap(m_currentPath), QIcon::Disabled, QIcon::On);
    setIcon(icon);
}

void FreyaPortrait::SetPortraitByBareJid(const QString &BareJid)
{
    SetPortrait(GetPortraitPathByBareJid(BareJid));
}

QString FreyaPortrait::GetPortraitPathByBareJid(const QString &BareJid)
{
    return QString(FREYA_FUN_PERSONCONFIGPATH(BareJid) + FREYA_PTH_DEFPORTRAITPATH);
}

void FreyaPortrait::resizeEvent(QResizeEvent *e)
{
    setIconSize(QSize(e->size().width() - BORDERWIDTH*2, e->size().height() - BORDERWIDTH*2));
    QToolButton::resizeEvent(e);
}
