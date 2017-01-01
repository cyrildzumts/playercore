#include "abstractmodel.h"

void AbstractModel::setRoles(const QHash<int, QByteArray> &roles)
{
    if(_roles != roles)
        _roles = roles;
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
    Q_EMIT queryChanged();
}

void AbstractModel::setQuery(const QString &str)
{
    queryStr = str;

    Q_EMIT queryChanged();

}

QSqlQuery &AbstractModel::getQuery()
{
    return _query;
}

void AbstractModel::resetInternalData()
{
    beginResetModel();
    //_query.finish();
    _query.clear();
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
    _query = data_access->query(queryStr);
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
    //_query.finish();
    _query.clear();

}
void AlbumModel::init()
{
    queryStr = QString("SELECT albumTitle as title,albumArtist as artist,"
                       "genre,cover as coverpath,"
                       "COUNT(albumTitle) as tracks ,SUM(length) as duration , "
                       "year, genre FROM BaseTableTracks "
                       "GROUP BY albumTitle "
                       "ORDER BY albumTitle;");

    _query = data_access->query(queryStr);
    _roles = AbstractModel::albumRoles();

}

/*
void AlbumModel::refresh()
{
    resetInternalData();

    beginInsertRows(QModelIndex(), FIRST, FIRST);
    _query = data_access->query(queryStr);
     if( !_query.isActive())
     {
         qDebug()<< "AlbumModel::refresh(): Error\n"
                 << "Error : "
                 << _query.lastError();
     }
    endInsertRows();
    Q_EMIT dataChanged();

}
*/


ArtistModel::ArtistModel(AbstractDataAccessObject *data_access)
{
    this->data_access  = data_access;
    this->setRoles(AbstractModel::artistRoles());

    connect(this, &ArtistModel::queryChanged,
            this, &ArtistModel::refresh);
}


void ArtistModel::init()
{
    queryStr = QString("SELECT albumArtist as artist,cover FROM BaseTableTracks GROUP BY albumArtist ORDER BY albumArtist;");
    _query = data_access->query(queryStr);
}
