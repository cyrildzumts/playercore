#include "dbusmanager.h"

DBusManager::DBusManager(QObject *parent,QDBusConnection parent2) :
    QObject(parent), bus(parent2)
{


    notify_body ="";
    notify_summary = "";
    notify_icon = "";
    serviceName = QString("org.mpris.MediaPlayer2.player");
    serviceName2 = QString("org.mpris.player");
    objectPath1 = QString("/org/mpris/MediaPlayer2");
    objectPath2 = QString("/org/mpris/MediaPlayer2/Player");
    mpris2_root = new MPRIS2();
    mpris2_player = new MPRIS2_PLAYER();
    notification = new Notification();
    bus = QDBusConnection::sessionBus();
    bus.registerObject(objectPath1, mpris2_root);
    bus.registerObject(objectPath2, mpris2_player);
    bus.registerService(serviceName);
    bus.registerService(serviceName2);
    if(bus.isConnected())
    {
        qDebug()<< "We are connected in the session Bus to the service : " << bus.name();
        qDebug()<< "We are connected in the session Bus to the basename : " << bus.baseService();
    }

    //connect(this,SIGNAL(notificationChanged()),this, SLOT(notify()));
    connect(this,&DBusManager::notificationChanged,this,&DBusManager::notify);
    //connect(mpris2_player,SIGNAL(nextClicked()),this,SIGNAL(nextClicked()));
    connect(mpris2_player,&MPRIS2_PLAYER::nextClicked,this,&DBusManager::nextClicked);
    //connect(mpris2_player,SIGNAL(pausedClicked()),this,SIGNAL(pauseClicked()));
    connect(mpris2_player,&MPRIS2_PLAYER::pausedClicked,this,&DBusManager::pauseClicked);
    //connect(mpris2_player,SIGNAL(playClicked()),this,SIGNAL(playClicked()));
    connect(mpris2_player,&MPRIS2_PLAYER::playClicked,this,&DBusManager::playClicked);
    //connect(mpris2_player,&MPRIS2_PLAYER::positionChanged,this,&DBusManager::positionChanged);
    connect(mpris2_player,&MPRIS2_PLAYER::positionChanged,this,&DBusManager::positionChanged);
    //connect(mpris2_player,SIGNAL(PreviousClicked()),this, SIGNAL(previousClicked()));
    connect(mpris2_player,&MPRIS2_PLAYER::PreviousClicked,this,&DBusManager::previousClicked);
    //connect(mpris2_player, SIGNAL(Seeked(qlonglong)),this,SIGNAL(Seeked(qlonglong)));
    connect(mpris2_player,&MPRIS2_PLAYER::Seeked,this,&DBusManager::Seeked);
    //connect(mpris2_player, SIGNAL(stopClicked()), this, SIGNAL(stopClicked()));
    connect(mpris2_player,&MPRIS2_PLAYER::stopClicked,this,&DBusManager::stopClicked);
}

DBusManager::~DBusManager()
{
    qDebug() << "DBusmanager destroyed: the Interface is no more available";
    bus.unregisterObject(objectPath1);
    bus.unregisterObject(objectPath2);
    bus.unregisterService(serviceName);
    bus.unregisterService(serviceName2);
    delete mpris2_player;
    delete mpris2_root;
    delete notification;
}

QString DBusManager::getConnectionName()
{
    return  bus.name();
}

void DBusManager::setNotification(const QString &summary, const QString &body, const QString &icon)
{
    notify_body = body;
    notify_icon = icon;
    notify_summary = summary;
    notification->setNotification(notify_summary,notify_body,notify_icon);
    Q_EMIT notificationChanged();
}

void DBusManager::notify()
{
    notification->sendNotify();
}

void DBusManager::nofity(const QString &summary, const QString &body, const QString &icon)
{
    notification->setNotification(summary,body,icon);
    notification->sendNotify();
}

void DBusManager::setObjectPath1(QString &path1)
{
    objectPath1 = path1;
}


void DBusManager::setObjectPath2(QString &path2)
{
    objectPath2 = path2;
}

void DBusManager::setServiceName(QString &name)
{
    serviceName = name;
}

void DBusManager::setServiceName2(QString &name)
{
    serviceName2 = name;
}

void DBusManager::setLoopStatus(QMediaPlaylist::PlaybackMode status)
{
    if( status == QMediaPlaylist::Sequential)
            mpris2_player->SetLoopStatus("None");
    else if( status == QMediaPlaylist::CurrentItemInLoop)
        mpris2_player->SetLoopStatus("Track");
    else if( status == QMediaPlaylist::Loop)
        mpris2_player->SetLoopStatus("Playlist");
}

void DBusManager::setPlaybackStatus(QMediaPlayer::State status)
{
    QString state;
    if(status == QMediaPlayer::PlayingState)
    {
        state = "Playing";
        mpris2_player->setPlaybackStatus(state);
    }
    else if(status == QMediaPlayer::PausedState)
    {
        state = "Paused";
        mpris2_player->setPlaybackStatus(state);
    }
   else if(status == QMediaPlayer::StoppedState)
    {
        state = "Stopped";
        mpris2_player->setPlaybackStatus(state);
    }
}

void DBusManager::setMetaData(Track track)
{

    mpris2_player->setMetaData(track);
}
