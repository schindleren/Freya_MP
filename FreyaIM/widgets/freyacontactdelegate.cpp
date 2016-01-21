#include "freyaim_global.h"
#include "freyaportrait.h"
#include "freyacontactdelegate.h"

FreyaContactWidget::FreyaContactWidget(QWidget *parent) :
    QWidget(parent)
{
    initWidget();
}

void FreyaContactWidget::SetContactPortrait(const QString &bareJid)
{
    m_ContactPortrait->SetPortraitByBareJid(bareJid);
}

void FreyaContactWidget::SetContactName(const QString &name)
{
    m_ContactNameLab->setText(name);
}

void FreyaContactWidget::SetContactDes(const QString &text)
{
    m_ContactDesLab->setText(text);
}

void FreyaContactWidget::SetWidgetState(bool isLite)
{
    m_ContactWidgetLay->setDirection(isLite?(QBoxLayout::LeftToRight):(QBoxLayout::TopToBottom));
    m_ContactPortrait->setMinimumSize(isLite? QSize(LITEHEIGHT, LITEHEIGHT) :
                                              QSize(NORMALHEIGHT, NORMALHEIGHT));
    m_ContactNameLab->setMaximumWidth(isLite? 120 : width());
}

void FreyaContactWidget::initWidget()
{
    setObjectName("FreyaContactWidget");
    setMouseTracking(true);
    QHBoxLayout *pMainHLay = new QHBoxLayout(this);
    pMainHLay->setSpacing(0);
    pMainHLay->setMargin(0);
    m_ContactWidgetLay = new QBoxLayout(QBoxLayout::LeftToRight);
    m_ContactWidgetLay->setSpacing(0);
    m_ContactWidgetLay->setMargin(0);

    m_ContactPortrait = new FreyaPortrait(this);
    m_ContactPortrait->SetDefault(FREYA_PTH_GLOBALRESPATH FREYA_PTH_DEFPORTRAITPATH);
    pMainHLay->addWidget(m_ContactPortrait);
    m_ContactNameLab = new QLabel(this);
    m_ContactNameLab->setObjectName("ContactNameLab");
    m_ContactWidgetLay->addWidget(m_ContactNameLab);
    m_ContactDesLab = new QLabel(this);
    m_ContactDesLab->setObjectName("ContactDesLab");
    m_ContactDesLab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_ContactWidgetLay->addWidget(m_ContactDesLab);
    pMainHLay->addLayout(m_ContactWidgetLay);
}

///////////////////////

FreyaContactDelegate::FreyaContactDelegate(QWidget *parent)
    : QStyledItemDelegate(parent)
{
}

void FreyaContactDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    const QStandardItemModel *pModel = qobject_cast<const QStandardItemModel*>(index.model());
    QStandardItem* pItem = NULL;
    QStandardItem* pParentItem = NULL;
    if(pModel)
    {
        if(index.parent() == QModelIndex())
        {
            pParentItem = pModel->item(index.row());
        }
        else
        {
            pItem = pModel->item(index.parent().row())->child(index.row());
        }
    }
    if(pItem)
    {
        FreyaContactWidget widget;
        if (option.state & QStyle::State_Selected)
        {
            pItem->setSizeHint(QSize(0, NORMALHEIGHT));
            widget.SetWidgetState(false);
        }
        else
        {
            pItem->setSizeHint(QSize(0, LITEHEIGHT));
            widget.SetWidgetState(true);
        }
        widget.setGeometry(option.rect);
        widget.SetContactName(index.data(FREYAROLE_USERNAME).toString());
        widget.SetContactDes(index.data(FREYAROLE_USERDES).toString());
        widget.SetContactPortrait(index.data(FREYAROLE_USERID).toString());
        painter->save();
        painter->translate(option.rect.topLeft());
        widget.render(painter);
        painter->restore();
    }
    else if(pParentItem)
    {
        pParentItem->setSizeHint(QSize(26, 26));
        QLabel Parentlab;
        Parentlab.setText(index.data(FREYAROLE_USERNAME).toString());
        Parentlab.setGeometry(option.rect);
        painter->save();
        painter->translate(option.rect.topLeft());
        Parentlab.render(painter);
        painter->restore();
    }
    QStyledItemDelegate::paint(painter, option, index);
}
