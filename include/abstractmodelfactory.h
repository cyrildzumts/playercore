#ifndef ABSTRACTMODELFACTORY_H
#define ABSTRACTMODELFACTORY_H

#include "abstractmodel.h"

class AbstractModelFactory
{
public:
    virtual ~AbstractModelFactory(){}
    AbstractModel *createModel(const QString &cmd) ;
    AbstractModel *createAlbumModel();
    AbstractModel *createPlaylistModel();
    AbstractModel *createRecentModel(int day_limit = 2);
    AbstractModel *createSearchResultModel();
    AbstractModel *createArtistAlbumModel(const QString &artist);
    AbstractModel *createByGenreModel(const QString& genre);
public:



};

#endif // ABSTRACTMODELFACTORY_H
