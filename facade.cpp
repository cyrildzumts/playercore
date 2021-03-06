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
    playlistManager = new Playlistmanager(data_access_object);
    playlist = new Playlist2(data_access_object);
    player = new Player();
    player->setPlaylist(playlist);
    albums = new AlbumModel(data_access_object);
    artists = new ArtistModel(data_access_object);
    artistAlbums = new AlbumModel(data_access_object);
    search_result = new AlbumModel(data_access_object);
    plsContents = new TracklistModel(data_access_object);
    albumContent = new TracklistModel(data_access_object);
    genres = new GenreModel(data_access_object);
    tracklists = new TracklistModel(data_access_object);
    playlists = new PlaylistModel(data_access_object);

    //search_result = nullptr;


    init();
}

PlayerFacade::~PlayerFacade()
{
    qDebug() << __PRETTY_FUNCTION__ << " called...";
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
    qDebug() << __PRETTY_FUNCTION__ << " saving application states";
    settings->setPlayerPlaybackMode(mode);
    settings->setPlayerPosition(position);
    settings->setPlayerTrackIndex(index);
    settings->setPlaylistContent(ids);
    qDebug() << __PRETTY_FUNCTION__ << " application states saved";


    disconnect(player, &Player::stateChanged, this, &PlayerFacade::playerStateChanged);
    //delete Busmanager;
    delete playlistManager;
    delete player;
    delete playlist;
    delete playlists;
    delete mediascanner;
    delete albums;
    delete genres;
    delete tracklists;

    delete data_access_object;
    delete settings;
    delete artistAlbums;
    delete search_result;
    delete plsContents;
    delete albumContent;
    qDebug() << __PRETTY_FUNCTION__ << " deleted ...";


}

void PlayerFacade::init()
{
    int index = settings->playerTrackIndex();
    int position = settings->playerPosition();
    int mode = settings->playerPlaybackMode();
    std::vector<int> ids = settings->playlistContent();
    QString mediaSourcePath = settings->getMediaSourcePath();
    mediascanner->init(mediaSourcePath);
    mediascanner->scanSourceDirectory();
    // populate the current playlist
    playlist->setTracklist(ids);
    playlist->setPlaybackMode(mode);
    player->setPlaylist(playlist);
    player->setPosition(position);
    playlist->setCurrentIndex(index);

    artists->populate();
    playlists->populate();
    //albums->test_queries();
    static_cast<AlbumModel*>(albums)->setDefault();
    static_cast<TracklistModel*>(tracklists)->setDefault();
//    qDebug() << "Albums Contents :";
//    albums->viewContent();
    genres->populate();

//    qDebug() << "Second Albums Contents :";
//    albums->viewContent();
//    qDebug() << "Genres Contents :";
//    genres->viewContent();

    connect(player, &Player::stateChanged, this, &PlayerFacade::playerStateChanged);
    connect(player, &Player::durationChanged, this, &Facade::lengthChanged);
    connect(player, &Player::positionChanged, this, &Facade::positionChanged);
    connect(playlist, &Playlist2::durationChanged, this, &Facade::durationChanged);

}

void PlayerFacade::update()
{
    mediascanner->updateData();
}
void PlayerFacade::playPlaylist(const QString &title, int index)
{
    std::cout << "Not implemented yet ..." << std::endl;
}

void PlayerFacade::playAlbum(const QString &album, int index)
{
    QString current_media;
    playlist->clear();
    playlist->addAlbum(album);
    std::cout << __PRETTY_FUNCTION__ << " --  Playlist tracks count : " << playlist->mediaCount() << std::endl;
    playlist->setCurrentIndex(index);
    current_media = playlist->media();
    std::cout << __PRETTY_FUNCTION__ << " -- current media path : " << current_media.toStdString() << " -- current index : " 
     << playlist->currentIndex() << std::endl ;

    //playlist->showContents();
    player->setMedia(playlist->media());
    player->play();
    //Busmanager->nofity(playlist->album(),playlist->artist(), playlist->cover());
}

AbstractModel *PlayerFacade::albumModel()
{

    if(albums)
    {
//        qDebug() << __PRETTY_FUNCTION__  << " Album List :";
//        albums->refresh();
//        albums->viewContent();
//        qDebug() << __PRETTY_FUNCTION__ << " Album List End";
    }
    return albums;
}


AbstractModel *PlayerFacade::albumContentModel(const QString &albumTitle)
{
    static_cast<TracklistModel*>(albumContent)->populateFromAlbum(albumTitle);
    return albumContent;
}

AbstractModel *PlayerFacade::artistModel()
{
    return artists;
}

AbstractModel *PlayerFacade::albumByGenre(const QString &genreName)
{
    static_cast<AlbumModel*>(albums)->filterByGenre(genreName);
    return albums;
}

AbstractModel *PlayerFacade::artistAlbumModel(const QString &artistName)
{
    static_cast<AlbumModel*>(artistAlbums)->filterByArtist(artistName);
    return albums;
}

AbstractModel *PlayerFacade::recentAlbumsModel()
{
    static_cast<AlbumModel*>(albums)->filterRecent(3);
    return albums;
}

AbstractModel *PlayerFacade::playlistContents(int plsID)
{
    static_cast<TracklistModel*>(plsContents)->populateFromPlaylist(plsID);
    return plsContents;
}


AbstractModel *PlayerFacade::tracklistModel()
{

    return tracklists;
}

AbstractModel *PlayerFacade::playlistModel()
{
    return playlists;
}

