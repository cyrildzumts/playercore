#include "dataaccessobject.h"

DataAccessObject::DataAccessObject(const QString& path)
{

    if(path.isEmpty())
    {
        throw std::invalid_argument("DataAccessObject : path is empty.\n");
    }
    /*
    else if(!QFile::exists(path))
    {
        throw std::invalid_argument( "DataAccessObject : "
                                     + path.toStdString()
                                    + "doesn't exists");
    }
    */
    appName = QString("Player");
    databasePath = path;
    driver = "QSQLITE";
    connectionName = QString("DataAccess");
    database = QSqlDatabase::addDatabase(driver);
    database.setDatabaseName(databasePath);
    db = QSqlDatabase::cloneDatabase(database, connectionName);


    addedDate = 0;
    modifiedDate = 0;
    if(!database.open())
    {
        std::cout << __FUNCTION__
                  << " Could not be opened. "
                  << database.lastError().text().toStdString()
                  << std::endl
                  << "Exiting ..." << std::endl;
        //QSqlDatabase::removeDatabase(connectionName);
        exit(-1);

    }
    if(!db.open())
    {
        std::cout << __FUNCTION__
                  << " Could not be opened. "
                  << db.lastError().text().toStdString()
                  << std::endl
                  << "Exiting ..." << std::endl;
        //QSqlDatabase::removeDatabase(connectionName);
        exit(-1);

    }

    _query = QSqlQuery(database);
    //add_query = QSqlQuery(database);

    if(!_query.exec("PRAGMA foreign_keys = ON ;"))
    {
        qDebug() << __PRETTY_FUNCTION__ << " - "  << __LINE__ <<
                    "Foreign Key error : " << _query.lastError().text() ;
    }
//    else
//    {
//        if(_query.exec("PRAGMA foreign_keys;"))
//        {
//           if( _query.next())
//           {
//                qDebug() << "Database Foreign Key : "
//                         << _query.value(0).toInt();
//           }
//        }

//    }
    if(!_query.exec("PRAGMA SYNCHRONOUS = OFF;"))
    {
         qDebug() << __PRETTY_FUNCTION__ << " - "  << __LINE__ <<
                     "Synchronous error : " << _query.lastError().text() ;
    }
    if(!_query.exec("PRAGMA journal_mode = MEMORY;"))
    {
         qDebug() << __PRETTY_FUNCTION__ << " - "  << __LINE__ <<
                     "Journal mode error : " << _query.lastError().text() ;
    }
    //auto add_db = QSqlDatabase::cloneDatabase(database, "ADD");
//    if(!add_db.open())
//    {
//        std::cout << __FUNCTION__
//                  << " Could not be opened. "
//                  << add_db.lastError().text().toStdString()
//                  << std::endl
//                  << "Exiting ..." << std::endl;
//        QSqlDatabase::removeDatabase("ADD");
//        exit(-1);

//    }
    add_query = QSqlQuery(db);
}


DataAccessObject::~DataAccessObject()
{
    qDebug() << __PRETTY_FUNCTION__ << " quitting ...";
    _query.clear();
    add_query.clear();
    database.commit();
    QSqlDatabase::removeDatabase(connectionName);
    QSqlDatabase::removeDatabase("ADD");
    database.close();
}

bool DataAccessObject::isOpen()const
{
    return database.isOpen();
}

void DataAccessObject::close()
{
    database.close();
}

void DataAccessObject::commit()
{
    database.commit();
}


QSqlQuery DataAccessObject::queryTrack(int trackID)
{
    QString cmd = QString( "SELECT * FROM BaseTableTracks where trackID=%1; ").arg(QString::number(trackID));

    return query(cmd);
}

QSqlQuery DataAccessObject::queryTrack(const QString &title)
{
    QString cmd = QString( "SELECT * FROM BaseTableTracks where title='%1'; ").arg(title);

    return query(cmd);
}

