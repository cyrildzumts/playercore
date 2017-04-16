#include "abstractmodel.h"

void AbstractModel::setRoles(const QHash<int, QByteArray> &roles)
{
    if(_roles != roles)
        _roles = roles;
}

void AbstractModel::populate()
{
    if(!_query.exec(queryStr))
    {
        qDebug() << __PRETTY_FUNCTION__
                 << _query.lastError().text();
    }
}

void AbstractModel::setDataAccessor(AbstractDataAccessObject *data_access)
{
    this->data_access = data_access;
}

QVariant AbstractModel::data(const QModelIndex &index, int role)const
{
    QVariant result = QVariant();
    QSqlQuery query = QSqlQuery(_query);

    if(!(index.row() < 0 || index.row() >= rowCount()))
    {
        if(query.isSelect())
        {
            if(query.first())
            {
                if(query.isValid())
                {
                    if(query.seek(index.row()))
                    {
                        result = query.value(_roles[role].data());
                    }
                    else
                    {
                        qDebug()<< "AbstractModel::data(): Seek Error on query\n"
                                << "Error : "
                                << query.lastError();
                    }
                }
                else
                {
                    qDebug()<< "AbstractModel::data(): Error query isn't valid\n"
                            << "Error : "
                            << query.lastError();
                }
            }
            else
            {
                qDebug()<< "AbstractModel::data(): Error query first failed\n";
            }

        }
        else
        {
            qDebug()<< "AbstractModel::data(): Error query isn't select\n"
                    << "Error : "
                    << query.lastError();
        }
    }
    else
    {
        qDebug()<< "AbstractModel::data(): Error query execution failed\n";
    }

    return result;
}

int AbstractModel::rowCount(const QModelIndex &parent)const
{
    int i = 0;
    QSqlQuery query(_query);
    if(query.isSelect() and query.isActive())
    {
        query.seek(-1);

        while(query.next())
        {
            i++;
        }
    }

    return i;
}

void AbstractModel::setQuery(QSqlQuery &query)
{
    //resetInternalData();
    _query = query;
    //Q_EMIT queryChanged();
}

void AbstractModel::setQuery(const QString &str)
{
    if( !str.isEmpty() && (queryStr != str) )
    {
        queryStr = str;
        populate();
        Q_EMIT queryChanged();
    }
}

QSqlQuery &AbstractModel::getQuery()
{
    return _query;
}

void AbstractModel::resetInternalData()
{
    beginResetModel();
    _query.finish();
    endResetModel();
}

QHash<int, QByteArray> AbstractModel::roleNames() const
{
    return _roles;
}


QHash<int, QByteArray> AbstractModel::albumRoles()
{
    QHash<int, QByteArray> roles;
    roles[AlbumRole::AlbumTitle] = "title";
    roles[AlbumRole::Coverpath] = "coverpath";
    roles[AlbumRole::Artist]  = "artist";
    roles[AlbumRole::Year]  = "year";
    roles[AlbumRole::ID] = "albumID";
    roles[AlbumRole::TrackCount] ="tracks";
    roles[AlbumRole::Duration] = "duration";
    roles[AlbumRole::AlbumGenre] = "genre";
    return roles;
}

QHash<int, QByteArray> AbstractModel::artistRoles()
{
    QHash<int, QByteArray> roles;
    roles[ArtistRole::ArtistID] = "artistID";
    roles[ArtistRole::ArtistName] = "artist";
    roles[ArtistRole::Description] = "description";
    roles[ArtistRole::ArtistCover] = "cover";
    return roles;
}

QHash<int, QByteArray> AbstractModel::genreRoles()
{
    QHash<int, QByteArray> roles;
    roles[GenreRole::GenreName] = "genre";
    return roles;
}

QHash<int, QByteArray> AbstractModel::playlistRoles()
{
    QHash<int, QByteArray> roles;
    roles[PlaylistRole::PLSTITLE]       = "title";
    roles[PlaylistRole::PLAYLISTID]     = "playlistID";
    roles[PlaylistRole::PLSFAVORITE]    = "favorite";
    return roles;
}

