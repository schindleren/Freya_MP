#ifndef FREYALAUNCH_H
#define FREYALAUNCH_H

#include "freyabasecontrol.h"
#include "freyabaseextension.h"
#include "freyabaseactionex.h"

#include <QtCore>

class FreyaLaunch : public FreyaBaseAction
{
public:
    FreyaLaunch(QString launchKey = "");
    ~FreyaLaunch();

    void launch();

protected:
    virtual void Execute(const FreyaData data);

private:
    void LoadModules();
    bool LoadLibrary(const QString &filePath);

private:
    QString                             m_LaunchKey;
    QList<FreyaBaseControl *>           m_ControlList;
    QList<QLibrary*>                    m_ModuleList;
};

#endif // FREYALAUNCH_H
