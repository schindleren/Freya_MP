#include "freyalaunch.h"
#include "freyabaseextension.h"

#include "freya_global.h"

typedef const char * (*ModuleInforFun)();
typedef bool (*ModulePreLaunchFun)();

FreyaLaunch::FreyaLaunch(QString launchKey) :
    FreyaBaseAction(FreyaBaseControl::GetFreyaControl(), FRYLAC_OBJ_LAUNCHER),
    m_LaunchKey(launchKey)
{
    RegisterCommands();

    qDebug() << "FreyaLaunch > " << "Freya Version:" << FreyaBaseControl::GetFreyaControl()->FreyaVersion();
#ifdef  __linux
    system(QString("rm /tmp/%1").arg(FRYLAC_COD_PLATFORMID).toUtf8().constData());
#endif
    bool listening = (new FreyaBaseExtension(FRYLAC_COD_PLATFORMID, FreyaBaseControl::GetFreyaControl() , FRYLAC_OBJ_PLUGINEXT)) \
                            ->DefineAuthCode(QStringList()<<FRYLAC_COD_MSG1<<FRYLAC_COD_MSG2<<FRYLAC_COD_MSG4<<FRYLAC_COD_MSG8,
                                            QStringList()<<FRYLAC_COD_CMD1<<FRYLAC_COD_CMD2<<FRYLAC_COD_CMD4<<FRYLAC_COD_CMD8);

    if(!listening)
    {
#ifdef  __linux
        qWarning() << "FreyaLaunch > " << "Cannot listened from plugin platform. Please check /tmp/ has no file named:" << FRYLAC_COD_PLATFORMID << ".";
#else
        qWarning() << "FreyaLaunch > " << "Cannot listened from plugin platform: " << FRYLAC_COD_PLATFORMID << ".";
#endif
    }

    LoadModules();
}

FreyaLaunch::~FreyaLaunch()
{
    foreach(QLibrary* lib, m_ModuleList)
    {
        lib->unload();
        lib->deleteLater();
    }
#ifdef  __linux
    system(QString("rm /tmp/%1").arg(FRYLAC_COD_PLATFORMID).toUtf8().constData());
#endif
}

void FreyaLaunch::launch()
{
    FREYA_REQUESTEXECUTION(FRYLAC_CMD_MODULELAUNCH);
}

void FreyaLaunch::Execute(const FreyaData data)
{
    auto RemoteQuit = [&]{
        qDebug() << "FreyaLaunch > " << "get command FRYLAC_CMD_REMOTEQUIT. Application will quit soon.";
        FREYA_REQUESTEXECUTION(FRYLAC_CMD_MODULEQUIT);
        QTimer::singleShot(1000, qApp, SLOT(quit()));
    };

    auto RemoteCMD = [&]{
        // FreyaRemote addlib#xxx.so
        QString arg = data->GetArgument().toString();
        QStringList argList = arg.split("#");
        if(arg.startsWith("addlib") && argList.size() > 1)
        {
            LoadLibrary(argList.at(1));
        }
    };

    FREYACONNECT(FRYLAC_CMD_REMOTEQUIT, RemoteQuit());
    FREYACONNECT(FRYLAC_CMD_REMOTECMD, RemoteCMD());
}

void FreyaLaunch::LoadModules()
{
    QDir moduleDir(MODULEDIR);
    if(moduleDir.exists())
    {
        QStringList nameFilte;

#ifdef Q_OS_WIN
        nameFilte.append("*.dll");
#else
#ifdef Q_OS_LINUX
    #ifdef QT_DEBUG
        nameFilte.append("*_d.so");
    #else
        nameFilte.append("*.so");
    #endif
#endif
#endif

        QFileInfoList moduleFileList = moduleDir.entryInfoList(nameFilte, QDir::NoDotAndDotDot | QDir::Files, QDir::Name);
        foreach (const QFileInfo &moduleFile, moduleFileList)
        {
            LoadLibrary(moduleFile.absoluteFilePath());
        }
    }
    else
    {
        qWarning() << "FreyaLaunch > " << "no" << MODULEDIR;
    }
}

bool FreyaLaunch::LoadLibrary(const QString &filePath)
{
    qDebug() << "FreyaLaunch > " << "loading:" << filePath ;
    QLibrary *lib = new QLibrary(filePath);
    auto loadLib = [&]{
        if(lib->load())
        {
            ModuleInforFun fmInf = (ModuleInforFun)lib->resolve("FreyaModuleInfor");

            if (fmInf)
            {
                QString infor = QString::fromLocal8Bit(fmInf());
                QVariantMap varMap = QJsonDocument::fromJson(infor.toUtf8()).toVariant().toMap();
                if(!m_LaunchKey.isEmpty())
                {
                    if(0 != m_LaunchKey.contains(varMap.value(FRYLAC_STR_LAUNCHKEY).toString(), Qt::CaseInsensitive))
                    {
                        lib->unload();
                        qDebug() << "FreyaLaunch > " << "not a useful module key" << varMap.value(FRYLAC_STR_LAUNCHKEY).toString();
                        return false;
                    }
                }
                qDebug() << "FreyaLaunch > " << "Module Name" << varMap.value(FRYLAC_STR_MODULENAME).toString();
                qDebug() << "FreyaLaunch > " << "Module Version" << varMap.value(FRYLAC_STR_MODULEVER).toString();

                ModulePreLaunchFun fmPreLaunch = (ModulePreLaunchFun)lib->resolve("FreyaModulePreLaunch");
                if(fmPreLaunch)
                {
                    return fmPreLaunch();
                }
                return false;
            }
            else
            {
                lib->unload();
                qDebug() << "FreyaLaunch > " << "not a useful module";
            }
        }
        else
        {
            qDebug() << "FreyaLaunch > " << "can not loadin this module";
        }
        return false;
    };
    if(loadLib())
    {
        m_ModuleList.append(lib);
        return true;
    }
    else
    {
        lib->deleteLater();
        return false;
    }
}
