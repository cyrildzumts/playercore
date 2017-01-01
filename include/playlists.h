#ifndef PLAYLISTS_H
#define PLAYLISTS_H

#include "randomgenerator.h"
#include "abstractmodel.h"
#include <QFile>
#include <QTextStream>
#include "common.h"
#include "tagreader.h"



class Playlist : public AbstractModel
{
    Q_OBJECT
public:
    Q_ENUMS(PlaybackMode)
    /*
    Q_PROPERTY(int duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(QString durationStr READ durationStr NOTIFY durationChanged)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(int playbackMode READ playbackMode WRITE setPlaybackMode NOTIFY playbackModeChanged)
    Q_PROPERTY(int mediaCount READ mediaCount)
    Q_PROPERTY(QString  media READ media)
    Q_PROPERTY(bool isEmpty READ isEmpty)
    Q_PROPERTY(QString cover READ cover WRITE setCover NOTIFY coverChanged)
    Q_PROPERTY(QString artist READ artist NOTIFY artistChanged)
    Q_PROPERTY(QString title READ title  NOTIFY titleChanged)
    Q_PROPERTY(QString album READ album NOTIFY albumChanged)
    Q_PROPERTY(int year READ year  NOTIFY yearChanged)
    Q_PROPERTY(QString  playlistTitle READ playlistTitle WRITE setPlaylistTitle NOTIFY playlistTitleChanged)

*/

public:

    explicit Playlist(AbstractDataAccessObject *data_access);
    Playlist(const QString name);
    Playlist(const Playlist &pls);
    Playlist &operator=(const Playlist & pls);
    virtual ~Playlist()
    {
        clear();
    }

    QString playlistTitle() const;
    void setPlaylistTitle(const QString &playlistTitle);

public Q_SLOTS:
    /**
     * @brief duration
     * @return the duration of the total Tracks.
     * If the playlist is empty, its return 0
     */
    virtual int duration()const;

    /**
     * @brief durationStr
     * @return return a string representation of the duration
     * of the total tracks. Returns an empty string if the
     * playlist is empty
     */
    virtual QString durationStr() const;
    /**
     * @brief setCover set the current Track's Cover path
     * @param path
     */
    virtual void setCover(QString path);
    /**
     * @brief cover This Method return the current Track's cover.
     * @return a path to the cover when there is an actual
     * track. When the playlist is empty it then returns an
     * empty string
     */
    virtual QString cover()const;
    /**
     * @brief setTracklist sets tracks as the new tracklist
     * @param tracks
     */
    virtual void setTracklist(std::vector<Track> tracks);

    virtual void setTracklist(const std::vector<int> &ids);

    /**
     * @brief album
     * @return Return the current Track's Album Title
     */
    virtual QString album()const;
    /**
     * @brief title
     * @return Returns the current Track's title
     */
    virtual QString title()const;
    /**
     * @brief artist
     * @return Returns the current Track's artist
     */
    virtual QString artist()const;
    /**
     * @brief year
     * @return Returns the current Track's year
     */
    virtual int year()const;

    /**
     * @brief tracklist
     * @return return the current tracklist.
     */
    virtual std::vector<Track> tracklist()const;

    /**
     * @brief currentIndex
     * @return returns the current Index.
     * if the playlist is empty, this methods returns -1
     */
    virtual int currentIndex()const;
    virtual int currentRandomIndex()const;
    /**
     * @brief setCurrentIndex Sets the current Index
     * @param index
     */
    virtual void setCurrentIndex(int index);
    /**
     * @brief playbackMode
     * @return The current PlaybackMode
     */
    virtual int playbackMode()const;
    /**
     * @brief setPlaybackMode Sets the Playlist in the
     * PlaybackMode mode
     * @param mode
     */
    virtual void setPlaybackMode(int mode);
    /**
     * @brief clear reset the playlist. After a call to this
     * methods the playlist is completely cleared. The playbackMode
     * is set to default : PlaybackMode::Sequential
     */
    virtual void clear();
    virtual void setTitle(QString t);
    //void addMedia(QString url, int trackID);
    /**
     * @brief addMedia adds track to the playlist.
     * @param track
     */
    virtual void addMedia(Track track);
    /**
     * @brief addMedia adds tracks to the playlist
     * @param tracks
     */
    virtual void addMedia(std::vector<Track> tracks);
    /**
     * @brief addMedia adds the tracks with the ID trackID
     * to the playlist. This methods queries the database
     * for the track with the ID trackID.
     * @param trackID
     */
    virtual void addMedia(int trackID);
    /**
     * @brief currentTrack
     * @return The current Track datastructure
     */
    virtual Track currentTrack()const;
    virtual bool isEmpty()const;
    /**
     * @brief insertMedia adds track to the position pos.
     * @param pos The position at which we want to insert
     * the track.
     * @param track
     * If the position pos is not empty, the playlist
     * contents will be right shifted from the Position
     * pos
     */
    virtual void insertMedia(int pos,Track track);
    virtual void insertMedia(int pos, std::vector<Track> tracks);
    virtual void insertMedia(int pos, int trackID);
    /**
     * @brief media
     * @param index
     * @return The path to the track at the position index
     * returns an empty string is there is no track at that
     * index
     */
    virtual QString media(int index)const;
    /**
     * @brief media
     * @return returns the current Track Path.
     */
    virtual QString media()const;
    /**
     * @brief mediaCount
     * @return returns how many tracks there are in the playlist
     */
    virtual int mediaCount()const;
    /**
     * @brief nextIndex
     * @param steps
     * @return the index of the next track with reference to the current
     * track. This method is meant to be used when the PlaybackMode is set
     * to PlaybackMode::Random
     */
    virtual int	nextIndex() const;
    /**
     * @brief previousIndex
     * @param steps
     * @return the index of the previous track with reference to the current
     * track. This method is meant to be used when the PlaybackMode is set
     * to PlaybackMode::Random
     */
    virtual int	previousIndex() const;
    /**
     * @brief removeMedia removes the track that is at the position
     * pos
     * @param pos
     * @return true when the track could be removed from the Playlist.
     */
    virtual bool removeMedia(int pos);
    /**
     * @brief removeMedia removes the tracks that is at the intervals
     * start and end, inclusive
     * @param pos
     * @return true when the tracks could be removed from the Playlist.
     */

