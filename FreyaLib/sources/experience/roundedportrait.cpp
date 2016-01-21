/*******************************************************************************
 File Name        : roundedportrait.cpp
 Summary          : a rounded rect with portrait
 Create           : 2015-08-11
 Author           : RenDan
 Update history   :
 NO.     Author        Date         Content
 001     Ren Dan       2015-08-11   Create New
*******************************************************************************/
#include "roundedportrait.h"

RoundedPortrait::RoundedPortrait(QString path, QWidget *parent)
    : QWidget(parent),m_PixmapPath(path),m_BorderWidth(2),m_BorderColor(Qt::white)
{
}

void RoundedPortrait::paintEvent(QPaintEvent *e)
{
    QPixmap pix(m_PixmapPath);
    pix = pix.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    int Xoffset = 0;
    int Yoffset = 0;
    if(pix.width() > width())
    {
        Xoffset = (pix.width()-width())/2;
    }
    else if(pix.height() > height())
    {
        Yoffset = (pix.height()-height())/2;
    }
    if((Xoffset + Yoffset) != 0)
    {
        pix = pix.copy(Xoffset, Yoffset, width(), height());
    }

    QPixmap maskPix(pix.size());
    maskPix.fill(Qt::transparent);
    QPainter maskPaint(&maskPix);
    maskPaint.setBrush(Qt::black);
    maskPaint.setPen(Qt::NoPen);
    maskPaint.drawEllipse(QRect(1, 1, width()-2, height()-2));
    pix.setMask(maskPix.mask());

    QPainter thisPaint(this);
    thisPaint.setRenderHint(QPainter::Antialiasing);
    thisPaint.drawPixmap(rect(), pix);
    QPen pen;
    pen.setWidth(m_BorderWidth*2);
    pen.setColor(m_BorderColor);
    thisPaint.setPen(pen);
    thisPaint.drawEllipse(QRect(m_BorderWidth, m_BorderWidth,
                                width()-m_BorderWidth*2, height()-m_BorderWidth*2));

    QWidget::paintEvent(e);
}
