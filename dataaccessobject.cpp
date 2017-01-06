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
    connectionName = QString("DataAccess");
    database = QSqlDatabase::addDatabase("QSQLITE",connectionName);
    database.setDatabaseName(databasePath);
    addedDate = 0;
    modifiedDate = 0;
    if(!database.open())
    {
        std::cout << __FUNCTION__
                  << " Could not be opened. "
                  << database.lastError().text().toStdString()
                  << std::endl
                  << "Exiting ..." << std::endl;
        QSqlDatabase::removeDatabase(connectionName);
        exit(-1);

    }
    _query = QSqlQuery(database);
    _query.exec("PRAGMA foreign_keys = ON ;");
    _query.exec("PRAGMA SYNCHRONOUS = OFF;");
    _query.exec("PRAGMA journal_mode = MEMORY;");
}


DataAccessObject::~DataAccessObject()
{
    _query.finish();
    database.close();
    QSqlDatabase::removeDatabase(connectionName);
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
    _query.prepare(
                "INSERT INTO BaseTableTracks (albumTitle,artist, albumArtist,title, genre,trackUrl,cover,trackNumber,length,playCount, favorite, bitrate, year, addedDate, modifiedDate) "
                "VALUES (?, ?, ?,?, ?, ?,?, ?, ?,?, ?, ?,?,?,?)");
    Track track = Tagreader::tagreader(path);
    _query.bindValue(0, track.albumTitle);
    _query.bindValue(1, track.artist);
    _query.bindValue(2, track.albumArtist);
    _query.bindValue(3, track.title);
    _query.bindValue(4, track.genre);
    _query.bindValue(5, track.path);
    _query.bindValue(6, track.cover);
    _query.bindValue(7, QString::number(track.position));
    _query.bindValue(8, QString::number(track.length));
    _query.bindValue(9, QString::number(track.playCount));
    _query.bindValue(10,QString::number(track.liked));
    _query.bindValue(11,QString::number(track.bitRate));
    _query.bindValue(12,QString::number(track.year));
    _query.bindValue(13,QString::number(track.addedDate));
    _query.bindValue(14,QString::number(track.modifiedDate));
    _query.exec();
}

void DataAccessObject::addTrack(const Track &track)
{

    if(!database.open())
    {
        qDebug() << __PRETTY_FUNCTION__
                 << " line "
                 << __LINE__
                 <<"database openError : " << database.lastError();
    }
    else
    {
        _query = QSqlQuery(database);
        _query.prepare(
                    "INSERT INTO BaseTableTracks (albumTitle,artist, albumArtist, title, genre,trackUrl,cover,trackNumber,length,playCount, favorite, bitrate, year, addedDate, modifiedDate) "
                    "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
        _query.bindValue(0, track.albumTitle);
        _query.bindValue(1, track.artist);
        _query.bindValue(2, track.albumArtist);
        _query.bindValue(3, track.title);
        _query.bindValue(4, track.genre);
        _query.bindValue(5, track.path);
        _query.bindValue(6, QString(track.cover).replace(QLatin1Char('\''), QLatin1String("''")));
        _query.bindValue(7, QString::number(track.position));
        _query.bindValue(8, QString::number(track.length));
        _query.bindValue(9, QString::number(track.playCount));
        _query.bindValue(10,QString::number(track.liked));
        _query.bindValue(11,QString::number(track.bitRate));
        _query.bindValue(12,QString::number(track.year));
        _query.bindValue(13,QString::number(track.addedDate));
        _query.bindValue(14,QString::number(track.modifiedDate));
        if(!_query.exec())
        {
            qDebug() << __PRETTY_FUNCTION__
                     << " line "
                     << __LINE__
                     <<" error :" << _query.lastError();
        }

        else
        {
            qDebug() << __PRETTY_FUNCTION__
                     << " line "
                     << __LINE__
                     << " title added :"
                     << track.title;
        }

        _query.finish();
        _query.clear();
    }

}

void DataAccessObject::removeTrack(int trackID)
{
    if(trackID > 0)
    {
        std::lock_guard<std::mutex>dbLock(databaseMutex);
        if(database.open())
        {
            _query = QSqlQuery(database);
            _query.exec(QString("DELETE FROM BaseTracksTable WHERE trackID = %1;").arg(QString::number(trackID)));
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
    QSqlQuery result;
    if(database.open())
    {
        result = database.exec(command);
    }
    return result;
}


