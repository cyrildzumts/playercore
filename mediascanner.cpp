#include "mediascanner.h"

MediaScanner::MediaScanner()
{

}

MediaScanner::MediaScanner(const QString &database, const QString &sourDir)
{

}

MediaScanner::MediaScanner(AbstractDataAccessObject *data_access)
{
    this->data_access = data_access;
    directoryWatcher = new QFileSystemWatcher;
    audioFormatFilter << QString ("*.m4a")
                      << QString("*.mp3")
                      << QString("*.flac")
                      << QString("*.ogg")
                      << QString("*.wma");

}

MediaScanner::~MediaScanner()
{
    qDebug() << __PRETTY_FUNCTION__ << " quitting ...";
    //    disconnect(directoryWatcher,
    //                &QFileSystemWatcher::directoryChanged,
    //                this,
    //                &MediaScanner::launchUpdateThread);
    queryScanner.clear();
    delete directoryWatcher;
    //delete data_access;
}

void MediaScanner::init(const QString &sourcedir)
{
    if(!sourcedir.isEmpty())
    {
        sourceDirPath = sourcedir;
        setConfig();
        databaseConfig();
    }

}

bool MediaScanner::exec(const QString &cmd)
{
    auto query = data_access->query(cmd);
    bool flag = query.lastError().isValid();
    if(flag)
    {
        qDebug() << __PRETTY_FUNCTION__
                 << " Error : database query command Error :"
                 << "Command issued :: " << cmd << "\n"
                 << " Error tracked : "
                 << query.lastError().text();
    }
    return !flag;
}

// TODO properly check if the scann was succesfull before
// updating the database table Settings to isScanned = 1
void MediaScanner::scanSourceDirectory()
{
    /******************************************
         * If the source directory was not yet scanned,
         * we scann it, if not we simply look for changes
         * through launchUpdateThread()
         */
    std::cout <<"Running " <<  __PRETTY_FUNCTION__  << std::endl;
    if(hasDataAccessObject())
    {

        //query.exec("BEGIN TRANSACTION;");


            auto _query = data_access->query("PRAGMA foreign_keys;");
            if(_query.next())
            {
                 qDebug() << "Database Foreign Key : "
                          << _query.value(0).toInt();
            }

        if(!isSourceDirectoryScanned())
        {
            qDebug() << __PRETTY_FUNCTION__ << " : source Directory was not scanned yet" ;

            QDir::Filters filter = QDir::Dirs|QDir::Files | QDir::NoDotAndDotDot;
            Track  track;
            QString tmppath;
            QFileInfo file;
            QDirIterator dirIter (sourceDirPath,audioFormatFilter, filter,QDirIterator::Subdirectories);
            //QTime t;
            std::lock_guard<std::mutex>dbLock(databaseMutex);
            //int i = 0;
            //int loggerTime = -1;
            //t.start();

            exec("BEGIN TRANSACTION;");
            while(dirIter.hasNext())
            {
                tmppath = dirIter.next();
                track = Tagreader::tagreader(tmppath);
                track.playCount = 0;
                track.liked = 0;
                file = QFileInfo(track.path);
                track.modifiedDate = file.lastModified().toTime_t();
                track.addedDate = QDateTime::currentDateTime().toTime_t();
                data_access->addTrack(track);
                //i++;
            }
            //loggerTime = t.elapsed();

            exec("UPDATE Settings SET isScanned = 1 WHERE ID = 1;");

            data_access->commit();
            //exec("END TRANSACTION;");

        }
        else
        {
            qDebug() << __PRETTY_FUNCTION__ << " : source Directory already scanned" ;
            qDebug() << __PRETTY_FUNCTION__ << " : running update Thread now." ;
            launchUpdateThread();
        }

    }
    else
    {
        qDebug() << __PRETTY_FUNCTION__ << " : no DataAccess Object";
    }
}


