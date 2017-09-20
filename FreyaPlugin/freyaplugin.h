#ifndef FREYAPLUGIN_H
#define FREYAPLUGIN_H

#include "freyabaseaction.h"
#include "freyabaseplugin.h"
#include "freyatest_define.h"

#include <QtWidgets>

class FreyaPlugin : public QTextEdit, public FreyaBaseAction
{
    Q_OBJECT

public:
    FreyaPlugin(QWidget *parent = 0);
    ~FreyaPlugin();

protected:
    virtual void Execute(const quint64 &command);
    virtual void Execute(const FreyaData data);

private slots:
    void OnPluginConnected();

private:
    FreyaBasePlugin *m_plugin;
};

#endif // FREYAPLUGIN_H
