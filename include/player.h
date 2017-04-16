#ifndef PLAYER_H
#define PLAYER_H

//#include <QMediaPlayer>
#include <QtMultimedia/QMediaPlayer>
#include <memory>
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

    //Q_PROPERTY(QString media READ media WRITE setMedia NOTIFY mediaChanged)

    public:
        Player(QMediaPlayer *parent = 0);
        ~Player();

        void setPlaylist(Playlist2* _playlist);
    public Q_SLOTS:
        void setMedia(QString path);
        void setCover(QString path);
        void fetchFromPlaylist();
        QString cover()const;
        QString positionString()const;
        QString durationString()const;


private Q_SLOTS:
        void updateCurrentMedia(MediaStatus status);

    private:
        Playlist2 *playlist;
        QString currentPath;
        QString _cover;
        int count;
        int  HOUR;
};

#endif // PLAYER_H
