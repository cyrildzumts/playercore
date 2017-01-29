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
class GUIManager : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(bool isPlaying READ isPlaying)
    Q_PROPERTY(bool isPaused READ isPaused)
    Q_PROPERTY(int position READ position WRITE setPosition NOTIFY positionChanged)
    //Q_PROPERTY(QString positionStr READ positionStr WRITE setPositionStr NOTIFY positionStrChanged)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(QString album READ album )
    Q_PROPERTY(QString genre READ genre )
    Q_PROPERTY(QString artist READ artist)
    Q_PROPERTY(QString cover READ cover)
    Q_PROPERTY(QString duration READ durationStr)
    Q_PROPERTY(QString length READ lengthStr)
    Q_PROPERTY(int playbackMode READ playbackMode WRITE setPlaybackMode NOTIFY playbackModeChanged)
    Q_PROPERTY(int mediaCount READ mediaCount )

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
    AbstractModel *playlistContent(const QString &pls);
    Playlist *now_playingPlaylist();


    // tracklist model object
    virtual void onAlbumClicked(const QString &title);
    virtual void onPlayAlbumPressed(const QString &title);

    // Playlist In
    virtual void setCurrentIndex(int index);
    virtual QString title();
    virtual QString album();
    virtual QString artist();
    virtual QString cover();
    virtual QString genre();
    virtual int duration();
    virtual QString durationStr();
    virtual int length();
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



Q_SIGNALS:
    void currentIndexChanged(int index);
    void playbackModeChanged(int mode);
    void positionChanged(int position);

public:

private:
    Facade *facade;
};

#endif // GUIMANAGER_H
