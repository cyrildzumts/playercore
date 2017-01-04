#include "facade.h"

// PlayerFacade definition

PlayerFacade::PlayerFacade(const QString &conf)
{
    try
    {
        settings = new SettingManager(conf);
    }
    catch(InexistentConfigException &e)
    {
        std::cout << e.what() << std::endl;
        delete settings;
        settings = nullptr;
        exit(EXIT_FAILURE);
    }
    QString database_path = settings->getDatabasePath();

    data_access_object = new DataAccessObject(database_path);
    mediascanner = new MediaScanner(data_access_object);

    playlist = new Playlist2(data_access_object);
    player = new Player();
    albums = new AlbumModel(data_access_object);
    tracklists = new TracklistModel(data_access_object);
    playlists = nullptr;
    init();
}

PlayerFacade::~PlayerFacade()
{
    int position = player->position();
    player->stop();
    int index = playlist->currentIndex();
    int mode = playlist->playbackMode();
    std::vector<Track> tracks = playlist->tracklist();
    std::vector<int> ids;
    for(auto track : tracks)
    {
        ids.push_back(track.ID);
    }
    settings->setPlayerPlaybackMode(mode);
    settings->setPlayerPosition(position);
    settings->setPlayerTrackIndex(index);
    settings->setPlaylistContent(ids);
    delete playlist;
    delete player;
    delete mediascanner;
    delete albums;
    delete tracklists;
    delete playlists;
    delete data_access_object;
    delete settings;


}

void PlayerFacade::init()
{
    int index = settings->playerTrackIndex();
    int position = settings->playerPosition();
    int mode = settings->playerPlaybackMode();
    std::vector<int> ids = settings->playlistContent();
    QString mediaSourcePath = settings->getMediaSourcePath();
    mediascanner->setSourceDirectory(mediaSourcePath);
    mediascanner->scanSourceDirectory();
    playlist->setTracklist(ids);
    playlist->setPlaybackMode(mode);
    player->setPlaylist(playlist);
    player->setPosition(position);
    playlist->setCurrentIndex(index);
    albums->refresh();
}

void PlayerFacade::update()
{
    mediascanner->updateData();
}


void PlayerFacade::playAlbum(const QString &album)
{
    playlist->addAlbum(album);
}

AbstractModel *PlayerFacade::albumModel()
{
    return albums;
}

AbstractModel *PlayerFacade::tracklistModel()
{
    return tracklists;
}

AbstractModel *PlayerFacade::playlistModel()
{
    return playlists;
}

AbstractModel *PlayerFacade::currentPlaylist()
{
    return playlist;
}

void PlayerFacade::play()
{
    player->play();
}

void PlayerFacade::stop()
{
    player->stop();
}

void PlayerFacade::pause()
{
    player->pause();
}

int PlayerFacade::currentPosition()
{
    return player->position();
}

void PlayerFacade::addTrack(int ID)
{
    playlist->addMedia(ID);
}

void PlayerFacade::removeTrack(int pos)
{
    playlist->removeMedia(pos);
}

void PlayerFacade::next()
{
    playlist->next();
}

void PlayerFacade::previous()
{
    playlist->previous();
}

void PlayerFacade::setPlaybackMode(int mode)
{
    playlist->setPlaybackMode(mode);
}

int PlayerFacade::playbackMode() const
{
    return playlist->playbackMode();
}

int PlayerFacade::currentIndex() const
{
    return playlist->currentIndex();
}

int PlayerFacade::duration() const
{
    return playlist->duration();
}

QString PlayerFacade::durationStr() const
{
    return playlist->durationStr();
}

void PlayerFacade::setCover(QString path)
{
    playlist->setCover(path);
}

QString PlayerFacade::cover() const
{
    return playlist->cover();
}

QString PlayerFacade::album() const
{
    return playlist->album();
}

QString PlayerFacade::title() const
{
    return playlist->title();
}

QString PlayerFacade::artist() const
{
    return playlist->artist();
}

int PlayerFacade::year() const
{
    return playlist->year();
}

int PlayerFacade::mediacount() const
{
    return playlist->mediaCount();
}

int PlayerFacade::length() const
{
    return player->duration();
}

QString PlayerFacade::lengthStr() const
{
    return player->durationString();
}


// FacadeStubs definition


void FacadeStubs::update()
{
    qDebug()<< __FUNCTION__ << " called." ;
}

AbstractModel *FacadeStubs::albumModel()
{
    qDebug()<< __FUNCTION__ << " called." ;
    return nullptr;
}

AbstractModel *FacadeStubs::tracklistModel()
{
    qDebug()<< __FUNCTION__ << " called." ;
    return nullptr;
}

AbstractModel *FacadeStubs::playlistModel()
{
    qDebug()<< __FUNCTION__ << " called." ;
    return nullptr;
}

AbstractModel *FacadeStubs::currentPlaylist()
{
    return nullptr;
}

void FacadeStubs::play()
{
    qDebug()<< __FUNCTION__ << " called." ;
}

void FacadeStubs::stop()
{
    qDebug()<< __FUNCTION__ << " called." ;
}

void FacadeStubs::pause()
{
    qDebug()<< __FUNCTION__ << " called." ;
}

int FacadeStubs::currentPosition()
{
    qDebug()<< __FUNCTION__ << " called." ;
    return 256;
}

void FacadeStubs::addTrack(int ID)
{
    qDebug()<< __FUNCTION__ << " called with param : "
            << ID;
}

void FacadeStubs::removeTrack(int ops)
{
    qDebug()<< __FUNCTION__ << " called with param : "
            << ops;
}

void FacadeStubs::next()
{
    qDebug()<< __FUNCTION__ << " called.";
}

void FacadeStubs::previous()
{
    qDebug()<< __FUNCTION__ << " called.";
}

void FacadeStubs::setPlaybackMode(int mode)
{
    qDebug()<< __FUNCTION__ << " called.";
}

int FacadeStubs::playbackMode() const
{
    qDebug()<< __FUNCTION__ << " called.";
    return 2;
}

int FacadeStubs::currentIndex() const
{
    qDebug()<< __FUNCTION__ << " called.";
    return 5;
}

int FacadeStubs::duration() const
{
    qDebug()<< __FUNCTION__ << " called.";
    return 0;
}

QString FacadeStubs::durationStr() const
{
    qDebug()<< __FUNCTION__ << " called.";
    return "4:12";
}

void FacadeStubs::setCover(QString path)
{
    qDebug()<< __FUNCTION__ << " called.";
}

QString FacadeStubs::cover() const
{
    qDebug()<< __FUNCTION__ << " called.";
    return "";
}

QString FacadeStubs::album() const
{
    qDebug()<< __FUNCTION__ << " called.";
    return "Stubs Album";
}

QString FacadeStubs::title() const
{
    qDebug()<< __FUNCTION__ << " called.";
    return "Stubs Title";
}

QString FacadeStubs::artist() const
{
    qDebug()<< __FUNCTION__ << " called.";
    return "Stubs Artist";
}

int FacadeStubs::year() const
{
    qDebug()<< __FUNCTION__ << " called.";
    return 2016;
}

int FacadeStubs::mediacount() const
{
    return 0;
}





int FacadeStubs::length() const
{
    return 0;
}


QString FacadeStubs::lengthStr() const
{
    return "";
}