    //virtual bool removeMedia(int start, int end);
    /**
     * @brief currentTrackID
     * @return returns the current Track ID
     * return -1 when the playlist is empty
     */
    virtual int currentTrackID()const;
    /**
     * @brief save the playlist to the location <location> with
     * the format <format>
     * @param location
     * @param format
     */
    virtual void save(const QUrl & location, const char * format = 0);
    virtual void save(QIODevice * device, const char * format);
    //PlayerControls:
    /**
     * @brief next avances the playlist to the next index
     */
    virtual void next();
    /**
     * @brief previous takes the playlist to the previous
     * track.
     */
    virtual void previous();
    /**
     * @brief shuffle set the PlaybackMode to Random
     */
    virtual void shuffle();

    // QAbstractlistModel Overwriting :
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual void resetInternalData();
    virtual QVariant data(const QModelIndex &index, int role) const;



Q_SIGNALS:
    void durationChanged();
    void coverChanged();
    void nomedia();
    void tracklistChanged();
    void titleChanged();
    void dataChanged();
    void playbackModeChanged();
    void currentIndexChanged();
    void albumChanged();
    void artistChanged();
    void yearChanged();
    void metadataChanged();
    void playlistTitleChanged();

protected:
    virtual QHash<int, QByteArray> roleNames()const;
    virtual void showContents();
    bool isIndexValid(int index)const;
protected Q_SLOTS:
    virtual void setCurrentMedia(int index);
    virtual void init();
    Track getTrack(int trackID);
    //virtual void emitAllMetadata();

protected:

    int _duration;
    /**
     * @brief _currentIndex is current index of the track being played
     */
    int _currentIndex;
    /**
     * @brief _randomCurrentIndex is used to access the random index from
     * randomIndices
     */
    int _randomCurrentIndex;

    QString _playlistTitle;
    /**
     * @brief _title is the Playlist's title
     */
    QString _title;
    /**
     * @brief _cover is the cover of the playlist
     */
    QString _cover;
    /**
     * @brief _playbackMode defines how the tracks contained in the playlist
     * are being played back.
     */
    int _playbackMode;

    //PlaybackEnum::PlaybackModeObject mode;
    /**
     * @brief _tracks represents the list of the tracks
     */
    std::vector<Track> _tracks;
    /**
     * @brief randomIndices contains the indices of the tracks when
     * the Random Playback is activated. _randomCurrentIndex is used
     * to access these indices.
     */
    std::vector<int> randomIndices;

    QString _currentMedia;
    AbstractDataAccessObject *data_access;
};


class PlaylistState;
//class RandomState;

class Playlist2 : public Playlist
{
public:
     explicit Playlist2(AbstractDataAccessObject *data_access);
    virtual void next();
    virtual void previous();
    virtual void setPlaybackMode(int mode);
    virtual int playbackMode()const;

private:
    void changeState(PlaylistState *state);


private:
    friend class PlaylistState;
    friend class CurrentItemInLoopState;
    friend class CurrentItemOnceState;
    friend class SequentialState;
    friend class LoopState;
    friend class RandomState;
    PlaylistState * state;

};


class PlaylistState
{

public :
    virtual ~PlaylistState(){}
    virtual void next(Playlist2 *pls) = 0;
    virtual void previous(Playlist2 *pls) = 0;
    virtual PlaybackMode playbackMode()const
    {
        return mode;
    }

protected:
    PlaybackMode mode;

};

class CurrentItemOnceState : public PlaylistState
{
private:
    CurrentItemOnceState();
    ~CurrentItemOnceState();

public :
    static CurrentItemOnceState *instance();


    // PlaylistState interface
public:
    virtual void next(Playlist2 *pls) override;
    virtual void previous(Playlist2 *pls) override;

private:
    static CurrentItemOnceState *_instance;
};

class CurrentItemInLoopState : public PlaylistState
{
private:
    CurrentItemInLoopState();

public:
    static CurrentItemInLoopState *instance();
    virtual ~CurrentItemInLoopState();

    // PlaylistState interface
public:
    virtual void next(Playlist2 *pls) override;
    virtual void previous(Playlist2 *pls) override;

private:
    static CurrentItemInLoopState* _instance;

};

class LoopState : public PlaylistState
{
private:
    LoopState();

public:
    static LoopState *instance();
    virtual ~LoopState();

    // PlaylistState interface
public:
    virtual void next(Playlist2 *pls) override;
    virtual void previous(Playlist2 *pls) override;


private:
    static LoopState *_instance;
};

class RandomState : public PlaylistState
{
private:
    RandomState();

public:
    static RandomState *instance();
    ~RandomState();
    // PlaylistState interface
public:
    virtual void next(Playlist2 *pls) override;
    virtual void previous(Playlist2 *pls) override;


private:
    static RandomState *_instance;


};

class SequentialState : public PlaylistState
{
private:
    SequentialState();

public :
    static SequentialState *instance();
    ~SequentialState();



    // PlaylistState interface
public:
    virtual void next(Playlist2 *pls) override;
    virtual void previous(Playlist2 *pls) override;


private:
    static SequentialState *_instance;
};

#endif // PLAYLISTS_H
