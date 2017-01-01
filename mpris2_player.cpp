
#include "mpris2_player.h"
#include "mpris2_playerAdaptor.h"

MPRIS2_PLAYER::MPRIS2_PLAYER(QObject *parent) : QObject(parent)
{
    AppName = "Player";
    freeDesktopObjectPath = "org.freedesktop.DBus.Properties";
    objectPath = "org/mpris/MediaPlayer2";
    serviceName = "org.mpris.MediaPlayer2.player";
    interfaceName = "org.mpris.MediaPlayer2.Player";


    new PlayerAdaptor (this);
     connect(this, SIGNAL(playClicked()),this, SLOT(signalReceived()));
     connect(this, SIGNAL(PreviousClicked()),this, SLOT(signalReceived()));
     connect(this, SIGNAL(nextClicked()),this, SLOT(signalReceived()));
     connect(this, SIGNAL(stopClicked()),this, SLOT(signalReceived()));
     connect(this, SIGNAL(pausedClicked()),this, SLOT(signalReceived()));
     //connect(this, SIGNAL(positionChanged(int)),this, SLOT(signalReceived()));
}


QString MPRIS2_PLAYER::ApplicationName()const
{
    return AppName;
}

void MPRIS2_PLAYER::setApplicationName(QString name)
{
    AppName = QString(name);
}


QString MPRIS2_PLAYER::PlaybackStatus()const
{
    // have to find a way to return the player State
    return playBackStatus;
}

QString MPRIS2_PLAYER::LoopStatus()const
{
    // have to find a way to return the playlist loop state
    return loopStatus;
}

void MPRIS2_PLAYER::SetLoopStatus(const QString status)
{
    //Q_EMIT RepeatModeChanged();
    loopStatus = status;

    QDBusMessage msg = QDBusMessage::createSignal(
                                objectPath, freeDesktopObjectPath,
                                "PropertiesChanged"
                                                   );

     QVariant val = status;
     QVariantMap map;
     map.insert(QString("LoopStatus"),val);
      QVariantList args = QVariantList()
                          << interfaceName
                          << map
                          << QStringList();
      msg.setArguments(args);
      QDBusConnection::sessionBus().send(msg);

}


double MPRIS2_PLAYER::Rate()const
{
    return 1.0;
}

void MPRIS2_PLAYER::SetRate(const double rate)
{
    Q_UNUSED(rate)
}

bool MPRIS2_PLAYER::Shuffle()const
{
    return false;
}

void MPRIS2_PLAYER::SetShuffle(bool shuffle)
{
    //Q_EMIT ShuffleModeChanged();
    Q_UNUSED(shuffle)
}

bool MPRIS2_PLAYER::CanSeek()const
{
    return true;
}

bool MPRIS2_PLAYER::CanGoNext()const
{
    return true;
}

bool MPRIS2_PLAYER::CanGoPrevious()const
{
    return true;
}

bool MPRIS2_PLAYER::CanPlay()const
{
    return true;
}

bool MPRIS2_PLAYER::CanPause()const
{
    return true;
}

bool MPRIS2_PLAYER::CanControl()const
{
    return true;
}


// MEDIAPLAYER2.Player Methodes
double MPRIS2_PLAYER::MaximumRate()const
{
    return 1.0;
}

double MPRIS2_PLAYER::MinimumRate()const
{
    return 1.0;
}

void MPRIS2_PLAYER::Next()
{
    Q_EMIT nextClicked();
}

void MPRIS2_PLAYER::Previous()
{
    Q_EMIT PreviousClicked();
}

void MPRIS2_PLAYER::Pause()
{
    Q_EMIT pausedClicked();
}

void MPRIS2_PLAYER::Play()
{
    Q_EMIT playClicked();
}

void MPRIS2_PLAYER::Stop()
{
    Q_EMIT stopClicked();
}


void MPRIS2_PLAYER::PlayPause()
{
    Q_EMIT playClicked();
}

void MPRIS2_PLAYER::Seek(qlonglong offset)
{
    Q_EMIT Seeked(offset);
    //Q_UNUSED(offset)
}

void MPRIS2_PLAYER::SetPosition(const QDBusObjectPath &trackId, qlonglong offset)
{
    Q_UNUSED(trackId)
    Q_UNUSED (offset)
}

void MPRIS2_PLAYER::OpenUri(const QString &uri)
{
    Q_UNUSED(uri)
}


void MPRIS2_PLAYER::signalReceived()const
{
    qDebug() << " MPRIS2_PLAYER : Signal received from DBUS" ;
}

QVariantMap MPRIS2_PLAYER::Metadata()const
{
    return last_metadata_;
}

void MPRIS2_PLAYER::setMetaData(Track &track)

{

    last_metadata_ ["xesam:album"] = track.albumTitle;
    last_metadata_["xesam:title"] = track.title;
    last_metadata_["xesam:artist"] = track.artist;
    last_metadata_["xesam:genre"] = track.genre;
    last_metadata_["mpris:artUrl"] = "file://" + Tagreader::cover(track.path);
    last_metadata_ ["xesam:url"] = "file://" + track.path;
    QDBusMessage msg = QDBusMessage::createSignal(
            objectPath, freeDesktopObjectPath, "PropertiesChanged");


      QVariantList args = QVariantList()
                          << interfaceName
                          << last_metadata_
                          << QStringList();
      msg.setArguments(args);
      QDBusConnection::sessionBus().send(msg);

}

void MPRIS2_PLAYER::setPlaybackStatus(QString &status)
{
    playBackStatus = QString(status);
    QDBusMessage msg = QDBusMessage::createSignal(
                                objectPath, freeDesktopObjectPath,
                                "PropertiesChanged"
                                                   );
      QVariant val = status;
      QVariantMap map ;
      map.insert(QString("PlaybackStatus"),val);
      QVariantList args = QVariantList()
                          << interfaceName
                          << map
                          << QStringList();
      msg.setArguments(args);
      QDBusConnection::sessionBus().send(msg);
}