AbstractModel *PlayerFacade::genreModel()
{
    if(genres)
    {
//        qDebug() << __PRETTY_FUNCTION__  << " Genre List :";
//        genres->refresh();
//        genres->viewContent();
//        qDebug() << __PRETTY_FUNCTION__ << " Genre List End";
    }
    return genres;
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

void PlayerFacade::setCurrentIndex(int index)
{
    playlist->setCurrentIndex(index);
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
    return static_cast<int>(player->duration());
}

QString PlayerFacade::lengthStr() const
{
    return player->durationString();
}


void PlayerFacade::createPlaylist(const QString &title)
{
    playlistManager->createPlaylist(title);
}

void PlayerFacade::addToPlaylist(const QString &pls, int trackID)
{
    playlistManager->addToPlaylist(pls, trackID);
}

void PlayerFacade::removePlaylist(const QString &pls)
{
    playlistManager->removePlaylist(pls);
}

void PlayerFacade::removeFromPlaylist(const QString &pls, int trackID)
{
    playlistManager->removeFromPlaylist(pls, trackID);
}

void PlayerFacade::seek(int pos)
{
    //qDebug() << __PRETTY_FUNCTION__ << "Seek position to " << pos;
    player->setPosition(pos);
}

void PlayerFacade::setMedia(const QString &path)
{
    player->setMedia(path);
}

void PlayerFacade::addAlbum(const QString &album)
{
    playlist->addAlbum(album);
}

int PlayerFacade::bitrate()
{
    return playlist->bitrate();
}

void PlayerFacade::sendNotify(const QString &summary, const QString &body, const QString &icon)
{
    Busmanager->setNotification(summary, body, icon);
}

QMediaPlayer::State PlayerFacade::playerState()
{
    return player->state();
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




QString PlayerFacade::author() const
{
    return settings->authorName();
}

QString PlayerFacade::appName() const
{
    return settings->applicationName();
}

QString PlayerFacade::version() const
{
    return settings->applicationVersion();
}


void FacadeStubs::playAlbum(const QString &album, int index)
{
}

void FacadeStubs::setCurrentIndex(int index)
{
}

QString FacadeStubs::author() const
{
    return "";
}

QString FacadeStubs::appName() const
{
    return "";
}

QString FacadeStubs::version() const
{
    return "";
}



void FacadeStubs::createPlaylist(const QString &title)
{
}

void FacadeStubs::addToPlaylist(const QString &pls, int trackID)
{
}

void FacadeStubs::removePlaylist(const QString &pls)
{
}

void FacadeStubs::removeFromPlaylist(const QString &pls, int trackID)
{

}

AbstractModel *FacadeStubs::genreModel()
{
    return nullptr;
}

AbstractModel *FacadeStubs::albumContentModel(const QString &albumTitle)
{
    return nullptr;
}

AbstractModel *FacadeStubs::artistModel()
{
    return nullptr;
}

AbstractModel *FacadeStubs::albumByGenre(const QString &genreName)
{
    return nullptr;
}

AbstractModel *FacadeStubs::artistAlbumModel(const QString &artistName)
{
}

AbstractModel *FacadeStubs::recentAlbumsModel()
{
    return nullptr;
}

AbstractModel *FacadeStubs::playlistContents(int plsID)
{
    return nullptr;
}

void FacadeStubs::seek(int pos)
{
}

void FacadeStubs::setMedia(const QString &path)
{
}

void FacadeStubs::addAlbum(const QString &album)
{
}

int FacadeStubs::bitrate()
{
    return  0;
}


void FacadeStubs::playPlaylist(const QString &title, int index)
{
}





void PlayerFacade::shuffle()
{
    if(playlist){
        playlist->shuffle();
    }

}

void PlayerFacade::shuffleOff()
{
    if(playlist){
        playlist->shuffle();
    }
}

void PlayerFacade::repeatModeOnce()
{
    if(playlist){
        playlist->repeatModeOnce();
    }

}

void PlayerFacade::repeatModeSeq()
{
    if(playlist){
        playlist->repeatModeSeq();
    }
}

void PlayerFacade::repeatModeLoop()
{
    if(playlist){
        playlist->repeatModeLoop();
    }
}

void PlayerFacade::repeatModeOneLoop()
{
    if(playlist){
        playlist->repeatModeOneLoop();
    }
}

QMediaPlayer::State FacadeStubs::playerState()
{
}

void FacadeStubs::shuffle()
{
}

void FacadeStubs::shuffleOff()
{
}

void FacadeStubs::repeatModeOnce()
{
}

void FacadeStubs::repeatModeSeq()
{
}

void FacadeStubs::repeatModeLoop()
{
}




void FacadeStubs::repeatModeOneLoop()
{
}


QString PlayerFacade::positionStr() const
{
    if(player)
        return player->positionString();
    return "";
}

QString FacadeStubs::positionStr() const
{
    return "";
}



void PlayerFacade::addCurrentTrackToFavorite()
{
    int trackID = playlist->currentTrackID();
    playlist->addToFavorite(trackID);
}

void PlayerFacade::removeCurrentTrackFromFavorite()
{
    playlist->removeFromFavorite(playlist->currentTrackID());
}

void PlayerFacade::addToFavorite(int trackID)
{

    playlist->addToFavorite(trackID);
}

void PlayerFacade::removeFromFavorite(int trackID)
{
    playlist->removeFromFavorite(trackID);
}

void FacadeStubs::addCurrentTrackToFavorite()
{
}

void FacadeStubs::removeCurrentTrackFromFavorite()
{
}

void FacadeStubs::addToFavorite(int trackID)
{
}

void FacadeStubs::removeFromFavorite(int trackID)
{
}


int PlayerFacade::currentTrackFavorite()
{
    return playlist->favorite();
}

int FacadeStubs::currentTrackFavorite()
{
    return 0;
}
