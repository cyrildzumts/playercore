#ifndef FACADE_H
#define FACADE_H

#include "abstractmodel.h"
#include "player.h"
#include "dbusmanager.h"
#include "playlistmanager.h"
#include "mediascanner.h"
#include "settingmanager.h"


class Facade : public QObject
{
    Q_OBJECT
public:
    virtual ~Facade(){}
    virtual void update() = 0;
    /**
     * @brief playAlbum Play the album starting from position <index>
     * @param album The album to be played
     * @param index the positon from where to start playing
     */
    virtual int currentTrackFavorite() = 0;
    virtual void playAlbum(const QString &album, int index = 0) = 0;
    virtual void playPlaylist(const QString &title, int index = 0) = 0;

    // Playlist Manager Interface
    /**
     * @brief createPlaylist create a playlist with
     * the title <<title>>. This method does nothing
     * when the title is already in use.
     * @param title The title of the new playlist.
     */
    virtual void createPlaylist(const QString& title) = 0;
    /**
     * @brief addToPlaylist Add the song with the ID
     * trackID into the Playlist whose title is pls.
     * @param pls The destination playlist
     * @param trackID The track's ID to be added.
     */
    virtual void addToPlaylist(const QString& pls, int trackID) = 0;

    /**
     * @brief removePlaylist Remove the Playlist pls
     * from the database. Does nothing if the
     * parameter pls is empty or doesn't to an
     * existing Playlist.
     * @param pls The Playlist to be removed.
     */
    virtual void removePlaylist(const QString& pls) = 0;
    /**
     * @brief removeFromPlaylist Remove the track
     * pointed by trackID from the Playlist pls.
     * @param pls
     * @param trackID
     */
    virtual void removeFromPlaylist(const QString& pls, int trackID) = 0;
    // Model Interface
    virtual AbstractModel* albumModel() = 0;
    virtual AbstractModel* genreModel() = 0;
    virtual AbstractModel* tracklistModel() = 0;
    virtual AbstractModel* playlistModel() = 0;
    virtual AbstractModel* currentPlaylist() = 0;
    virtual AbstractModel* albumContentModel(const QString &albumTitle) = 0;
    virtual AbstractModel* artistModel() = 0;
    virtual AbstractModel* albumByGenre(const QString& genreName) = 0;
    virtual AbstractModel* artistAlbumModel(const QString &artistName) = 0;
    virtual AbstractModel* recentAlbumsModel() = 0;
    virtual AbstractModel *playlistContents(int plsID) = 0;


    // Player Interface
    virtual void play() = 0;
    virtual void stop() = 0;
    virtual void pause() = 0;
    virtual void seek(int pos) = 0;
    virtual void setMedia(const QString& path) = 0;
    virtual int currentPosition() = 0;
    virtual QMediaPlayer::State playerState() = 0;

    // Playlist  Intercae : This controls the current Playlist
    /**
     * @brief addTrack Adds a track into the current Playlist
     * @param ID the ID(database primary key) of the track to tbe added.
     */
    virtual void addTrack(int ID) = 0;
    /**
     * @brief addAlbum Add an Album into the current Playlist
     * @param album The title of the album to be added.
     */
    virtual void addAlbum(const QString& album) = 0;
    /**
     * @brief removeTrack Remove a track from the current Playlist
     * @param pos the index of the track to be removed
     */
    virtual void removeTrack(int pos) = 0;
    /**
     * @brief next  Changes to the next track.
     */
    virtual void next() = 0;
    /**
     * @brief previous Changes the previous track
     */
    virtual void previous() = 0;
    /**
     * @brief setPlaybackMode Change the current playback mode
     * @param mode The new playback mode to be set
     */
    virtual void setPlaybackMode(int mode) = 0;

    virtual void shuffle() = 0;
    virtual void shuffleOff() = 0;
    virtual void repeatModeOnce() = 0;
    virtual void repeatModeOneLoop() = 0;
    virtual void repeatModeSeq() = 0;
    virtual void repeatModeLoop() = 0;
    virtual void addCurrentTrackToFavorite() = 0;
    virtual void removeCurrentTrackFromFavorite() = 0;
    virtual void addToFavorite(int trackID) = 0;
    virtual void removeFromFavorite(int trackID) = 0;
    /**
     * @brief playbackMode
     * @return The current playback mode
     */
    virtual int playbackMode()const = 0;
    /**
     * @brief currentIndex
     * @return The index of the current track being played
     */
    virtual int currentIndex()const = 0;
    /**
     * @brief setCurrentIndex Changes the current index of the current playlist.
     * This will advance the current index to the new index
     * @param index The new index of the track to be played
     */
    virtual void setCurrentIndex(int index) = 0;
    /**
     * @brief mediacount
     * @return The number of tracks contained into the current Playlist
     */
    virtual int mediacount()const = 0;
    /**
     * @brief duration
     * @return the duration of the total Tracks.
     * If the playlist is empty, it returns 0
     */
    virtual int duration()const = 0;

