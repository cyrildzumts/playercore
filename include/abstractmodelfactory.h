#ifndef ABSTRACTMODELFACTORY_H
#define ABSTRACTMODELFACTORY_H

#include "abstractmodel.h"

class AbstractModelFactory
{
public:
    virtual ~AbstractModelFactory();
    static AbstractModelFactory *instance();
    AbstractModel *createModel(const QString &cmd) ;
    AbstractModel *createAlbumModel();
    AbstractModel *createPlaylistModel();
    AbstractModel *createRecentModel(int day_limit = 2);
    AbstractModel *createSearchResultModel();
    AbstractModel *createArtistAlbumModel(const QString &artist);
    AbstractModel *createByGenreModel(const QString& genre);
public:

private:
    AbstractModelFactory();
    static AbstractModelFactory* _instance;
};

#endif // ABSTRACTMODELFACTORY_H
