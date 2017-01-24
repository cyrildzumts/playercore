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

    virtual void playAlbum(const QString &album, int index = 0) = 0;

    // Playlist Manager Interface
    virtual void createPlaylist(const QString& title) = 0;
    virtual void addToPlaylist(const QString& pls, int trackID) = 0;
    virtual void removePlaylist(const QString& pls) = 0;
    virtual void removeFromPlaylist(const QString& pls, int trackID) = 0;
    // Model Interface
    virtual AlbumModel* albumModel() = 0;
    virtual GenreModel* genreModel() = 0;
    virtual AbstractModel* tracklistModel() = 0;
    virtual AbstractModel* playlistModel() = 0;
    virtual AbstractModel* currentPlaylist() = 0;

    // Player Interface
    virtual void play() = 0;
    virtual void stop() = 0;
    virtual void pause() = 0;
    virtual void seek(int pos) = 0;
    virtual void setMedia(const QString& path) = 0;
    virtual int currentPosition() = 0;

    // Playlist  Intercae
    virtual void addTrack(int ID) = 0;
    virtual void addAlbum(const QString& album) = 0;
    virtual void removeTrack(int pos) = 0;
    virtual void next() = 0;
    virtual void previous() = 0;
    virtual void setPlaybackMode(int mode) = 0;
    virtual int playbackMode()const = 0;
    virtual int currentIndex()const = 0;
    virtual void setCurrentIndex(int index) = 0;
    virtual int mediacount()const = 0;
    /**
     * @brief duration
     * @return the duration of the total Tracks.
     * If the playlist is empty, its return 0
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
     * @return the duration of the total Tracks.
     * If the playlist is empty, its return 0
     */
    virtual int length()const = 0;

    /**
     * @brief lengthStr
     * @return return a string representation of the length
     * of the current track. Returns an empty string if the
     * playlist is empty
     */
    virtual QString lengthStr() const = 0;
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
    virtual AlbumModel *albumModel() override;
    virtual AbstractModel *tracklistModel() override;
    virtual AbstractModel *playlistModel() override;
    virtual AbstractModel *currentPlaylist() override;
    virtual GenreModel *genreModel() override;

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
    virtual void createPlaylist(const QString &title) override;
    virtual void addToPlaylist(const QString &pls, int trackID) override;
    virtual void removePlaylist(const QString &pls) override;
    virtual void removeFromPlaylist(const QString &pls, int trackID) override;
    virtual void seek(int pos) override;
    virtual void setMedia(const QString &path) override;
    virtual void addAlbum(const QString &album) override;
    virtual int bitrate() override;



    //  Utilies :
    void sendNotify(const QString &summary, const QString &body, const QString &icon );

private:
    AbstractDataAccessObject * data_access_object;
    Playlist2 *playlist;
    Player *player;
    AlbumModel *albums;
    GenreModel *genres;
    AbstractModel *tracklists;
    AbstractModel *playlists;
    IMediaScanner *mediascanner;
    ISettingManager *settings;
    IPlaylistManager *playlistManager;
    DBusManager *Busmanager;


};

class FacadeStubs : public Facade
{

    // Facade interface
public:
    virtual void update() override;
    virtual AlbumModel *albumModel() override;
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
};

#endif // FACADE_H