QHash<int, QByteArray> AbstractModel::trackRoles()
{
    QHash<int, QByteArray> roles;
    roles[TrackRole::TrackTitle] = "title";
    roles[TrackRole::Path]  = "trackUrl" ;
    roles[TrackRole::AlbumArtist] = "albumArtist";
    roles[TrackRole::ArtistTrack] = "artist";
    roles[TrackRole::Position] ="trackNumber";
    roles[TrackRole::Length] = "length";
    roles [TrackRole::LengthString] = "lengthStr";
    roles[TrackRole::TrackID] = "trackID";
    roles[TrackRole::TrackAlbum] = "albumTitle";
    roles[TrackRole::Playcount] = "playcount";
    roles[TrackRole::Bitrate] = "bitrate";
    //roles[TrackRole::Year] = "year";
    roles[TrackRole::Favorite] = "favorite";
    roles[TrackRole::Cover] = "cover";
    roles[TrackRole::TrackGenre] = "genre";

    return roles;
}

void AbstractModel::refresh()
{
    resetInternalData();

    beginInsertRows(QModelIndex(), FIRST, FIRST);
    //_query = data_access->query(queryStr);
    if(!_query.exec(queryStr))
    {
        qDebug() << __PRETTY_FUNCTION__
                 << " - Query : "
                 << queryStr << " -- "
                 << _query.lastError().text();
    }
    if( !_query.isActive())
    {
        qDebug()<< "AbstractmModel::refresh(): Error\n"
                << "Error : "
                << _query.lastError();
    }
    else
    {
        qDebug()<< "AbstractmModel::refresh(): \n"
                << "data refreshed ...";
    }
    endInsertRows();
    Q_EMIT dataChanged();
}


AlbumModel::AlbumModel(AbstractDataAccessObject *data_access)
{
    this->data_access = data_access;
    init();
}
AlbumModel::~AlbumModel()
{
    qDebug() << __PRETTY_FUNCTION__ << " called ...";
    //_query.finish();
    _query.clear();

}

void AlbumModel::init()
{
    _roles = AbstractModel::albumRoles();
}


void AlbumModel::test_queries()
{
    //QSqlQuery query1 = data_access->query(queryStr);
    QSqlQuery query1 ;
        if(!query1.exec(queryStr))
        {
            qDebug() << __PRETTY_FUNCTION__
                     << query1.lastError().text();
        }
    QString str = QString("SELECT genre FROM BaseTableTracks GROUP BY genre ORDER BY genre;");
    //QSqlQuery query2 = data_access->query(str);
    QSqlQuery query2 ;
    if(!query2.exec(queryStr))
    {
        qDebug() << __PRETTY_FUNCTION__
                 << query2.lastError().text();
    }
    qDebug() << "First call : Test Album : " ;
    test_album(query1);
    qDebug() << "First call : Test Album done.";



    qDebug() << "First call : Test Genre : " ;
    test_genre(query2);
    qDebug() << "First call : Test Genre done.";

    qDebug() << "Second call : Test Album : " ;

    test_album(query1);
    qDebug() << "Second call : Test Album done ";

}

void AlbumModel::test_album(QSqlQuery &q)
{
    if(q.isSelect() and q.isActive())
    {
        q.seek(-1);
        int i = 0;
        while(q.next())
        {
            i++;
            qDebug() <<i << " - " <<  q.value("title").toString();
        }
    }
}

void AlbumModel::test_genre(QSqlQuery &q)
{
    if(q.isSelect() and q.isActive())
    {
        q.seek(-1);
        int i= 0;
        while(q.next())
        {
            i++;
            qDebug() <<i << " - " <<  q.value("genre").toString();
        }
    }
}

void AlbumModel::filterByGenre(const QString &genre)
{
    QString cmd = QString("SELECT albumTitle as title,albumArtist as artist,"
                           "genre,cover as coverpath,"
                           "COUNT(albumTitle) as tracks ,SUM(length) as duration , "
                           "year, genre FROM BaseTableTracks "
                           "WHERE genre = '%1' "
                           "GROUP BY albumTitle "
                           "ORDER BY albumTitle;").arg(genre);
    setQuery(cmd);
}

void AlbumModel::filterByArtist(const QString &artist)
{
    QString cmd = QString("SELECT albumTitle as title,albumArtist as artist,"
                           "genre,cover as coverpath,"
                           "COUNT(albumTitle) as tracks ,SUM(length) as duration , "
                           "year, genre FROM BaseTableTracks "
                           "WHERE albumArtist = '%1' "
                           "GROUP BY albumTitle "
                           "ORDER BY albumTitle;").arg(artist);
    setQuery(cmd);

}

