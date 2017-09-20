#ifndef LOCALCLIENTA_H
#define LOCALCLIENTA_H

#include <QtWidgets>

#include "freyabaseaction.h"

class LocalClientA : public QTextEdit, public FreyaBaseAction
{
    Q_OBJECT

public:
    LocalClientA(QWidget *parent = 0);

protected:
    virtual void Execute(const quint64 &command);
    virtual void Execute(const FreyaData data);

};

#endif // LOCALCLIENTA_H
