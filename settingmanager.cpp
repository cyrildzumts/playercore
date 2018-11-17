#include "settingmanager.h"

// InexistentConfigException
InexistentConfigException::InexistentConfigException()
{
    this->message = __FUNCTION__ + std::string(" : Configuration File doesn't exist.");
}
InexistentConfigException::InexistentConfigException(const std::string &filename)
{
    this->message = __FUNCTION__ +
            std::string(" : Configuration File ") +
            filename +
            " doesn't exist";
}


const char* InexistentConfigException::what() const noexcept
    {
        return message.c_str();
    }

SettingManager::SettingManager() : SettingManager(QString())
{

}

SettingManager::SettingManager(const QString &confPath)
{
    init2();
}

SettingManager::~SettingManager()
{
    qDebug() << __PRETTY_FUNCTION__ << " quitting ...";
//    watcher->removePath(conf);
//        disconnect(watcher, &QFileSystemWatcher::fileChanged,
//                   this, &SettingsController::updateConfig);
//        delete watcher;
        watcher = nullptr;
        if(file.isOpen())
        {
            file.close();
        }
}


QString SettingManager::getDatabasePath() const
{
    QJsonObject appJson = getRootObject()["application"].toObject();
        QString database = appJson["database"].toString();
        return database;
}

QString SettingManager::getMediaSourcePath() const
{
    QJsonObject appJson = getRootObject()["application"].toObject();
        QString sourceDir = appJson["source_dir"].toString();
        return sourceDir;
}

void SettingManager::setDatabasePath(const QString &path)
{
    if(QFile::exists(path))
        {
            QJsonDocument document;
            readJsonDocument(document);
            if( file.open(QIODevice::WriteOnly))
            {
                QJsonObject root = document.object();
                QJsonObject appJson = root["application"].toObject();
                appJson["database"] = path;
                root["application"] = appJson;
                document = QJsonDocument(root);
                file.write(document.toJson());
                file.close();
            }
            else
            {
                qDebug() << __FUNCTION__ << "Could not open " << file.fileName()
                         << " in WRITEONLY mode" ;
                return ;
            }

        }
}

void SettingManager::setMediaSourcePath(const QString &path)
{
    if(QFile::exists(path))
        {
            QJsonDocument document;
            readJsonDocument(document);
            if( file.open(QIODevice::WriteOnly))
            {
                QJsonObject root = document.object();
                QJsonObject appJson = root["application"].toObject();
                appJson["source_dir"] = path;
                root["application"] = appJson;
                document = QJsonDocument(root);
                file.write(document.toJson());
                file.close();
            }
            else
            {
                qDebug() << __FUNCTION__ << "Could not open " << file.fileName()
                         << " in WRITEONLY mode" ;
                return ;
            }

        }
}

void SettingManager::setConfigPath(const QString &conf)
{
    if(QFile::exists(conf))
       {
           this->conf = conf;
           if(file.isOpen())
               file.close();
           file.setFileName(this->conf);
           //Q_EMIT configChanged();
       }
}

QString SettingManager::getConfigPath() const
{
    return conf;
}

void SettingManager::updateConfig()
{
}

bool SettingManager::isReady() const
{
    return true;
}

QString SettingManager::applicationName() const
{
    QJsonObject appJson = getRootObject()["application"].toObject();
        QString name = "";
        if(!appJson.isEmpty())
        {
            name = appJson["name"].toString();
        }
        return name;
}

QString SettingManager::applicationVersion() const
{
    QJsonObject appJson = getRootObject()["application"].toObject();
        QString version = "";
        if(!appJson.isEmpty())
        {
            version = appJson["version"].toString();
        }
        return version;
}

QString SettingManager::authorName() const
{
    QJsonObject appJson = getRootObject()["application"].toObject();
        QString name = "";
        if(!appJson.isEmpty())
        {
            name = appJson["author"].toObject()["name"].toString();
        }
        return name;
}

QString SettingManager::authorEmail() const
{
    QJsonObject appJson = getRootObject()["application"].toObject();
        QString email = "";
        if(!appJson.isEmpty())
        {
            email = appJson["author"].toObject()["email"].toString();
        }
        return email;

}

int SettingManager::playerPosition() const
{
    QJsonObject appJson = getRootObject()["application"].toObject();
        int position = -1;
        if(!appJson.isEmpty())
        {
            position = appJson["player"].toObject()["positon"].toInt();
        }
        return position;
}

int SettingManager::playerTrackIndex() const
{
    QJsonObject appJson = getRootObject()["application"].toObject();
        int index = -1;
        if(!appJson.isEmpty())
        {
            index = appJson["player"].toObject()["index"].toString().toInt();
        }
        return index;
}

int SettingManager::playerPlaybackMode() const
{
    QJsonObject appJson = getRootObject()["application"].toObject();
        int mode = 2;
        if(!appJson.isEmpty())
        {
            mode = appJson["player"].toObject()["playbackMode"].toString().toInt();
        }
        return mode;
}

std::vector<int> SettingManager::playlistContent() const
{
    QJsonObject appJson = getRootObject()["application"].toObject();
        std::vector<int> ids ;
        if(!appJson.isEmpty())
        {
            auto jarray= appJson["player"].toObject()["playlist"].toArray();
            if(!jarray.isEmpty())
            {
                QJsonArray::const_iterator it = jarray.constBegin();
                while(it != jarray.constEnd())
                {
                    ids.push_back((*it).toString().toInt());
                    it++;
                }
            }
        }
        return ids;
}

