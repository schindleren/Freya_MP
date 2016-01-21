#include "freyacryptogram.h"

FreyaCryptogram::FreyaCryptogram()
{
}

QString FreyaCryptogram::CheckSum(const QString &filePath)
{
    QFile file(filePath);
    if(file.open(QFile::ReadOnly))
    {
        QString fileMd5 = QCryptographicHash::hash(file.readAll(), QCryptographicHash::Md5).toHex();
        file.close();
        return fileMd5.toLower();
    }
    return "";
}

QString FreyaCryptogram::EnCryptogram(const QString &original, int cls)
{
    switch (cls)
    {
    case 0:
        return original;
    case 1:
        return EnCryptogramByCLSA(original);
    case 2:
        return EnCryptogramByCLSB(original);
    default:
        return original;
    }
}

QString FreyaCryptogram::DeCryptogram(const QString &confidential, int cls)
{
    switch (cls)
    {
    case 0:
        return confidential;
    case 1:
        return DeCryptogramByCLSA(confidential);
    case 2:
        return DeCryptogramByCLSB(confidential);
    default:
        return confidential;
    }
}

QByteArray FreyaCryptogram::Compress(const QByteArray &original, int cls)
{
    return (original.isEmpty()) ? "" : qCompress(original, cls);
}

QByteArray FreyaCryptogram::Uncompress(const QByteArray &confidential, int /*cls*/)
{
    return (confidential.isEmpty()) ? "" : qUncompress(confidential);
}

QString FreyaCryptogram::EnCryptogramByCLSA(const QString &original)
{
    return (original.isEmpty()) ? "" : QString::fromUtf8(qCompress(original.toUtf8(), 1).toHex());
}

QString FreyaCryptogram::DeCryptogramByCLSA(const QString &confidential)
{
    return (confidential.isEmpty()) ? "" : QString::fromUtf8(qUncompress(QByteArray::fromHex(confidential.toUtf8())));
}

QString FreyaCryptogram::EnCryptogramByCLSB(const QString &original)
{
    return (original.isEmpty()) ? "" : QString::fromUtf8(qCompress(original.toUtf8(), 1).toHex());
}

QString FreyaCryptogram::DeCryptogramByCLSB(const QString &confidential)
{
    return (confidential.isEmpty()) ? "" : QString::fromUtf8(qUncompress(QByteArray::fromHex(confidential.toUtf8())));
}