void MediaScanner::scanSourceDirectory2()
{
    /******************************************
         * If the source directory was not yet scanned,
         * we scann it, if not we simply look for changes
         * through launchUpdateThread()
         */
    std::cout <<"Running " <<  __PRETTY_FUNCTION__  << std::endl;
    if(hasDataAccessObject())
    {
        QSqlQuery query = data_access->query(QString());
        query.exec("BEGIN TRANSACTION;");
        if(!isSourceDirectoryScanned())
        {
            qDebug() << __PRETTY_FUNCTION__ << " : source Directory was not scanned yet" ;

            QDir::Filters filter = QDir::Dirs|QDir::Files | QDir::NoDotAndDotDot;
            Track  track;
            QString tmppath;
            QFileInfo file;
            QDirIterator dirIter (sourceDirPath,audioFormatFilter, filter,QDirIterator::Subdirectories);
            //QTime t;
            std::lock_guard<std::mutex>dbLock(databaseMutex);
            //int i = 0;
            //int loggerTime = -1;
            //t.start();


            while(dirIter.hasNext())
            {
                tmppath = dirIter.next();
                track = Tagreader::tagreader(tmppath);
                track.playCount = 0;
                track.liked = 0;
                file = QFileInfo(track.path);
                track.modifiedDate = file.lastModified().toTime_t();
                track.addedDate = QDateTime::currentDateTime().toTime_t();
                data_access->addTrack(track);
                //i++;
            }
            //loggerTime = t.elapsed();
            query.exec("UPDATE Settings SET isScanned = 1 WHERE ID = 1;");

        }
        else
        {
            qDebug() << __PRETTY_FUNCTION__ << " : source Directory already scanned" ;
            qDebug() << __PRETTY_FUNCTION__ << " : running update Thread now." ;
            launchUpdateThread();
        }
        query.exec("END TRANSACTION;");
        query.finish();
        data_access->commit();
    }
    else
    {
        qDebug() << __PRETTY_FUNCTION__ << " : no DataAccess Object";
    }
}
void MediaScanner::setSourceDirectory(const QString &dirpath)
{
    sourceDirPath = dirpath;
    directoryWatcher->addPath(sourceDirPath);
    /*
    connect(directoryWatcher,
            &QFileSystemWatcher::directoryChanged,
            this,
            &MediaScanner::launchUpdateThread);
            */
}

int MediaScanner::getLastAddedDate()
{
    // bool openSuccess = false;
    int date = -1;
    if(data_access not_eq nullptr)
    {

        auto query = data_access->query(QString("SELECT MAX(addedDate) AS date FROM BaseTableTracks;"));
        std::lock_guard<std::mutex>dbLock(databaseMutex);
        if(query.isActive())
        {
            while(query.next())
            {
                date = query.value("date").toInt();
            }
            query.finish();
        }

    }
    else
    {
        qDebug() << __PRETTY_FUNCTION__ << ": DataAccess Object not initialized";
    }
    return date;
}

int MediaScanner::getLastModifiedDate()
{
    // bool openSuccess = false;
    int date = -1;
    if(data_access not_eq nullptr)
    {
        auto query = data_access->query(QString("SELECT MAX(modifiedDate) AS date FROM BaseTableTracks;"));
        std::lock_guard<std::mutex>dbLock(databaseMutex);
        if(query.isActive())
        {
            while(query.next())
            {
                date = query.value("date").toInt();
            }
            query.finish();
        }

    }
    else
    {
        qDebug() << __PRETTY_FUNCTION__ << ": DataAccess Object not initialized";
    }
    return date;
}



int MediaScanner::getLastAddedDate2()
{
    // bool openSuccess = false;
    int date = -1;
    if(data_access not_eq nullptr)
    {

        auto query = data_access->query(QString());
        std::lock_guard<std::mutex>dbLock(databaseMutex);
        query.exec(QString("SELECT MAX(addedDate) AS date FROM BaseTableTracks;"));
        if(query.isActive())
        {
            while(query.next())
            {
                date = query.value("date").toInt();
            }
            query.finish();
        }

    }
    else
    {
        qDebug() << __PRETTY_FUNCTION__ << ": DataAccess Object not initialized";
    }
    return date;
}

