#ifndef FREYABASECONTROL_H
#define FREYABASECONTROL_H

#include "freyalib_global.h"
#include "freyaabstractcontrol.h"
#include "freyapublicregister.h"
#include "freyacommanddistribution.h"

#include <vector>
#include <QDebug>
#include <QThread>

#define CUSTOMCLSTOVARIANT(pObject)             QVariant::fromValue((void*)pObject)
#define VARIANTTOCUSTOMCLS(pObject, ClassName)  static_cast<ClassName*>(pObject.value<void*>())

#define FREYA_REQUESTEXECUTION_NON              FreyaBaseControl::GetFreyaControl()->RequestExecution(this)
#define FREYA_REQUESTEXECUTION(data)            FreyaBaseControl::GetFreyaControl()->RequestExecution(data, this)

class FreyaPublicRegister;
class FreyaBaseAction;
class FREYALIBSHARED_EXPORT FreyaBaseControl : public QObject, public FreyaAbstractControl
{
    Q_OBJECT
    friend class FreyaCommandDistribution;
private:
    explicit FreyaBaseControl();

public:
    ~FreyaBaseControl();

    QVariantMap GetConfigFromFile(const QString &filePath);
    bool SetConfigToFile(const QString &filePath, const QVariantMap &varmap);
    QVariant GetConfig(const QStringList &configPath);
    bool SetConfig(const QStringList &configPath, const QVariant &var);
    bool InsertConfig(const QStringList &configPath, const QVariant &var);
    bool RemoveConfig(const QStringList &configPath);

    bool RegisterObject(FreyaBaseAction *actObject, const char *objectName);
    bool UnRegisterObject(const QString &objectName);
    bool UnRegisterObject(FreyaBaseAction *actObject);
    bool RegisterCommand(FreyaBaseAction *actObject, QList<quint64> commandList);
    bool UnRegisterCommand(FreyaBaseAction *actObject);
    FreyaBaseAction *GetActionObject(const QString &objectName);
    QString GetActionObjectName(FreyaBaseAction *actObject);
    void DeleteAllAction(const QList<FreyaBaseAction*> &except = QList<FreyaBaseAction*>());
    void DeleteAllAction(const QStringList &except);

    bool InsertFreyaData(const FreyaData pData);
    FreyaData FindFreyaData(const QString &dataID);
    FreyaData TakeFreyaData(const QString &dataID);

    bool RequestExecution(void *pRequester = NULL);
    void RequestExecution(const quint64 &command, void *pRequester = NULL);
    void RequestExecution(const FreyaData BaseData, void *pRequester = NULL);

    void *FreyaRequester(){return m_pRequester;}

signals:
    void ToRequestExecution(const FreyaData BaseData, void *pRequester = NULL);

private slots:
    void ActionExecution(FreyaBaseAction* pAction, const FreyaData BaseData);

private:
    void*                               m_pRequester;
    FreyaPublicRegister                 m_FreyaPublicRegister;
    FreyaCommandDistribution            m_CMDDistribution;

public:
    static FreyaBaseControl *GetFreyaControl()
    {
        static FreyaBaseControl *pFreyaBaseControl = NULL;
        if(NULL == pFreyaBaseControl)
        {
            pFreyaBaseControl = new FreyaBaseControl();
            qDebug()<<"FreyaLib > "<<"Structure FreyaBaseControl"<<pFreyaBaseControl;
        }
        return pFreyaBaseControl;
    }
};

#endif // FREYABASECONTROL_H
