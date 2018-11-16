#include "player.h"
#include "tagreader.h"

/* TODO needs refcatoring : implement a state machine
 * to synchronize the player and the playlist and DBus
 */

// TODO : Remove connexion through signal for notification and dbus
// PlayerFacade should be at least the one to handle this.
Player::Player(QMediaPlayer *parent):QMediaPlayer(parent)
{
       HOUR = 3600000; // Hour in Milliseconds

}

Player::~Player()
{
    qDebug() << __PRETTY_FUNCTION__ << " quitting ...";
    playlist = nullptr;
}



void Player::updateCurrentMedia(QMediaPlayer::MediaStatus status)
{
    qDebug() << "media status changed to " << status;
    if((status == MediaStatus::NoMedia) ||(status == MediaStatus::EndOfMedia))
    {
        playlist->next();
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
  //Q_EMIT coverChanged();
}

QString Player::cover()const
{
  return _cover;
}

QString Player::positionString()const
{

        int d = static_cast<int>(position());
        QString result = "";
        QTime tmpTime (0,0);
        if(!currentMedia().isNull()){
            QTime t =  tmpTime.addMSecs(d);
            if(d < HOUR)
            {
                result =  t.toString("mm:ss");
            }
            else
            {
                result = t.toString("hh:mm:ss");
            }
        }
        return result;
}

QString Player::durationString()const
{
        int d = static_cast<int>(duration());
        QString result = "";
        QTime tmpTime (0,0);
        if(!currentMedia().isNull()){
            QTime t =  tmpTime.addMSecs(d);
            if(d < HOUR)
            {
                result =  t.toString("mm:ss");
            }
            else
            {
                result = t.toString("hh:mm:ss");
            }
        }
        return result;
}

void Player::setPlaylist(Playlist2 *_playlist)
{

    this->playlist = _playlist;
    if(_playlist != nullptr)
    {
        connect(this, &Player::mediaStatusChanged, this, &Player::updateCurrentMedia);
        connect(playlist, &Playlist2::currentIndexChanged,this, &Player::fetchFromPlaylist);
        connect(playlist, &Playlist2::nomedia, this, &Player::stop);
    }
}
