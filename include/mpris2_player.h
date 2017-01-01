#ifndef MPRIS2_PLAYER_H
#define MPRIS2_PLAYER_H

//#include <QDBusAbstractAdaptor>
#include "track.h"
#include "tagreader.h"
#include <QtDBus>

class MPRIS2_PLAYER: public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2.Player")
    Q_PROPERTY(QString ApplicationName READ ApplicationName WRITE setApplicationName)
    Q_PROPERTY( QString PlaybackStatus READ PlaybackStatus )
    Q_PROPERTY( QString LoopStatus READ LoopStatus WRITE SetLoopStatus )
    Q_PROPERTY( double Rate READ Rate WRITE SetRate )
    Q_PROPERTY( bool Shuffle READ Shuffle WRITE SetShuffle )
    Q_PROPERTY( QVariantMap Metadata READ Metadata )
    //Q_PROPERTY( double Volume READ Volume WRITE SetVolume )
    //Q_PROPERTY( qlonglong Position READ Position )
    Q_PROPERTY( double MinimumRate READ MinimumRate )
    Q_PROPERTY( double MaximumRate READ MaximumRate )
    Q_PROPERTY( bool CanGoNext READ CanGoNext )
    Q_PROPERTY( bool CanGoPrevious READ CanGoPrevious )
    Q_PROPERTY( bool CanPlay READ CanPlay )
    Q_PROPERTY( bool CanPause READ CanPause )
    Q_PROPERTY( bool CanSeek READ CanSeek )
    Q_PROPERTY( bool CanControl READ CanControl )



public:
    MPRIS2_PLAYER(QObject* parent=0);

public Q_SLOTS:
    // APP's Name getter and setter
    QString ApplicationName()const;
    void setApplicationName(QString name);
    void signalReceived()const;

    // MPRISV2 MediaPlayer2.Player Interface
    QString PlaybackStatus()const;
    void setPlaybackStatus(QString &status);
    QVariantMap Metadata()const;
    void setMetaData(Track &track);

    QString LoopStatus()const;
    Q_NOREPLY void SetLoopStatus(const QString status );
    double Rate()const;
    Q_NOREPLY void SetRate (const double rate);
    double MaximumRate() const;
    double MinimumRate() const;
    bool Shuffle ()const;

    Q_NOREPLY void SetShuffle (bool shuffle);
    bool CanGoNext ()const;
    bool CanGoPrevious ()const;
    bool CanPlay()const;
    bool CanPause ()const;
    bool CanControl ()const;
    bool CanSeek()const;

    // Methode
    // Methods : Need to be callable by QML to control the player.
    Q_NOREPLY void Next();
    Q_NOREPLY void Previous();
    Q_NOREPLY void Pause();
    Q_NOREPLY void PlayPause();
    Q_NOREPLY void Stop();
    Q_NOREPLY void Play();
    Q_NOREPLY void Seek(qlonglong offset);
    Q_NOREPLY void SetPosition(const QDBusObjectPath& trackId, qlonglong offset);
    Q_NOREPLY void OpenUri(const QString& uri);

Q_SIGNALS:
    void propertiesChanged();
    // Player
    Q_SCRIPTABLE void Seeked(qlonglong position);
    Q_SCRIPTABLE void nextClicked();
    Q_SCRIPTABLE void PreviousClicked();
    Q_SCRIPTABLE void playClicked();
    Q_SCRIPTABLE void pausedClicked();
    Q_SCRIPTABLE void stopClicked();
    Q_SCRIPTABLE void positionChanged(int );

private:

   QVariantMap last_metadata_;
   QString playBackStatus;
   QString loopStatus;
   QString freeDesktopObjectPath;
   QString serviceName;
   QString objectPath;
   QString interfaceName;
   QString AppName;

};

#endif // MPRIS2_PLAYER_H
