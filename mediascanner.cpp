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

    queryScanner = data_access->query(QString());
}

MediaScanner::~MediaScanner()
{
    //    disconnect(directoryWatcher,
    //                &QFileSystemWatcher::directoryChanged,
    //                this,
    //                &MediaScanner::launchUpdateThread);
    queryScanner.clear();
    delete directoryWatcher;
    //delete data_access;
}

// TODO : MOVE EVERY database query to DataaccessObject
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

int MediaScanner::getLastModifiedDate()
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
        auto query = queryScanner;
        query.exec("SELECT isConfigured FROM Settings");
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
        auto query = queryScanner;
        query.exec(str);
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
    auto query = data_access->query(QString());
    auto deletQuery = data_access->query(QString());
    if(query.exec(QString("SELECT trackUrl,albumTitle,cover FROM BaseTableTracks ORDER BY albumTitle;")))
    {
        if(query.isActive())
        {
            // search for removed file.

            QString path ;
            while(query.next())
            {
                path = query.value(QString("trackUrl")).toString();
                if(!QFile::exists(path))
                {

                    deletQuery.exec(QString("DELETE FROM BaseTableTracks WHERE trackUrl ='%1';").arg(path));
                }
                else
                {
                    fileInDatabase.push_back(path);
                }
            }
            std::sort(fileInDatabase.begin(), fileInDatabase.end());
            //qDebug() << __PRETTY_FUNCTION__ << " : " << fileInDatabase.size() << " Files in database";
        }
    }

    else
    {
        qDebug() << __PRETTY_FUNCTION__ << "query error : " << query.lastError();
    }
    query.clear();
    deletQuery.clear();
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
    if(query.isActive())
    {
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
        }
        std::sort(files.begin(), files.end());
        //qDebug() << __PRETTY_FUNCTION__ << " : " << files.size() << " Files in database";
    }
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
