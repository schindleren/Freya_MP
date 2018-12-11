#include "freyalaunch.h"

#include "freya_global.h"

typedef const char * (*ModuleInforFun)();
typedef bool (*ModulePreLaunchFun)(FreyaBaseControl *pLaunchControl);

#ifdef Q_OS_WIN
QString ModuleSuffix("dll");
#elif Q_OS_LINUX
QString ModuleSuffix("so");
#endif

FreyaLaunch::FreyaLaunch(QString launchKey) :
    FreyaBaseActionEx(FreyaBaseControl::GetFreyaControl(), FRYLAC_OBJ_LAUNCHER),
    m_LaunchKey(launchKey)
{
    qDebug() << "FreyaLaunch > " << "Freya Version:" << FreyaBaseControl::GetFreyaControl()->FreyaVersion();
    RegisterCommands();

#ifdef  Q_OS_LINUX
    system(QString("rm /tmp/%1").arg(FRYLAC_COD_PLATFORMID).toUtf8().constData());
#endif
    bool listening = (new FreyaBaseExtension(FRYLAC_COD_PLATFORMID, FreyaBaseControl::GetFreyaControl() , FRYLAC_OBJ_PLUGINEXT)) \
            ->DefineAuthCode(QStringList()<<FRYLAC_COD_MSG1<<FRYLAC_COD_MSG2<<FRYLAC_COD_MSG4<<FRYLAC_COD_MSG8,
                             QStringList()<<FRYLAC_COD_CMD1<<FRYLAC_COD_CMD2<<FRYLAC_COD_CMD4<<FRYLAC_COD_CMD8);

    if(!listening)
    {
#ifdef  Q_OS_LINUX
        qWarning() << "FreyaLaunch > " << "Cannot listened from plugin platform. Please check /tmp/ has no file named:" << FRYLAC_COD_PLATFORMID << ".";
#else
        qWarning() << "FreyaLaunch > " << "Cannot listened from plugin platform: " << FRYLAC_COD_PLATFORMID << ".";
#endif
    }

    LoadModules();
}

FreyaLaunch::~FreyaLaunch()
{
    qDebug() << "FreyaLaunch > " << "FreyaLaunch is quit!";
}

void FreyaLaunch::launch()
{
//    FREYA_REQUESTEXECUTION(FRYLAC_CMD_MODULELAUNCH);
}

void FreyaLaunch::OnExecuteEx(const FreyaData data)
{
    auto RemoteQuit = [&]{
        qDebug() << "FreyaLaunch > " << "get command FRYLAC_CMD_REMOTEQUIT. Application will quit soon.";
        FREYA_REQUESTEXECUTION(FRYLAC_CMD_MODULEQUIT);
        QTimer::singleShot(1000, this, SLOT(OnQuit()));
    };

    auto RemoteCMD = [&]{
        // FreyaRemote addlib xxx.so
        // FreyaRemote addes x:/xxx.xx
        QStringList argList = data->GetArgument().toStringList();
        qDebug() << "FreyaLaunch > " << "FreyaRemote command:" << argList;
        if((0 == argList.first().compare("addlib", Qt::CaseInsensitive)) && argList.size() > 1)
        {
            QFile libFile(argList.at(1));
            if(libFile.exists())
            {
                if(LoadLibrary(argList.at(1)))
                {
                    QFileInfo libInfo(libFile);
                    libFile.copy(QString(MODULEDIR) + QDir::separator() + libInfo.baseName() + "." + ModuleSuffix);
                }
            }
        }
    };

    FREYACONNECT(FRYLAC_CMD_REMOTEQUIT, RemoteQuit());
    FREYACONNECT(FRYLAC_CMD_REMOTECMD, RemoteCMD());
}

void FreyaLaunch::OnQuit()
{
    qDebug() << "FreyaLaunch > " << "destory objects.";
    FreyaBaseControl::GetFreyaControl()->DeleteAllAction(QStringList()<<FRYLAC_OBJ_LAUNCHER);

    foreach(QLibrary* lib, m_ModuleList)
    {
        lib->unload();
        lib->deleteLater();
    }
#ifdef  Q_OS_LINUX
    system(QString("rm /tmp/%1").arg(FRYLAC_COD_PLATFORMID).toUtf8().constData());
#endif

    qApp->quit();

//    QTimer::singleShot(2000, qApp, SLOT(quit()));
}

void FreyaLaunch::LoadModules()
{
    QDir moduleDir(MODULEDIR);
    if(moduleDir.exists())
    {
        QStringList nameFilte;
        nameFilte.append(QString("*.%1").arg(ModuleSuffix));

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
                    return fmPreLaunch(FreyaBaseControl::GetFreyaControl());
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