int MediaScanner::getLastModifiedDate2()
{
    // bool openSuccess = false;
    int date = -1;
    if(data_access not_eq nullptr)
    {
        auto query = data_access->query(QString());
        std::lock_guard<std::mutex>dbLock(databaseMutex);
        query.exec(QString("SELECT MAX(modifiedDate) AS date FROM BaseTableTracks;"));
        if(query.isActive())
        {
            while(query.next())
            {
                date = query.value("date").toInt();
            }
            query.finish();
        }

    }
    else
    {
        qDebug() << __PRETTY_FUNCTION__ << ": DataAccess Object not initialized";
    }
    return date;
}
void MediaScanner::updateData()
{
    //bool openSuccess = false;
    std::cout << "running " << __PRETTY_FUNCTION__ << std::endl;
    std::vector<QString> filesInSourceDir,fileToadd, urls_from_database;
    std::lock_guard<std::mutex>dbLock(databaseMutex);
    if(data_access not_eq nullptr)
    {
        removeDeletedFile();
        //urls_from_database = getUrlFromDatabase();
        filesInSourceDir = getFileInSourceDir();
        setNewFiles(filesInSourceDir, fileInDatabase, fileToadd);
        addFileToDataAccessObject(fileToadd);
    }
    else
    {
        std::cout << __PRETTY_FUNCTION__ << ": DataAccess Object not initialized"  << std::endl;
    }
}

void MediaScanner::setDataAccessObject(AbstractDataAccessObject *data_access)
{
    this->data_access = data_access;
}

bool MediaScanner::isSourceDirectoryConfigured() const
{
    bool flag = false;
    if(hasDataAccessObject())
    {
        QString str = "SELECT isConfigured FROM Settings";
        auto query = data_access->query(str);
        if(query.isActive())
        {
            if(query.first())
            {
                int isConfigured  = query.value("isConfigured").toInt();
                flag = isConfigured == 1;
            }
        }
        query.clear();
    }
    return flag;
}

bool MediaScanner::isSourceDirectoryScanned() const
{
    bool flag = false;
    if(hasDataAccessObject())
    {
        QString str = "SELECT isScanned FROM Settings";
        auto query = data_access->query(str);
        //query.exec(str);
        if(query.isActive())
        {
            if(query.first())
            {
                int isScanned  = query.value("isScanned").toInt();
                flag = isScanned == 1;
            }
        }
        query.clear();

    }
    return flag;
}

bool MediaScanner::hasDataAccessObject() const
{
    return data_access != nullptr;
}

void MediaScanner::createTables()
{
    if (data_access->isOpen())
        {
            //data_access->query("BEGIN TRANSACTION;");

            createCoverTable();
            createGenreTable();

            createArtistTable();

            createAlbumTable();

            createBaseTrackTable();
            createPlaylistTable();
            createPlaylistTracksTable();
            // Directory is the Collection source: for example Music, /media/$USER/Music ...
            //createDirectoriesTable();
            //Subdirectory is the content of the collection
            //createSubDirectoriesTable();
            exec("UPDATE Settings SET isConfigured = 1 WHERE ID = 1;");
            //data_access->commit();
        }
        else qDebug() << "DataAcess::config(): database is not opened" ;
}

void MediaScanner::createCoverTable()
{
    if(exec(QString("CREATE TABLE IF NOT EXISTS Cover"
                             "("
                             "coverID INTEGER NOT NULL,"
                             "coverpath TEXT UNIQUE COLLATE NOCASE,"
                             "PRIMARY KEY (coverID));")
                    ))
    {
        qDebug() << __PRETTY_FUNCTION__
                 << " run successfully ";
    }

}

void MediaScanner::createGenreTable()
{
    if(exec(QString("CREATE TABLE IF NOT EXISTS Genre"
                           "("
                           "genreID INTEGER ,"
                           "genre TEXT UNIQUE COLLATE NOCASE,"
                           "PRIMARY KEY(genreID)"
                           ");"
                           )))
    {
        qDebug() << __PRETTY_FUNCTION__
                 << " run successfully ";
    }

}

void MediaScanner::createArtistTable()
{
   if(exec(QString("CREATE TABLE IF NOT EXISTS Artist (artistID INTEGER,"
                           "artistname TEXT UNIQUE COLLATE NOCASE,"
                           "description TEXT,"
                           "PRIMARY KEY(artistID));"
                           )))
   {
       qDebug() << __PRETTY_FUNCTION__
                << " run successfully ";
   }

}

