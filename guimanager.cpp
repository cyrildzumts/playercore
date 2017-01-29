#include "guimanager.h"

GUIManager::GUIManager()
{
    facade = new PlayerFacade(QString());
}

GUIManager::~GUIManager()
{
    qDebug() << __PRETTY_FUNCTION__ << " deleted ...";
    delete facade;
}

// Player Interface
void GUIManager::play()
{
    facade->play();
}

void GUIManager::next()
{
    facade->next();
}

void GUIManager::previous()
{
    facade->previous();
}

int GUIManager::position()
{
    return facade->currentPosition();
}

// TODO
void GUIManager::setPosition(int position)
{
    facade->seek(position);
}

// TODO

QString GUIManager::positionStr()
{
    return "";
}

// TODO
bool GUIManager::isPlaying()
{
    return false;
}

// TODO
bool GUIManager::isStopped()
{
    return false;
}

//TODO
bool GUIManager::isPaused()
{
    return false;
}
// Model Object Factories:


AbstractModel *GUIManager::albumModel()
{
    return facade->albumModel();
}

AbstractModel *GUIManager::genreModel()
{
    return facade->genreModel();
}

AbstractModel *GUIManager::albumByGenre(const QString &genreName)
{
    return facade->albumByGenre(genreName);
}

AbstractModel *GUIManager::albumContents(const QString &titleAlbum)
{
    return facade->albumContent(titleAlbum);
}


AbstractModel *GUIManager::artistsModel()
{
    return facade->artistModel();
}

AbstractModel *GUIManager::artistAlbumsModel(const QString &artistName)
{
    return facade->artistAlbumModel(artistName);
}

AbstractModel *GUIManager::recentAlbum()
{
    return facade->recentAlbumsModel();
}

AbstractModel *GUIManager::playlistModel()
{
    return facade->playlistModel();
}

AbstractModel *GUIManager::playlistContent(const QString &pls)
{
    return facade->playlistContents(pls);
}

//TODO
Playlist *GUIManager::now_playingPlaylist()
{
    return static_cast<Playlist*>(facade->currentPlaylist());
}
//TODO
void GUIManager::onAlbumClicked(const QString &title)
{

}


void GUIManager::onPlayAlbumPressed(const QString &title)
{
    facade->playAlbum(title);
}

// Playlist In

void GUIManager::setCurrentIndex(int index)
{
    facade->setCurrentIndex(index);
}

QString GUIManager::title()
{
    return facade->title();
}

QString GUIManager::album()
{
    return facade->album();
}

QString GUIManager::artist()
{
    return facade->artist();
}

QString GUIManager::cover()
{
    return facade->cover();
}

QString GUIManager::genre()
{
    return "Genre";
}

int GUIManager::duration()
{
    return facade->duration();
}

QString GUIManager::durationStr()
{
    return facade->durationStr();
}


int GUIManager::length()
{
    return facade->length();
}

QString GUIManager::lengthStr()
{
    return facade->lengthStr();
}

int GUIManager::currentIndex()
{
    return facade->currentIndex();
}

int GUIManager::playbackMode()
{
    return facade->playbackMode();
}

void GUIManager::setPlaybackMode(int mode)
{
    facade->setPlaybackMode(mode);
}


// TODO
int GUIManager::mediaCount()
{
    return 0;
}

void GUIManager::addTrack(int id)
{
    facade->addTrack(ID);
}

void GUIManager::removeTrack(int pos)
{
    facade->removeTrack(pos);
}

//  Mediascanner Interface :
void GUIManager::update()
{
    facade->update();
}

 // Settings Controller interface
//TODO
void GUIManager::shutdown()
{

}

QString GUIManager::author() const
{
    return facade->author();
}

QString GUIManager::appName() const
{
    return facade->appName();
}

QString GUIManager::version() const
{
    return facade->version();
}

bool GUIManager::createPlaylist(const QString &pls)
{
    return facade->createPlaylist(pls);
}

bool GUIManager::removePlaylist(const QString &pls)
{
    return facade->removePlaylist(pls);
}

bool GUIManager::removeFromPlaylist(const QString &pls, int trackID)
{
    return facade->removeFromPlaylist(pls, trackID);
}

bool GUIManager::addToPlaylist(const QString &pls, int trackID)
{
    facade->addToPlaylist(pls, trackID);
}





