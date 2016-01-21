/*******************************************************************************
 File Name        : overicon.cpp
 Summary          : Overlay the icons
 Create           : 2013-01-19
 Author           : RenDan
 Update history   :
 NO.     Author        Date         Content
 001     Ren Dan       2013-01-19   Create New
*******************************************************************************/
#ifndef OVERICON_H
#define OVERICON_H

#include <QtWidgets>

class OverIcon : public QObject
{
    Q_OBJECT
public:
    explicit OverIcon(QObject *parent = 0);
    static QImage OverImage(const QList<QImage> &imageList);
    static QImage Decoloration(const QImage &colorImage);
};

#endif // OVERICON_H