void MediaScanner::createAlbumTable()
{
    if(exec(QString("CREATE TABLE IF NOT EXISTS Album"
                           "("
                           "albumID INTEGER ,"
                           "genreID INTEGER NOT NULL,"
                           "artistID INTEGER NOT NULL,"
                           "coverID INTEGER ,"
                           "albumTitle TEXT UNIQUE NOT NULL COLLATE NOCASE,"
                           //"subDirID INTEGER REFERENCES SubDirectories(subDirectoryID),"
                           "year INTEGER DEFAULT 0,"
                           "tracks INTEGER DEFAULT 0,"
                           "duration INTEGER DEFAULT 0,"
                           "UNIQUE(albumTitle,artistID),"
                           "FOREIGN KEY (artistID) REFERENCES Artist(artistID) ON DELETE CASCADE,"
                           "FOREIGN KEY (genreID) REFERENCES Genre(genreID),"
                           "FOREIGN KEY (coverID) REFERENCES Cover(coverID) ON DELETE CASCADE,"
                           "PRIMARY KEY (albumID) );"
                           )))
    {
        qDebug() << __PRETTY_FUNCTION__
                 << " run successfully ";
    }

}

void MediaScanner::createBaseTrackTable()
{
    if(exec(QString("CREATE TABLE IF NOT EXISTS BaseTableTracks"
                           "(trackID INTEGER PRIMARY KEY,"
                           "albumTitle TEXT NOT NULL COLLATE NOCASE,"
                           "artist TEXT NOT NULL COLLATE NOCASE,"
                           "albumArtist TEXT NOT NULL COLLATE NOCASE,"
                           "title TEXT NOT NULL COLLATE NOCASE,"
                           "genre TEXT NOT NULL COLLATE NOCASE,"
                           "trackUrl  TEXT  UNIQUE NOT NULL  COLLATE NOCASE,"
                           "cover  TEXT  NOT NULL  COLLATE NOCASE,"
                           "trackNumber INTEGER ,"
                           "length INTEGER NOT NULL,"
                           "playCount INTEGER DEFAULT 0,"
                           "favorite INTEGER DEFAULT 0,"
                           "bitrate INTEGER,"
                           "year INTEGER,"
                           "addedDate INTERGER NOT NULL,"
                           "modifiedDate INTEGER ,"
                           "UNIQUE(albumTitle,trackUrl));"
                           )))
    {
        qDebug() << __PRETTY_FUNCTION__
                 << " run successfully ";
    }
}

void MediaScanner::createPlaylistTable()
{
    if(exec(QString("CREATE TABLE IF NOT EXISTS Playlist"
                           "("
                           "playlistID INTEGER,"

                           "title TEXT UNIQUE COLLATE NOCASE,"
                           "favorite INTEGER DEFAULT 0,"
                           "coverID INTEGER,"
                           "FOREIGN KEY (coverID) REFERENCES Cover(coverID),"
                           "PRIMARY KEY (playlistID) );"
                           )))
    {
        qDebug() << __PRETTY_FUNCTION__
                 << " run successfully ";
    }
}

void MediaScanner::createPlaylistTracksTable()
{
    if(exec(QString("CREATE TABLE IF NOT EXISTS PlaylistTrack"
                           "(plsTrackID INTEGER ,"
                           "playlistID INTEGER ,"
                           "trackID INTEGER,"
                           "FOREIGN KEY(playlistID) REFERENCES Playlist(playlistID) ON DELETE CASCADE,"
                           "FOREIGN KEY (trackID) REFERENCES BaseTableTracks(trackID)ON DELETE CASCADE,"
                           "PRIMARY KEY (plsTrackID) );"
                           )))
    {
        qDebug() << __PRETTY_FUNCTION__
                 << " run successfully ";
    }
}

void MediaScanner::createDirectoriesTable()
{

   if(exec(QString("CREATE TABLE IF NOT EXISTS Directories"
                           "(directoryID INTEGER,"
                           "path TEXT NOT NULL UNIQUE COLLATE NOCASE,"
                           "lastScanedDate INTEGER,"
                       "PRIMARY KEY (directoryID));")))
   {
       qDebug() << __PRETTY_FUNCTION__
                << " run successfully ";
   }
}

