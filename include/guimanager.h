#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <QtQuick/QQuickItem>
#include "facade.h"

/**
 * @brief The GUIManager class
 * This class is a bridge between
 * QML UI and C++ Core Logic.
 * it responds to every signal and user request
 * from the UI
 */

#define  HOUR_IN_MILLISECONDS  ((uint)(3600000)) // Hour in Milliseconds
#define  HOUR_IN_SECONDS  ((uint)(3600)) // Hour in Seconds
class GUIManager : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY isPlayingChanged)
    Q_PROPERTY(bool isPaused READ isPaused)
    Q_PROPERTY(int position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(int duration READ duration  NOTIFY durationChanged)
   Q_PROPERTY(QString positionStr READ positionStr NOTIFY positionStrChanged)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QString album READ album NOTIFY albumChanged )
    Q_PROPERTY(QString genre READ genre NOTIFY genreChanged)
    Q_PROPERTY(QString artist READ artist NOTIFY artistChanged)
    Q_PROPERTY(QString cover READ cover NOTIFY coverChanged)
    Q_PROPERTY(QString durationStr READ durationStr NOTIFY durationStrChanged)
    Q_PROPERTY(QString lengthStr READ lengthStr NOTIFY lengthStrChanged)
    Q_PROPERTY(qint64 length READ length NOTIFY lengthChanged)
    Q_PROPERTY(int playbackMode READ playbackMode WRITE setPlaybackMode NOTIFY playbackModeChanged)
    Q_PROPERTY(int mediaCount READ mediaCount )
    Q_PROPERTY(int favorite READ favorite )


public:
    GUIManager();
    virtual ~GUIManager();

public Q_SLOTS:
    // Player Interface
    virtual void play();
    virtual void next();
    virtual void previous();
    virtual int position();
    virtual void setPosition(int position);
    virtual QString positionStr();
    virtual bool isPlaying();
    virtual bool isStopped();
    virtual bool isPaused();

    // Model Object Factories:
    AbstractModel *tracklistModel();
    AbstractModel *albumModel();
    AbstractModel *genreModel();
    AbstractModel *albumByGenre(const QString& genreName);
    AbstractModel *albumContents(const QString &tileAlbum);
    AbstractModel *artistsModel();
    AbstractModel *artistAlbumsModel(const QString &artistName);
    AbstractModel *recentAlbum();
    AbstractModel *playlistModel();
    AbstractModel *playlistContent(int plsID);
    Playlist *now_playingPlaylist();


    // tracklist model object
    virtual void onAlbumClicked(const QString &title);
    virtual void onPlayAlbumPressed(const QString &title);
    virtual void playAlbum(const QString& title, int index);
    virtual void playPlaylist(const QString& title, int index);

    // Playlist In
    /**
     * @brief durationToString convert a duration (time in milliseconds)
     * into a string representation.
     * @param d The time duration in millisecond
     * @return the duration in string format.
     */
    virtual int favorite();
    virtual QString durationToString(int d);
    virtual void setCurrentIndex(int index);
    virtual QString title();
    virtual QString album();
    virtual QString artist();
    virtual QString cover();
    virtual QString genre();
    virtual int duration();
    virtual QString durationStr();
    virtual qint64 length();
    virtual QString lengthStr();
    virtual int currentIndex();
    virtual int playbackMode();
    virtual void setPlaybackMode(int mode);
    virtual int mediaCount();
    virtual void addTrack(int id);
    virtual void removeTrack(int pos);


    //  Mediascanner Interface :
    void update();
    // Settings Controller interface
    virtual void shutdown();
    virtual QString author()const;
    virtual QString appName()const;
    virtual QString version()const;

    //PlaylistManager Interface
    virtual bool createPlaylist(const QString& pls);
    virtual bool removePlaylist(const QString& pls);
    virtual bool removeFromPlaylist(const QString& pls, int trackID);
    virtual bool addToPlaylist(const QString& pls, int trackID);


    virtual void addCurrentTrackToFavorite();
    virtual void removeCurrentTrackFromFavorite();
    virtual void addToFavorite(int trackID);
    virtual void removeFromFavorite(int trackID);
    virtual void shuffle() ;
    virtual void repeatModeOnce() ;
    virtual void repeatModeSeq() ;
    virtual void repeatModeLoop() ;
    virtual void repeatModeOneLoop();


Q_SIGNALS:
    void coverChanged();
    void titleChanged();
    void albumChanged();
    void artistChanged();
    void genreChanged();
    void currentIndexChanged(int index);
    void playbackModeChanged(int mode);
    void positionChanged(int position);
    void positionStrChanged(int position);
    void durationChanged();
    void durationStrChanged();
    void lengthChanged(int len);
    void lengthStrChanged();
    void isPlayingChanged();

public:

private:
    Facade *facade;
};

#endif // GUIMANAGER_H
