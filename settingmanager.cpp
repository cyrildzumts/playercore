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

SettingManager::SettingManager()
{
    qDebug() << __FUNCTION__ << "Building Settings Object ";
    conf = QDir::homePath() +  QString("/.Player/config/player.conf");
    init();
}

SettingManager::SettingManager(const QString &confPath)
{
    qDebug() << __FUNCTION__ << "Building Settings Object ";
    conf = confPath;
    init();
}

SettingManager::~SettingManager()
{
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


void SettingManager::setPlayerPosition(int position)
{
}

void SettingManager::setPlayerTrackIndex(int index)
{
}

void SettingManager::setPlayerPlaybackMode(int mode)
{
}