void MediaScanner::createSubDirectoriesTable()
{
    if(exec(QString("CREATE TABLE IF NOT EXISTS SubDirectories"
                          "(subDirectoryID INTEGER,"
                          /*"parentID INTEGER REFERENCES Directories(directoryID)," */
                          "path TEXT NOT NULL UNIQUE COLLATE NOCASE,"
                          "addedDate INTEGER,"
                          "modifiedDate INTEGER,"
                          "PRIMARY KEY (subDirectoryID));")))
    {
        qDebug() << __PRETTY_FUNCTION__
                 << " run successfully ";
    }
}

// TODO use SettingManager to get the
void MediaScanner::setConfig()
{
    qDebug() << __PRETTY_FUNCTION__ << " ..." ;
    QDir home = QDir::home();
        if (!home.exists(home.absolutePath() + "/.Player"))
        {
            home.mkdir(home.absolutePath() + "/.Player");
        }
        std::lock_guard<std::mutex>dbLock(databaseMutex);
        if (data_access->isOpen())
        {

            //auto confQuery = QSqlQuery(query);

            //auto query = data_access->query("PRAGMA foreign_keys = ON ;");
            QString str = QString("INSERT OR IGNORE INTO Settings(sourcedirectory,applicationName) VALUES('%1','%2');")
                    .arg(sourceDirPath,"Player");

            //exec("PRAGMA foreign_keys = ON ;");

            //exec("PRAGMA SYNCHRONOUS = OFF");
           // exec("PRAGMA journal_mode = MEMORY");
            exec(QString( "CREATE TABLE IF NOT EXISTS Settings"
                                    "("
                                    "ID INTEGER NOT NULL,"
                                    "sourcedirectory TEXT UNIQUE COLLATE NOCASE,"
                                    "applicationName TEXT UNIQUE COLLATE NOCASE,"
                                    "isConfigured INTEGER DEFAULT 0,"
                                    "isScanned INTEGER  DEFAULT 0,"
                                    "PRIMARY KEY (ID));")
                           );
           exec( str);
           qDebug() << __PRETTY_FUNCTION__ << ": Str = " << str ;
           //data_access->commit();

        }

        else
        {
            qDebug() << "DataAccess::setConfig(): Database couldn't be opened" ;
            //TODO need to read the last error from the database through data_access
        }
}

void MediaScanner::databaseConfig2()
{
    auto query = data_access->query("SELECT * FROM Settings ;");
            if(query.isActive())
            {
                if(query.first())
                {
                    //isScanned = query.value("isScanned").toInt();
                   // isConfigured = query.value("isConfigured").toInt();
                }
                if(!isDatabaseConfigured())
                {
                    try
                    {
                        createTables();

                    }
                    catch(std::exception &e)
                    {

                    }

                }
            }
        else
        {
            qDebug() << "DataAccess::databaseConfig(): Query not executed" ;
            qDebug() << "DataAccess::databaseConfig() Error: " << query.lastError() ;
        }

}

void MediaScanner::databaseConfig()
{
    qDebug() << __PRETTY_FUNCTION__ << " started";
    if(!isSourceDirectoryConfigured())
    {
        createTables();
        //data_access->commit();
    }
    else
    {
        qDebug() << __PRETTY_FUNCTION__
                 << " : database is configured.";
    }
}

void MediaScanner::setup()
{

}

bool MediaScanner::isDatabaseConfigured() const
{

}

void MediaScanner::launchScanThread()
{
    directoryScannThread = std::thread(&MediaScanner::scanSourceDirectory,this);
    if(directoryScannThread.joinable())
        directoryScannThread.detach();
}

void MediaScanner::launchUpdateThread()
{
    updateThread = std::thread (&MediaScanner::updateData,this);
    if (updateThread.joinable())
        updateThread.join();
}