void DataAccessObject::addTrack(const QString &path)
{
    add_query.prepare(
                "INSERT INTO BaseTableTracks (albumTitle,artist, albumArtist,title, genre,trackUrl,cover,trackNumber,length,playCount, favorite, bitrate, year, addedDate, modifiedDate) "
                "VALUES (?, ?, ?,?, ?, ?,?, ?, ?,?, ?, ?,?,?,?)");
    Track track = Tagreader::tagreader(path);
    add_query.bindValue(0, track.albumTitle);
    add_query.bindValue(1, track.artist);
    add_query.bindValue(2, track.albumArtist);
    add_query.bindValue(3, track.title);
    add_query.bindValue(4, track.genre);
    add_query.bindValue(5, track.path);
    add_query.bindValue(6, track.cover);
    add_query.bindValue(7, QString::number(track.position));
    add_query.bindValue(8, QString::number(track.length));
    add_query.bindValue(9, QString::number(track.playCount));
    add_query.bindValue(10,QString::number(track.liked));
    add_query.bindValue(11,QString::number(track.bitRate));
    add_query.bindValue(12,QString::number(track.year));
    add_query.bindValue(13,QString::number(track.addedDate));
    add_query.bindValue(14,QString::number(track.modifiedDate));
    add_query.exec();
}

void DataAccessObject::addTrack(const Track &track)
{

    if(database.isOpen())
    {
         //add_query = QSqlQuery(database);
         if(add_query.prepare(
                     "INSERT INTO BaseTableTracks (albumTitle,artist, albumArtist, title, genre,trackUrl,cover,trackNumber,length,playCount, favorite, bitrate, year, addedDate, modifiedDate) "
                     "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);"))
         {
             add_query.bindValue(0, track.albumTitle);
             add_query.bindValue(1, track.artist);
             add_query.bindValue(2, track.albumArtist);
             add_query.bindValue(3, track.title);
             add_query.bindValue(4, track.genre);
             add_query.bindValue(5, track.path);
             add_query.bindValue(6, track.cover);
             add_query.bindValue(7, QString::number(track.position));
             add_query.bindValue(8, QString::number(track.length));
             add_query.bindValue(9, QString::number(track.playCount));
             add_query.bindValue(10,QString::number(track.liked));
             add_query.bindValue(11,QString::number(track.bitRate));
             add_query.bindValue(12,QString::number(track.year));
             add_query.bindValue(13,QString::number(track.addedDate));
             add_query.bindValue(14,QString::number(track.modifiedDate));
             if(!add_query.exec())
             {
                 qDebug() << __PRETTY_FUNCTION__
                          << " line "
                          << __LINE__
                          <<" error :" << add_query.lastError();
             }

         }

         else
         {
             qDebug() << __PRETTY_FUNCTION__ << " Query Prepare Error : "
                      << add_query.lastError().text();
         }
    }
    else
    {

        qDebug() << __PRETTY_FUNCTION__
                 << " line "
                 << __LINE__
                 <<"database openError : " << database.lastError();
    }

}

void DataAccessObject::removeTrack(int trackID)
{
    if(trackID > 0)
    {
        std::lock_guard<std::mutex>dbLock(databaseMutex);
        if(database.open())
        {
            //_query = QSqlQuery(database);
           add_query.exec(QString("DELETE FROM BaseTracksTable WHERE trackID = %1;").arg(QString::number(trackID)));
        }
        else
        {
            qDebug() << "Error on DataAccess::removeTrack(trackID): "
                     << "the database couldn't be opened.";
            qDebug()<< "DataAccess::removeTrack(trackID): "
                    << database.lastError();
        }
    }
}


QSqlQuery DataAccessObject::query(const QString &command)
{
    //qDebug() << __PRETTY_FUNCTION__ << " called ...";
    QSqlQuery result;
    if(!command.isEmpty())
    {
        if(database.isOpen())
        {
            result = database.exec(command);
            //database.commit();
            //qDebug() << __PRETTY_FUNCTION__ << " command executed  : "
                     //<< command ;
            if(result.lastError().isValid())
            {
                qDebug() << __PRETTY_FUNCTION__
                         << " Error : database query command Error :"
                         << "Command issued :: " << command << "\n"
                         << " Error tracked : "
                         << result.lastError().text();
            }
        }
        else
        {
            qDebug() << __PRETTY_FUNCTION__ << " Error : database is not opened.";
            if(database.isOpenError())
            {
                qDebug() << __PRETTY_FUNCTION__ << " Error : database isOpenError : "
                         << database.lastError().text();
            }
        }

    }
    else
    {
        qDebug() << __PRETTY_FUNCTION__ << " Error : empty query .";
    }

    return result;
}


