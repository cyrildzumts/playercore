#ifndef PLAYER_H
#define PLAYER_H

//#include <QMediaPlayer>
#include <QtMultimedia>
#include <memory>
#include "dbusmanager.h"
#include "playlists.h"

//struct PlayerStateBackup
//{
//    int index;
//    int position;
//    int volume;
//    QStringList list;
//    int  playBackMode;
//    bool ShuffleMode;

//};


class Player : public QMediaPlayer
{
    Q_OBJECT
    Q_PROPERTY(QString cover READ cover WRITE setCover NOTIFY coverChanged)
    Q_PROPERTY(QString positionString READ positionString )
    Q_PROPERTY(QString durationString READ durationString )
    //Q_PROPERTY(QString media READ media WRITE setMedia NOTIFY mediaChanged)

    public:
        Player(QMediaPlayer *parent = 0);

        void setPlaylist(Playlist2* _playlist);
    public Q_SLOTS:
        void notify(bool isReady);
        void setMedia(QString path);
        void setCover(QString path);
        void fetchFromPlaylist();
        QString cover()const;
        QString positionString()const;
        QString durationString()const;

Q_SIGNALS:
        void coverChanged();
        void dbusNextClicked();
        void dbusPreviousClicked();
        void dbusPlayClicked();

private Q_SLOTS:
        void updateCurrentMedia(MediaStatus status);

    private:
        Playlist2 *playlist;
        DBusManager Busmanager;
        QString currentPath;
        QString _cover;
        int count;
        int  HOUR;
};

#endif // PLAYER_H
