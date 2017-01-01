#include "player.h"
#include "tagreader.h"

/* TODO needs refcatoring : implement a state machine
 * to synchronize the player and the playlist and DBus
 */
Player::Player(QMediaPlayer *parent):QMediaPlayer(parent)
{
       HOUR = 3600000; // Hour in Milliseconds
      connect(this,&Player::metaDataAvailableChanged,this, &Player::notify);
      connect(&Busmanager,SIGNAL(nextClicked()),this,SIGNAL(dbusNextClicked()));
      connect(&Busmanager,SIGNAL(previousClicked()),this,SIGNAL(dbusPreviousClicked()));
      connect(&Busmanager,SIGNAL(playClicked()),this,SIGNAL(dbusPlayClicked()));
      connect(this, &Player::mediaStatusChanged, this, &Player::updateCurrentMedia);
      connect(playlist, &Playlist2::currentIndexChanged,this, &Player::fetchFromPlaylist);
      connect(playlist, &Playlist2::nomedia, this, &Player::stop);

}



void Player::updateCurrentMedia(QMediaPlayer::MediaStatus status)
{
    if(status == (MediaStatus::NoMedia || MediaStatus::EndOfMedia))
    {
        playlist->next();
    }
}

void Player::notify(bool isReady)
{
        if( isReady)
        {
            QString body,summary, icon;
            summary = metaData(QString("Title")).toString();
            body = QString("From ") + metaData(QString("AlbumTitle")).toString()  + " , " + QString("By ")
                    +  metaData(QString("AlbumArtist")).toString();
            icon = _cover ;
            Busmanager.setNotification(summary,body,icon);

            qDebug() << " Notify():: " << summary + " - " + body + " - " + icon;
        }


}

void Player::fetchFromPlaylist()
{
    setMedia(playlist->media());
}

void Player::setMedia(QString path)
{
    if(!path.isEmpty())
    {
        bool wasPlaying = state() == QMediaPlayer::PlayingState ? true : false ;
        QMediaPlayer::setMedia(QUrl::fromLocalFile(path));
        if(wasPlaying)
            play();
    }
}

void Player::setCover(QString path)
{
  _cover = path;
  Q_EMIT coverChanged();
}

QString Player::cover()const
{
  return _cover;
}

QString Player::positionString()const
{
        int d = position();
        QTime tmpTime (0,0);
        QTime t =  tmpTime.addMSecs(d);
        if(d < HOUR)
        {
           return  t.toString("mm:ss");
        }
        else
        {
            return t.toString("hh:mm:ss");
        }
}

QString Player::durationString()const
{
        int d = duration();
        QTime tmpTime (0,0);
        QTime t =  tmpTime.addMSecs(d);
        if(d < HOUR)
        {
            return  t.toString("mm:ss");
        }
        else
        {
            return t.toString("hh:mm:ss");
        }
}

void Player::setPlaylist(Playlist2 *_playlist)
{
    this->playlist = _playlist;
}
