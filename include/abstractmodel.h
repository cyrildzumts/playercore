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

class AbstractModel : public QAbstractListModel
{
    Q_OBJECT
public:

    virtual ~AbstractModel(){}
    void setRoles(const QHash<int, QByteArray> &roles);

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
    virtual void refresh();
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
    ~AlbumModel();
public Q_SLOTS:
protected:
    virtual void init();
private:
    //QString queryStr;
    QSqlQuery query;
};


class ArtistModel : AbstractModel
{
public:
    ArtistModel(AbstractDataAccessObject *data_access);

    // AbstractModel interface
protected:
    virtual void init() override;
};

class TracklistModel : public AbstractModel
{
    Q_OBJECT
public:
    TracklistModel(AbstractDataAccessObject *data_access = nullptr);

    ~TracklistModel();
    // AbstractModel interface
protected:
    virtual void init() override;

public Q_SLOTS:
    virtual void refresh() override;
    void onQueryChanged();

};

class GenreModel : public AbstractModel
{
Q_OBJECT
public:
    GenreModel(AbstractDataAccessObject  *data_access = nullptr);
    ~GenreModel();
public Q_SLOTS:
protected:
    virtual void init();
    QSqlQuery _query;
};

#endif // ABSTRACTMODEL_H


