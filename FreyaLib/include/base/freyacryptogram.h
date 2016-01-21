#ifndef FREYACRYPTOGRAM_H
#define FREYACRYPTOGRAM_H

#include "freyalib_global.h"
#include <QString>
#include <QByteArray>
#include <QFile>
#include <QCryptographicHash>

class FREYALIBSHARED_EXPORT FreyaCryptogram
{
public:
    explicit FreyaCryptogram();
    static QString CheckSum(const QString &filePath);
    static QString EnCryptogram(const QString &original, int cls = 1);
    static QString DeCryptogram(const QString &confidential, int cls = 1);
    static QByteArray Compress(const QByteArray &original, int cls = 1);
    static QByteArray Uncompress(const QByteArray &confidential, int cls = 1);

private:
    static QString EnCryptogramByCLSA(const QString &original);
    static QString DeCryptogramByCLSA(const QString &confidential);
    static QString EnCryptogramByCLSB(const QString &original);
    static QString DeCryptogramByCLSB(const QString &confidential);
};

#endif // FREYACRYPTOGRAM_H