void MediaScanner::removeDeletedFile()
{
    auto query = data_access->query(QString("SELECT trackUrl,albumTitle,cover FROM BaseTableTracks ORDER BY albumTitle;"));
    //auto deletQuery = data_access->query(QString());
if(query.isActive())
        {

            QString path ;
            while(query.next())
            {
                path = query.value(QString("trackUrl")).toString();
                if(!QFile::exists(path))
                {

                    exec(QString("DELETE FROM BaseTableTracks WHERE trackUrl ='%1';").arg(path));
                    qDebug() << "removing file " + path;
                }
                else
                {
                    fileInDatabase.push_back(path);
                }
            }
            if(!fileInDatabase.empty())
                std::sort(fileInDatabase.begin(), fileInDatabase.end());
           qDebug() << __PRETTY_FUNCTION__ << " : " << fileInDatabase.size() << " Files in database";
        }

    else
    {
        qDebug() << __PRETTY_FUNCTION__ << "query error : " << query.lastError();
    }
    query.clear();
}

std::vector<QString> MediaScanner::getFileInSourceDir()
{
    QString path;
    std::vector<QString> files;
    QDir::Filters filter = QDir::Dirs|QDir::Files | QDir::NoDotAndDotDot;
    QDirIterator dirIter (sourceDirPath,audioFormatFilter, filter,QDirIterator::Subdirectories);
    while(dirIter.hasNext())
    {
        path = dirIter.next();
        files.push_back(path);
    }
    std::sort(files.begin(), files.end());
    //qDebug() << __PRETTY_FUNCTION__ << " : " << files.size() << " Files in " << sourceDirPath;
    return files;
}

std::vector<QString> MediaScanner::getUrlFromDatabase()
{
    std::vector<QString> files;
    auto query = data_access->query(QString("SELECT trackUrl,albumTitle,cover FROM BaseTableTracks ORDER BY albumTitle;"));
        // search for removed file.
        if(query.isActive())
        {
            QString path ;
            while(query.next())
            {
                path = query.value(QString("trackUrl")).toString();
                if(QFile::exists(path))
                {
                    files.push_back(path);
                }
            }
            std::sort(files.begin(), files.end());
        }

        //qDebug() << __PRETTY_FUNCTION__ << " : " << files.size() << " Files in database";
    else
    {
        qDebug() << __PRETTY_FUNCTION__ << "query error : " << query.lastError();
    }
    query.finish();
    return files;
}

void MediaScanner::setNewFiles(std::vector<QString> &lhs, std::vector<QString> &rhs, std::vector<QString> &newFiles)
{
    /**
         *  I compute the difference between the urls I obtained from the database
         * with the ones I got from the source directory and store the result in fileToadd
         * and then add all the urls present in fileToadd into the Database.
         *
         * */
    std::set_difference(lhs.begin(), lhs.end(),
                        rhs.begin(), rhs.end(),
                        std::back_inserter(newFiles));

    //qDebug() << __PRETTY_FUNCTION__ << " : " << newFiles.size() << " new Files to add";
}

void MediaScanner::addFileToDataAccessObject(std::vector<QString> &files)
{
    if(!files.empty())
    {
        //qDebug() << __PRETTY_FUNCTION__ << " : adding file into database started" ;
        Track track;
        auto addedDate = QDateTime::currentDateTime().toTime_t();
        for(auto file: files)
        {
            //qDebug() << __PRETTY_FUNCTION__ << " : adding file " << file ;
            track = Tagreader::tagreader(file);
            track.modifiedDate = QFileInfo(file).lastModified().toTime_t();
            track.addedDate = addedDate;
            track.playCount = 0;
            track.liked = 0;
            data_access->addTrack(track);
        }
        std::cout << __PRETTY_FUNCTION__ << " : update finished" << std::endl;

    }
    else
    {
        std::cout << __PRETTY_FUNCTION__ << " nothing to add " << std::endl;
    }
}

void MediaScanner::print_difference(std::vector<QString> &lhs, std::vector<QString> &rhs) const
{
    qDebug() << __PRETTY_FUNCTION__ << " Method called ";

    qDebug() << __PRETTY_FUNCTION__ << "LHS :" ;

    for(auto path : lhs)
        qDebug() << "In LHS : " << path;

    qDebug()  << __PRETTY_FUNCTION__ << "RHS :";
    for(auto path : rhs)
        qDebug()<< "In RHS : " << path;
}
