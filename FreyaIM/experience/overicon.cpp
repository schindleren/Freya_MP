/*******************************************************************************
 File Name        : overicon.h
 Summary          : Overlay the icons
 Create           : 2013-01-19
 Author           : RenDan
 Update history   :
 NO.     Author        Date         Content
 001     Ren Dan       2013-01-19   Create New
*******************************************************************************/
#include "overicon.h"

OverIcon::OverIcon(QObject *parent) :
    QObject(parent)
{
}

QImage OverIcon::OverImage(const QList<QImage> &imageList)
{
    if(imageList.size() > 0)
    {
        QImage image(imageList.at(0).size(), QImage::Format_ARGB32_Premultiplied);

        QPainter painter(&image);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        painter.eraseRect(image.rect());

        foreach(const QImage &img, imageList)
        {
            painter.drawImage(QRect(QPoint(0, 0), image.size()), img);
        }
        return image;
    }
    QImage image(QSize(0, 0), QImage::Format_ARGB32_Premultiplied);
    return image;
}

QImage OverIcon::Decoloration(const QImage &colorImage)
{
    QImage grayImage(colorImage.size(), QImage::Format_ARGB32_Premultiplied);

    for(int i = 0; i<colorImage.width(); i++)
    {
        for(int j = 0; j<colorImage.height(); j++)
        {
            QRgb rgb = colorImage.pixel(i, j);
            grayImage.setPixel(i, j, qRgba(qGray(rgb), qGray(rgb), qGray(rgb), qAlpha(rgb)));
        }
    }
    return grayImage;
}
