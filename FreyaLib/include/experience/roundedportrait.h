/*******************************************************************************
 File Name        : roundedportrait.h
 Summary          : a rounded rect with portrait
 Create           : 2015-08-11
 Author           : RenDan
 Update history   :
 NO.     Author        Date         Content
 001     Ren Dan       2015-08-11   Create New
*******************************************************************************/
#ifndef ROUNDEDPORTRAIT_H
#define ROUNDEDPORTRAIT_H

#include "freyalib_global.h"
#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QBitmap>

class FREYALIBSHARED_EXPORT RoundedPortrait : public QWidget
{
    Q_OBJECT

public:
    RoundedPortrait(QString path, QWidget *parent = 0);
    void SetPortraitPath(const QString &path){m_PixmapPath = path;}
    QString GetPortraitPath(){return m_PixmapPath;}
    void SetFalfBorderWidth(int width){m_BorderWidth = width>2?width:2;}
    int GetFalfBorderWidth(){return m_BorderWidth;}
    void SetBorderColor(const QColor &color){m_BorderColor = color;}
    QColor GetBorderColor(){return m_BorderColor;}

protected:
    void paintEvent(QPaintEvent *);

private:
    QString m_PixmapPath;
    int     m_BorderWidth;
    QColor  m_BorderColor;
};

#endif // ROUNDEDPORTRAIT_H
