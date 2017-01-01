#ifndef DBUSMANAGER_H
#define DBUSMANAGER_H
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QObject>
#include <QtDBus>
#include "mpris2.h"
#include "mpris2_player.h"
#include "notify.h"


class DBusManager : public QObject
{
    Q_OBJECT
public:
    explicit DBusManager(QObject *parent = 0, QDBusConnection parent2 = QDBusConnection::sessionBus());
    ~DBusManager();

public Q_SLOTS:
    QString getConnectionName();
    void setNotification(QString &summary, QString &body, QString &icon);
    void nofity(QString &summary, QString &body, QString &icon);
    void notify();
    //void connectToBus();
    QString service()
    {
        return QString (serviceName);
    }
    void setServiceName(QString &name);
    void setServiceName2(QString &name);
    void setObjectPath1(QString &path1);
    void setObjectPath2(QString &path2);
    void setPlaybackStatus(QMediaPlayer::State status);
    void setLoopStatus(QMediaPlaylist::PlaybackMode status);
    void setMetaData(Track track);
Q_SIGNALS:
void notificationChanged();
void Seeked(qlonglong position);
void playClicked();
void nextClicked();
void previousClicked();
void pauseClicked();
void stopClicked();
void positionChanged(int );


protected:

MPRIS2 *mpris2_root;
MPRIS2_PLAYER *mpris2_player;
Notification * notification;
QDBusConnection bus;
QString serviceName;
QString serviceName2;
QString objectPath1;
QString objectPath2;
QString notify_summary;
QString notify_body;
QString notify_icon;

};

#endif // DBUSMANAGER_H
