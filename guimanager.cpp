#include "guimanager.h"

GUIManager::GUIManager()
{
    facade = new PlayerFacade(QString());
    qDebug() << __PRETTY_FUNCTION__;
    qDebug() << "Facade states : ";
    qDebug() << "Playlist Playback mode : " << facade->playbackMode();
    qDebug() << "Player State : " << facade->playerState();
    connect(facade, &PlayerFacade::playerStateChanged, this, &GUIManager::isPlayingChanged);
    connect(facade, &Facade::positionChanged, this, &GUIManager::positionChanged);
    connect(facade, &Facade::positionChanged, this, &GUIManager::positionStrChanged);
    connect(facade, &Facade::durationChanged, this, &GUIManager::durationChanged);
    connect(facade, &Facade::lengthChanged, this, &GUIManager::lengthChanged);
    connect(facade, &Facade::lengthChanged, this, &GUIManager::lengthStrChanged);
    connect(facade, &Facade::lengthChanged, this, &GUIManager::coverChanged);
    connect(facade, &Facade::lengthChanged, this, &GUIManager::durationStrChanged);
    connect(facade, &Facade::durationChanged, this, &GUIManager::albumChanged);
    connect(facade, &Facade::lengthChanged, this, &GUIManager::artistChanged);
    connect(facade, &Facade::lengthChanged, this, &GUIManager::titleChanged);
}

GUIManager::~GUIManager()
{
    qDebug() << __PRETTY_FUNCTION__ << " deleted ...";
    delete facade;
}

// Player Interface
void GUIManager::play()
{
    if(isPlaying())
        facade->pause();
    else
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


void GUIManager::setPosition(int position)
{
    facade->seek(position);
}



QString GUIManager::positionStr()
{
    return facade->positionStr();

}


bool GUIManager::isPlaying()
{
    auto state = facade->playerState();
    return state == QMediaPlayer::PlayingState;
}


bool GUIManager::isStopped()
{
    auto state = facade->playerState();
    return state == QMediaPlayer::StoppedState;
}


bool GUIManager::isPaused()
{
    auto state = facade->playerState();
    return state == QMediaPlayer::PausedState;
}

AbstractModel *GUIManager::tracklistModel()
{
    return facade->tracklistModel();
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
    return facade->albumContentModel(titleAlbum);
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

AbstractModel *GUIManager::playlistContent(int plsID)
{
    return facade->playlistContents(plsID);
}


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

void GUIManager::playAlbum(const QString &title, int index)
{
    facade->playAlbum(title, index);
}

void GUIManager::playPlaylist(const QString &title, int index)
{

}

int GUIManager::favorite()
{
    return facade->currentTrackFavorite();
}

QString GUIManager::durationToString(int d)
{
    QTime tmpTime (0,0);
    QTime t =  tmpTime.addSecs(d);
    if(d < HOUR_IN_SECONDS)
    {
       return  t.toString("mm:ss");
    }
    else
    {
        return t.toString("hh:mm:ss");
    }
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

//TODO
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


qint64 GUIManager::length()
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
    //facade->setPlaybackMode(mode);

    switch (mode) {
        case PlaybackMode::CurrentItemOnce:
            repeatModeOnce();
            break;
        case PlaybackMode::CurrentItemInLoop:
            repeatModeOneLoop();
            break;
        case PlaybackMode::Loop:
            repeatModeLoop();
            break;
        case PlaybackMode::Sequential:
            repeatModeSeq();
            break;

    }
}


int GUIManager::mediaCount()
{
    return facade->mediacount();
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
    facade->createPlaylist(pls);
    return true;
}

bool GUIManager::removePlaylist(const QString &pls)
{
    facade->removePlaylist(pls);
    return true;
}

bool GUIManager::removeFromPlaylist(const QString &pls, int trackID)
{
    facade->removeFromPlaylist(pls, trackID);
    return true;
}

bool GUIManager::addToPlaylist(const QString &pls, int trackID)
{
    facade->addToPlaylist(pls, trackID);
    return true;
}

void GUIManager::addCurrentTrackToFavorite()
{
    facade->addCurrentTrackToFavorite();
}

void GUIManager::removeCurrentTrackFromFavorite()
{
    facade->removeCurrentTrackFromFavorite();
}

void GUIManager::addToFavorite(int trackID)
{
    facade->addToFavorite(trackID);
}

void GUIManager::removeFromFavorite(int trackID)
{
    facade->removeFromFavorite(trackID);
}

void GUIManager::shuffle()
{
    std::cout << __FUNCTION__ << "Called on shuffle" << std::endl;
    facade->shuffle();
}

void GUIManager::repeatModeOnce()
{
    facade->repeatModeOnce();
}

void GUIManager::repeatModeSeq()
{
    facade->repeatModeSeq();
}

void GUIManager::repeatModeLoop()
{
    facade->repeatModeLoop();
}

void GUIManager::repeatModeOneLoop()
{
    facade->repeatModeOneLoop();
}





