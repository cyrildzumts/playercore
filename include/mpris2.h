#ifndef MPRIS2_H
#define MPRIS2_H

/*
 *
 * This class implement the MPRISV2 SPECIFIACTIONS.
 * This code is open SOURCE:
 * Implemented by Cyrille Ngassam Nkwenga
 * contact : cyrildz@gmail.com
 *
 */


#include <QtDBus>

//typedef QList<QVariantMap> TrackMetadata;
//typedef QList<QDBusObjectPath> TrackIds;

//Q_DECLARE_METATYPE(TrackMetadata)

class MPRIS2 : public QObject
{

        Q_OBJECT
        Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2")
        // APP's Name
        Q_PROPERTY(QString ApplicationName READ ApplicationName WRITE setApplicationName)

        // MPRISV2 MEDIAPLAYER2 INTERFACE
        Q_PROPERTY( bool CanQuit  READ CanQuit)
        Q_PROPERTY( bool CanRaise READ CanRaise )
        Q_PROPERTY( bool HasTrackList READ HasTrackList )
        Q_PROPERTY( QString Identity READ Identity )
        Q_PROPERTY( QString DesktopEntry READ DesktopEntry )
        //Q_PROPERTY( QStringList SupportedUriSchemes READ SupportedUriSchemes )
        Q_PROPERTY( QStringList SupportedMimeTypes READ SupportedMimeTypes )



        //MPRISV2 MediaPlayer2.Tracklist : Not needed now
        //Q_PROPERTY( TrackIds Tracks READ Tracks )
        //Q_PROPERTY( bool CanEditTracks READ CanEditTracks )



    public:
        explicit MPRIS2(QObject *parent = 0);

    Q_SIGNALS:

    // org.mpris.MediaPlayer2


      void RaiseMainWindow();
      void quitActivated();


    public Q_SLOTS:
        //void notify(QString notificationNAme);
        // APP's Name getter and setter
        QString ApplicationName()const;
        void signalReceived()const;
        void setApplicationName(QString name);

        // MPRISV2 MEDIAPLAYER2 INTERFACE
        bool CanQuit()const;
        bool CanRaise() const;
        bool HasTrackList()const;
        QString Identity()const;
        QString DesktopEntry() const;
        QStringList SupportedUriSchemes()const;
        QStringList SupportedMimeTypes()const;

        // Version 2.2
        bool CanSetFullscreen() const { return false; }
        bool Fullscreen() const { return false; }
        Q_NOREPLY void SetFullscreen(bool) {}

        Q_NOREPLY void raise();
        Q_NOREPLY void quit();



        //MPRISV2 MediaPlayer2.Tracklist : Not needed now
        //TrackIds Tracks() const;
        //bool CanEditTracks() const;
        //TrackMetadata GetTracksMetadata(const TrackIds& tracks) const;
        //void AddTrack(const QString& uri, const QDBusObjectPath& afterTrack, bool setAsCurrent);
        //void RemoveTrack(const QDBusObjectPath& trackId);
        //void GoTo(const QDBusObjectPath& trackId);

    private:
//       QString serviceName ;
//       QString objectPath ;
       //QString freedesktopPath ;
      QString AppName;



    };



#endif // MPRIS2_H