    /**
     * @brief durationStr
     * @return return a string representation of the duration
     * of the total tracks. Returns an empty string if the
     * playlist is empty
     */
    virtual QString durationStr() const = 0;


    /**
     * @brief length
     * @return the duration of the current track being played.
     * If the playlist is empty, it returns 0
     */
    virtual int length() const = 0;

    /**
     * @brief lengthStr
     * @return return a string representation of the length
     * of the current track. Returns an empty string if the
     * playlist is empty
     */
    virtual QString lengthStr() const = 0;

    virtual QString positionStr() const = 0;
    /**
     * @brief setCover set the current Track's Cover path
     * @param path
     */
    virtual void setCover(QString path) = 0;
    /**
     * @brief cover This Method return the current Track's cover.
     * @return a path to the cover when there is an actual
     * track. When the playlist is empty it then returns an
     * empty string
     */
    virtual QString cover()const = 0;

    /**
     * @brief album
     * @return Return the current Track's Album Title
     */
    virtual QString album()const = 0;
    /**
     * @brief title
     * @return Returns the current Track's title
     */
    virtual QString title()const = 0;
    /**
     * @brief artist
     * @return Returns the current Track's artist
     */
    virtual QString artist()const = 0;
    /**
     * @brief year
     * @return Returns the current Track's year
     */
    virtual int year()const = 0;
    virtual int bitrate() = 0;

    virtual QString author()const = 0;
    virtual QString appName()const = 0;
    virtual QString version() const = 0;



    // SIGNAL Emited from Components
Q_SIGNALS:
    void playerStateChanged();
    void positionChanged(int pos);
    void durationChanged();
    void lengthChanged(int len);

};


class PlayerFacade : public Facade
{
public:
    PlayerFacade(const QString &conf = QString());
    virtual ~PlayerFacade();
    void init();

    // Facade interface
public:
    virtual void update() override;
    virtual void playAlbum(const QString &album, int index = 0) override;

    // Model
    virtual AbstractModel *albumModel() override;
    virtual AbstractModel *tracklistModel() override;
    virtual AbstractModel *playlistModel() override;
    virtual AbstractModel *currentPlaylist() override;
    virtual AbstractModel *genreModel() override;

    // Player
    virtual void play() override;
    virtual void stop() override;
    virtual void pause() override;
    virtual int currentPosition() override;

    // Playlist
    virtual void addTrack(int ID) override;
    virtual void removeTrack(int pos) override;
    virtual void next() override;
    virtual void previous() override;
    virtual void setPlaybackMode(int mode) override;
    virtual int playbackMode() const override;
    virtual int currentIndex() const override;
    virtual void setCurrentIndex(int index) override;
    virtual int duration() const override;
    virtual QString durationStr() const override;
    virtual void setCover(QString path) override;
    virtual QString cover() const override;
    virtual QString album() const override;
    virtual QString title() const override;
    virtual QString artist() const override;
    virtual int year() const override;
    virtual int mediacount() const override;
    virtual int length() const override;
    virtual QString lengthStr() const override;
    // Facade interface
public:
    virtual QString author() const override;
    virtual QString appName() const override;
    virtual QString version() const override;


    // Facade interface
public:

    virtual void seek(int pos) override;
    virtual void setMedia(const QString &path) override;
    virtual void addAlbum(const QString &album) override;
    virtual int bitrate() override;



    //  Utilies :
    void sendNotify(const QString &summary, const QString &body, const QString &icon );

private:
    AbstractDataAccessObject *data_access_object;
    /**
     * @brief playlist the current playlist
     */
    Playlist2 *playlist;
    Player *player;
    AbstractModel *albums;
    AbstractModel *artistAlbums;
    AbstractModel *artists;
    AbstractModel *search_result;
    AbstractModel *filtered_results;
    AbstractModel *albumContent;
    AbstractModel *genres;
    AbstractModel *tracklists;
    AbstractModel *plsContents;
    AbstractModel *playlists;
    IMediaScanner *mediascanner;
    ISettingManager *settings;
    IPlaylistManager *playlistManager;
    DBusManager *Busmanager;