void AlbumModel::filterRecent(int day_limit)
{

    int recent_date = QDateTime::currentDateTime().toTime_t();
            - (day_limit * DAY_TO_SECONDS);
    QString cmd = QString("SELECT albumTitle as title,albumArtist as artist,"
                           "genre,cover as coverpath,"
                           "COUNT(albumTitle) as tracks ,SUM(length) as duration , "
                           "year, genre FROM BaseTableTracks "
                           "WHERE addedDate >= %1 "
                           "GROUP BY albumTitle "
                           "ORDER BY albumTitle;").arg(QString::number(recent_date));
    setQuery(cmd);
}

void AlbumModel::setDefault()
{
    QString cmd = QString("SELECT albumTitle as title,albumArtist as artist,"
                       "genre,cover as coverpath,"
                       "COUNT(albumTitle) as tracks ,SUM(length) as duration , "
                       "year, genre FROM BaseTableTracks "
                       "GROUP BY albumTitle "
                       "ORDER BY albumTitle;");

    setQuery(cmd);

}



void AlbumModel::viewContent()
{
    int i = 0;
    QSqlQuery query(_query);
    if(query.isSelect() and query.isActive())
    {
        query.seek(-1);

        while(query.next())
        {
            i++;
            qDebug() <<i << " - " <<  query.value("title").toString();
        }
    }
}



ArtistModel::ArtistModel(AbstractDataAccessObject *data_access)
{
    this->data_access  = data_access;
    this->setRoles(AbstractModel::artistRoles());
     queryStr = QString("SELECT albumArtist as artist,cover FROM BaseTableTracks GROUP BY albumArtist ORDER BY albumArtist;");

    connect(this, &ArtistModel::queryChanged,
            this, &ArtistModel::refresh);
}


void ArtistModel::init()
{

    //_query = data_access->query(queryStr);

}


// TRACKLISTMODEL

TracklistModel::TracklistModel(AbstractDataAccessObject *data_access)
{
    this->data_access = data_access;
    _roles = AbstractModel::trackRoles();
    connect(this,&TracklistModel::queryChanged,
            this, &TracklistModel::refresh);
    connect(this,&TracklistModel::queryChanged,
            this, &TracklistModel::onQueryChanged);

}

TracklistModel::~TracklistModel()
{
    resetInternalData();
}

void TracklistModel::populateFromAlbum(const QString &album)
{
    QString cmd = QString("SELECT * FROM BaseTableTracks WHERE  albumTitle ='%1' ORDER BY trackNumber;").arg(album);
    setQuery(cmd);

}

void TracklistModel::populateFromPlaylist(int plsID)
{
    QString cmd = QString("SELECT * FROM BaseTableTracks  NATURAL JOIN (select trackID FROM PlaylistTrack   JOIN Playlist on  PlaylistTrack.playlistID= %1 ORDER BY plsTrackID;").arg(QString::number(plsID));
    setQuery(cmd);
}

void TracklistModel::setDefault()
{
    QString cmd = QString("SELECT * FROM BaseTableTracks;");
    setQuery(cmd);
}


void TracklistModel::init()
{

}


void TracklistModel::onQueryChanged()
{
    qDebug() << __FUNCTION__ << " : Query changed to : "
             << queryStr;
    //_query = data_access->query(queryStr);
}

GenreModel::GenreModel(AbstractDataAccessObject *data_access)
{
    this->data_access = data_access;
    queryStr = QString("SELECT genre FROM BaseTableTracks GROUP BY genre ORDER BY genre;");
    _roles = AbstractModel::genreRoles();
    init();
}

GenreModel::~GenreModel()
{
    qDebug() << __PRETTY_FUNCTION__ << " called ...";
    _query.clear();
}

void GenreModel::init()
{


}


PlaylistModel::PlaylistModel(AbstractDataAccessObject *data_access)
{
    this->data_access = data_access;
    queryStr = QString("SELECT * FROM Playlist;");
    _roles = playlistRoles();
}

PlaylistModel::~PlaylistModel()
{
    qDebug() << __PRETTY_FUNCTION__ << " called ...";
    _query.clear();
}

void PlaylistModel::init()
{
}
