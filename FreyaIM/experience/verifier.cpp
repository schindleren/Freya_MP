/*******************************************************************************
 File Name        : verifier.cpp
 Summary          : make a verifier code
 Create           : 2015-05-21
 Author           : RenDan
 Update history   :
 NO.     Author        Date         Content
 001     Ren Dan       2015-05-21   Create New
*******************************************************************************/
#include "verifier.h"
#include <QDebug>

Verifier::Verifier(QObject *parent) :
    QObject(parent)
{
}

Verifier::~Verifier()
{
}

QPixmap Verifier::VerifierCodePixmap(QString &code, const QSize &size)
{
    qsrand(QDateTime::currentMSecsSinceEpoch());
    if(code.isEmpty())
    {
        QString baseString = "abcdefghijklmnopqrstuvwxyz0123456789";
        for(int i = 0; i < 4; ++i)
        {
            code.append(baseString.at(qrand() % baseString.length()));
        }
    }
    QPixmap pix(size);
    pix.fill(Qt::transparent);
    int TextSize = qMin(size.width() / code.length(), size.height());
    QPainter painter(&pix);
    QPen pen;
    pen.setWidth(2);

    QFont font;
    font.setPixelSize(TextSize);
    font.setFamily("Arial");
    painter.setFont(font);

    for(int i = 0; i < code.length(); ++i)
    {
        pen.setColor(QColor(qrand() % 200, qrand() % 200, qrand() % 200));
        painter.setPen(pen);
        QRect rect(TextSize*i, 0, TextSize, TextSize);
        painter.drawText(QRect(QPoint(qMax((rect.x() + (qrand() % 10 - 5)), 0),
                                      rect.y() + (qrand() % 10 - 5)), rect.size()),
                         (qrand() % 2)?code.at(i):code.at(i).toUpper());
    }

    pen.setColor(QColor(qrand() % 255, qrand() % 255, qrand() % 255));
    painter.setPen(pen);
    painter.drawLine(QPoint(qrand() % size.width(), qrand() % size.height()),
                     QPoint(qrand() % size.width(), qrand() % size.height()));

    pen.setColor(QColor(qrand() % 255, qrand() % 255, qrand() % 255));
    painter.setPen(pen);
    painter.drawLine(QPoint(qrand() % size.width(), qrand() % size.height()),
                     QPoint(qrand() % size.width(), qrand() % size.height()));
    return pix;
}
