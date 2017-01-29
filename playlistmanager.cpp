#include "playlistmanager.h"

Playlistmanager::Playlistmanager(AbstractDataAccessObject *data_acces)
{
    this->data_acces = data_acces;
}

Playlistmanager::~Playlistmanager()
{
    qDebug() << __PRETTY_FUNCTION__ << " quitting ...";
}


int Playlistmanager::createPlaylist(const QString& title)
{
    QSqlQuery query;
    int id = -1;
    if (query.exec(QString("REPLACE INTO Playlist (title) VALUES('%1') ;").arg(title)))
    {
        id = query.lastInsertId().toInt();
    }
    else
    {
        qDebug() << __PRETTY_FUNCTION__ << " : "
                 << query.lastError().text();
    }

    return id;
}

bool Playlistmanager::removePlaylist(int playlistID)
{

    auto query = data_acces->query("BEGIN TRANSACTION");
    query.exec(QString("DELETE  * FROM  PlaylistTrack  WHERE trackplaylistID='%1' ;").arg(QString::number(playlistID)));
    query.exec(QString("DELETE  * FROM  Playlist  WHERE playlistID='%1' ;").arg(QString::number(playlistID)));
    query.exec("END TRANSACTION");
    return query.lastError().isValid();
}

bool Playlistmanager::addToPlaylist(int playlistID, int trackID)
{
    QSqlQuery query ;
    bool flag;
    flag = query.exec(QString("INSERT INTO PlaylistTrack (playlistID,trackID) VALUES('%1', '%2') ;").arg(QString::number(playlistID),QString::number(trackID)));
    if(!flag)
    {
        qDebug()<< __PRETTY_FUNCTION__
                << " : "
                << query.lastError().text();
    }
    return flag;
}

bool Playlistmanager::removeFromPlaylist(int playlistID, int trackID)
{
    QSqlQuery query ;
    bool flag;
    flag = query.exec(QString("DELETE  * FROM  PlaylistTrack  WHERE trackplaylistID='%1' AND trackID ='%2' ;").arg(QString::number(playlistID),QString::number(trackID)));

    if(!flag)
    {
        qDebug()<< __PRETTY_FUNCTION__
                << " : "
                << query.lastError().text();
    }
    return flag;
}

bool Playlistmanager::removePlaylist(const QString &pls)
{
    bool flag = false;
    if(!pls.isEmpty() && !(pls == QString(" ")))
     {
        auto query = data_acces->query(QString("DELETE  FROM Playlist WHERE title ='%1';").arg(pls));
        flag = query.lastError().isValid();

    }
        return flag;
}

bool Playlistmanager::addToPlaylist(const QString &pls, int trackID)
{
    bool flag = false;
    if((trackID >0) && !pls.isEmpty() && !(pls == QString(" ")))
      {

        int playlistID = -1;
        auto query = data_acces->query(QString("SELECT playlistID FROM Playlist WHERE title ='%1';").arg(pls));
        if (query.isActive())
        {
            if(query.next())
            {
                playlistID = query.value("playlistID").toInt();
                flag = addToPlaylist(playlistID, trackID);
            }
        }

    }
        return flag;
}

bool Playlistmanager::removeFromPlaylist(const QString &pls, int trackID)
{
    bool flag = false;
    if((trackID >0) && !pls.isEmpty() && !(pls == QString(" ")))
      {

        int playlistID = -1;
        auto query = data_acces->query(QString("SELECT playlistID FROM Playlist WHERE title ='%1';").arg(pls));
        if (query.isActive())
        {
            if(query.next())
            {
                playlistID = query.value("playlistID").toInt();
                flag = removeFromPlaylist(playlistID, trackID);
            }
        }

    }
        return flag;
}

void Playlistmanager::setDataAccessObject(AbstractDataAccessObject *data_acces)
{
    this->data_acces = data_acces;
}