void SettingManager::setPlaylistContent(std::vector<int> &tracklist)
{
}

QJsonObject SettingManager::getRootObject() const
{
    QFile file(conf);
        QJsonObject obj;
        if(file.open(QIODevice::ReadOnly))
        {
            QJsonDocument document = QJsonDocument::fromJson(file.readAll());
            obj = document.object();

        }
        file.close();
        return obj;
}

void SettingManager::readJsonDocument(QJsonDocument &doc)
{
    if(file.open(QIODevice::ReadOnly))
        {
            doc = QJsonDocument::fromJson(file.readAll());
            file.close();

        }
        else
        {
            qDebug() << __FUNCTION__ << "Could not open " << file.fileName()
                     << " in READYONLY mode" ;
            return ;
    }
}

QJsonObject SettingManager::createApplicationEntry()
{
    QJsonObject app;
    app.insert("author", createAuthorOject("cyrildz@gmail.com","Cyrille Ngassam Nkwenga" ));
    app.insert("database", QDir::homePath()+"/.Player/config/player.db");
    app.insert("name", "Player");
    app.insert("source_dir", QDir::homePath()+"/"+"Music");
    app.insert("version", "0.5.2");
    app.insert("player", createPlayerObject());
    return app;
}

QJsonObject SettingManager::createAuthorOject(const QString &email, const QString &name)
{
    QJsonObject author{
        {"name", name},
        {"email", email}
    };
    return author;
}

QJsonObject SettingManager::createPlayerObject()
{
    QJsonObject obj{
        {"index", "0"},
        {"playbackMode", "2"},
        {"playlist", QJsonArray()},
        {"position", "0"}
    };
    return obj;

}

QJsonArray SettingManager::createPlaylistJSONArray()
{
    return  QJsonArray();
}

QJsonArray SettingManager::createPlaylistJSONArray(std::vector<int> &tracks)
{
    QJsonArray arr;
    std::for_each(tracks.begin(), tracks.end(), [&arr](int id){
        arr.append(id);
    });
    return arr;
}

QJsonObject &SettingManager::addJSONObjectToRootObject(QJsonObject &root, const QString &key, const QJsonValue &value)
{
    if(!key.isEmpty())
        root.insert(key, value);
    return root;
}

bool SettingManager::createConfigFile()
{
    bool created = false;
    QString appName = "Player";
    QString appDirName = "." + appName;
    QString confName = "player.conf";
    QString configDirName = "config";
    QString confDirPath = appDirName + "/" + configDirName;
    QString configFilePath;
    qDebug() << __FUNCTION__ << "Building Settings Object ";
    {
        QDir dir = QDir::home();
        bool appDirCreated = dir.mkpath(confDirPath);
        if(appDirCreated || dir.exists(appDirName)){
            qDebug()<< "The player directory is created";
            if(dir.cd(confDirPath)){
                qDebug()<< "Inside config directory : " << dir.absolutePath();
                configFilePath = dir.path() + "/" + confName;
                QFile configFile(configFilePath);
                configFile.open(QIODevice::WriteOnly);
                created = true;
            }
        }
        conf = configFilePath;
    }
    return created;
}

bool SettingManager::configFileExist()
{
    bool ret = false;
    QString conf = QDir::homePath()+"/.Player/config/player.conf";
    ret = QFile::exists(conf);
    if(ret){
        this->conf = conf;
    }
    return QFile::exists(conf);
}

bool SettingManager::configFileIsEmpty()
{
    bool isEmpty = true;
    QString conf = QDir::homePath()+"/.Player/config/player.conf";
    QFile configFile(conf);
    configFile.open(QIODevice::ReadOnly);
    if(configFile.isOpen())
       isEmpty = (configFile.size()== 0);
    if(!isEmpty){
        this->conf = conf;
    }
    return isEmpty;
}

void SettingManager::populateConfigFile()
{
    QJsonValue app_value = createApplicationEntry();
    QJsonObject root;
    root.insert("application", app_value);
    QJsonDocument jsonfile{root};
    QByteArray raw = jsonfile.toJson();
    QFile configFile{conf};
    configFile.open(QIODevice::WriteOnly);
    QDataStream out(&configFile);
    out.writeRawData(raw.data(), raw.size());
}

void SettingManager::init()
{

    if(QFile::exists(conf))
       {
           file.setFileName(conf);
           //watcher = new QFileSystemWatcher();
           //watcher->addPath(conf);
           //connect(watcher,&QFileSystemWatcher::fileChanged,
           //        this, &SettingsController::updateConfig);
       }
       else
       {
           qWarning("Couldn't open conf file.");
           throw InexistentConfigException(conf.toStdString().c_str());
       }

}

void SettingManager::init2()
{
    if(configFileExist()){
        if(configFileIsEmpty()){
            populateConfigFile();
        }
    }
    else{
        createConfigFile();
        populateConfigFile();
    }
}


void SettingManager::setPlayerPosition(int position)
{
}

void SettingManager::setPlayerTrackIndex(int index)
{
}

void SettingManager::setPlayerPlaybackMode(int mode)
{
}
