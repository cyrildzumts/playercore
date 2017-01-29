#include "abstractmodelfactory.h"


AbstractModelFactory* AbstractModelFactory::_instance = nullptr;
AbstractModelFactory::AbstractModelFactory()
{

}
AbstractModel *AbstractModelFactory::createAlbumModel()
{

}

AbstractModelFactory::~AbstractModelFactory()
{
    if(_instance)
        delete _instance;
}

AbstractModelFactory *AbstractModelFactory::instance()
{
    if(_instance == nullptr)
        _instance = new AbstractModelFactory();
    return _instance;
}

AbstractModel *AbstractModelFactory::createModel(const QString &cmd)
{

}

AbstractModel *AbstractModelFactory::createPlaylistModel()
{

}

AbstractModel *AbstractModelFactory::createRecentModel(int day_limit)
{

}


AbstractModel *AbstractModelFactory::createSearchResultModel()
{

}

AbstractModel *AbstractModelFactory::createArtistAlbumModel(const QString &artist)
{

}

AbstractModel *AbstractModelFactory::createByGenreModel(const QString &genre)
{

}