    // Facade interface
public:
    virtual AbstractModel *albumContentModel(const QString &albumTitle) override;
    virtual AbstractModel *artistModel() override;
    virtual AbstractModel *albumByGenre(const QString &genreName) override;
    virtual AbstractModel *artistAlbumModel(const QString &artistName) override;
    virtual AbstractModel *recentAlbumsModel() override;
    virtual AbstractModel *playlistContents(int plsID) override;

    // Facade interface
public:
    virtual void playPlaylist(const QString &title, int index) override;

    // Facade interface
public:
    virtual QMediaPlayer::State playerState() override;

    // Facade interface
public:
    virtual void shuffle() override;
    virtual void shuffleOff() override;
    virtual void repeatModeOnce() override;
    virtual void repeatModeSeq() override;
    virtual void repeatModeLoop() override;
    virtual void repeatModeOneLoop() override;

    // Facade interface
public:
    virtual QString positionStr() const override;

    // Facade interface
public:
    virtual void createPlaylist(const QString &title) override;
    virtual void addToPlaylist(const QString &pls, int trackID) override;
    virtual void removePlaylist(const QString &pls) override;
    virtual void removeFromPlaylist(const QString &pls, int trackID) override;
    virtual void addCurrentTrackToFavorite() override;
    virtual void removeCurrentTrackFromFavorite() override;
    virtual void addToFavorite(int trackID) override;
    virtual void removeFromFavorite(int trackID) override;
    virtual int currentTrackFavorite() override;
};

class FacadeStubs : public Facade
{

    // Facade interface
public:
    virtual void update() override;
    virtual AbstractModel *albumModel() override;
    virtual AbstractModel *tracklistModel() override;
    virtual AbstractModel *playlistModel() override;
    virtual AbstractModel *currentPlaylist() override;

    virtual void play() override;
    virtual void stop() override;
    virtual void pause() override;
    virtual int currentPosition() override;
    virtual void addTrack(int ID) override;
    virtual void removeTrack(int ops) override;
    virtual void next() override;
    virtual void previous() override;
    virtual void setPlaybackMode(int mode) override;
    virtual int playbackMode() const override;
    virtual int currentIndex() const override;
    virtual int duration() const override;
    virtual QString durationStr() const override;
    virtual void setCover(QString path) override;
    virtual QString cover() const override;
    virtual QString album() const override;
    virtual QString title() const override;
    virtual QString artist() const override;
    virtual int year() const override;
    virtual int mediacount()const override;

    // Facade interface
public:
    virtual int length() const override;
    virtual QString lengthStr() const override;

    // Facade interface
public:
    virtual void playAlbum(const QString &album, int index) override;
    virtual void setCurrentIndex(int index) override;
    virtual QString author() const override;
    virtual QString appName() const override;
    virtual QString version() const override;

    // Facade interface
public:
    virtual void createPlaylist(const QString &title) override;
    virtual void addToPlaylist(const QString &pls, int trackID) override;
    virtual void removePlaylist(const QString &pls) override;
    virtual void removeFromPlaylist(const QString &pls, int trackID) override;
    virtual AbstractModel *genreModel() override;
    virtual AbstractModel *albumContentModel(const QString &albumTitle) override;
    virtual AbstractModel *artistModel() override;
    virtual AbstractModel *albumByGenre(const QString &genreName) override;
    virtual AbstractModel *artistAlbumModel(const QString &artistName) override;
    virtual AbstractModel *recentAlbumsModel() override;
    virtual AbstractModel *playlistContents(int plsID) override;
    virtual void seek(int pos) override;
    virtual void setMedia(const QString &path) override;
    virtual void addAlbum(const QString &album) override;
    virtual int bitrate() override;

    // Facade interface
public:
    virtual void playPlaylist(const QString &title, int index) override;

    // Facade interface
public:
    virtual QMediaPlayer::State playerState() override;
    virtual void shuffle() override;
    virtual void shuffleOff() override;
    virtual void repeatModeOnce() override;
    virtual void repeatModeSeq() override;
    virtual void repeatModeLoop() override;
    virtual void repeatModeOneLoop() override;

    // Facade interface
public:
    virtual QString positionStr() const override;


    // Facade interface
public:
    virtual void addCurrentTrackToFavorite() override;
    virtual void removeCurrentTrackFromFavorite() override;
    virtual void addToFavorite(int trackID) override;
    virtual void removeFromFavorite(int trackID) override;

    // Facade interface
public:
    virtual int currentTrackFavorite() override;
};

#endif // FACADE_H
