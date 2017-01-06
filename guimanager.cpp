#include "guimanager.h"

GUIManager::GUIManager()
{
    facade = new PlayerFacade(QString());
}

GUIManager::~GUIManager()
{
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

}

// TODO

QString GUIManager::positionStr()
{
    return "";
}

// TODO
bool GUIManager::isPlaying()
{

}

// TODO
bool GUIManager::isStopped()
{

}

//TODO
bool GUIManager::isPaused()
{

}
// Model Object Factories:


AbstractModel *GUIManager::albumModel()
{
    return facade->albumModel();
}

//TODO
AbstractModel *GUIManager::albumContents()
{

}


//TODO
AbstractModel *GUIManager::artistsModel()
{

}

//TODO
AbstractModel *GUIManager::artistAlbumsModel()
{

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

//TODO
void GUIManager::onPlayAlbumPressed(const QString &title)
{
    facade->playAlbum(title);
}

// Playlist In

//TODO
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

//TODO
int GUIManager::length()
{
    return facade->length();
}

// TODO
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



