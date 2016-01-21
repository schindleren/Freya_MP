/*******************************************************************************
 File Name        : verifier.h
 Summary          : make a verifier code
 Create           : 2015-05-21
 Author           : RenDan
 Update history   :
 NO.     Author        Date         Content
 001     Ren Dan       2015-05-21   Create New
*******************************************************************************/
#ifndef VERIFIER_H
#define VERIFIER_H

#include <QObject>
#include <QPainter>
#include <QPixmap>
#include <QDateTime>

class Verifier : public QObject
{
    Q_OBJECT
public:
    explicit Verifier(QObject *parent = 0);
    ~Verifier();
    static QPixmap VerifierCodePixmap(QString &code, const QSize &size);
};

#endif // VERIFIER_H
