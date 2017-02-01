#ifndef ABSTRACTMODEL_H
#define ABSTRACTMODEL_H

#include <QtSql>
#include <QAbstractListModel>
#include "dataaccessobject.h"
#include "common.h"

constexpr int UserRole  = 0x0100;
constexpr int FIRST = 0;
enum TrackRole
{
    TrackID = UserRole + 1,
    ArtistTrack,
    AlbumArtist,
    TrackAlbum,
    TrackTitle ,
    Path,
    Position,
    Length,
    LengthString,
    Playcount,
    Bitrate,
    Favorite,
    Cover,
    AddedDate,
    TrackGenre
};


enum AlbumRole
{

    AlbumTitle = UserRole + 1,
    Coverpath,
    Artist,
    Year,
    ID,
    TrackCount,
    Duration,
    AlbumGenre
};

enum ArtistRole
{

    ArtistID = UserRole + 1,
    ArtistName,
    ArtistCover,
    Description

};
enum GenreRole
{

    GenreID = UserRole + 1,
    GenreName
};

enum PlaylistRole
{
    PLAYLISTID = UserRole + 1,
    PLSTITLE,
    PLSFAVORITE
};

class AbstractModel : public QAbstractListModel
{
    Q_OBJECT
public:

    virtual ~AbstractModel(){
        qDebug() << __PRETTY_FUNCTION__ << " called ...";
    }
    virtual void setRoles(const QHash<int, QByteArray> &roles);
    virtual void populate();
    virtual void setQuery(QSqlQuery &query);
    virtual void setQuery(const QString &str);
    virtual QSqlQuery &getQuery();
    virtual void setDataAccessor(AbstractDataAccessObject *data_access);
    /**
     * @brief refresh refresh the query old by this model.
     *  Concrete Model needs to implement this method
     * By Default this method does nothing
     */

    static QHash<int, QByteArray> albumRoles();
    static QHash<int, QByteArray> artistRoles();
    static QHash<int, QByteArray> trackRoles();
    static QHash<int, QByteArray> genreRoles();
    static QHash<int, QByteArray> playlistRoles();
//    static AbstractModel* albumModel();
//    static AbstractModel* genreModel();
//    static AbstractModel* traclistModel();
//    static AbstractModel* artistsModel();
//    static AbstractModel* playlistModel();


protected:
    virtual void resetInternalData();
    QHash<int, QByteArray> roleNames()const;
    virtual void init() = 0;
public Q_SLOTS:
    virtual void refresh() ;
    virtual void viewContent() {}
    virtual QVariant data(const QModelIndex &index, int role)const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex())const;
Q_SIGNALS:
    void dataChanged();
    void queryChanged();

protected:
    QString queryStr;
    QHash<int, QByteArray> _roles;
    QSqlQuery _query;
    AbstractDataAccessObject *data_access;

};


class AlbumModel : public AbstractModel
{
Q_OBJECT
public:
    AlbumModel(AbstractDataAccessObject  *data_access = nullptr);
    virtual ~AlbumModel();

    void test_queries();
    void test_album(QSqlQuery &q);
    void test_genre(QSqlQuery &q);
public Q_SLOTS:
    void filterByGenre(const QString &genre);
    void filterByArtist(const QString &artist);
    void filterRecent(int day_limit);
    void setDefault();
    virtual void viewContent();
protected:
    virtual void init() override;
private:


    // QAbstractItemModel interface
public:

};


class ArtistModel : public AbstractModel
{
    Q_OBJECT
public:
    ArtistModel(AbstractDataAccessObject *data_access);

    // AbstractModel interface
protected:

    virtual void init() override;
private:


    // QAbstractItemModel interface
public:


    // AbstractModel interface
public:


public slots:
};

class TracklistModel : public AbstractModel
{
    Q_OBJECT
public:
    TracklistModel(AbstractDataAccessObject *data_access = nullptr);

    ~TracklistModel();
    // AbstractModel interface
    /**
     * @brief populateFromAlbum populate this model
     * with the content from the designated album.
     * @param album The album to query the tracks from.
     */
    void populateFromAlbum(const QString &album);
    /**
     * @brief populateFromPlaylist populate this model
     * with the content from the designated album.
     * @param pls The Playlist to query the Tracks from
     */
    void populateFromPlaylist(int plsID);
    /**
     * @brief setDefault Populate this model with default
     * content. This will populate this model with every
     * tracks available from the database
     */
    void setDefault();
protected:
    virtual void init() override;

public Q_SLOTS:

    void onQueryChanged();

private:

};

class GenreModel : public AbstractModel
{
Q_OBJECT
public:
    GenreModel(AbstractDataAccessObject  *data_access = nullptr);
    virtual ~GenreModel();
public Q_SLOTS:
protected:
    virtual void init() override;

private:



public slots:

};

class PlaylistModel : public AbstractModel
{
    Q_OBJECT
public:
    PlaylistModel(AbstractDataAccessObject  *data_access = nullptr);
    ~PlaylistModel();
    // AbstractModel interface
protected:
    virtual void init() override;
};

#endif // ABSTRACTMODEL_H


