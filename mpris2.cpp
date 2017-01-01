#include "mpris2.h"
#include "mpris2_Adaptor.h"



    MPRIS2::MPRIS2(QObject *parent) :
        QObject(parent)
    {

        AppName = "Player";
        new MediaPlayer2Adaptor(this);
        connect(this,SIGNAL(RaiseMainWindow()), this, SLOT(signalReceived()) );
        connect(this,SIGNAL(quitActivated()), this, SLOT(signalReceived()) );

    }
    // APP's Name
    QString MPRIS2::ApplicationName()const
    {
        return AppName;
    }

    void MPRIS2::setApplicationName(QString name)
    {
        AppName = QString(name);
    }

    // MPRISV2 MEDIAPLAYER2 Interface

    // Root Properties added in MPRIS 2.2

    bool MPRIS2::CanQuit()const
    {
        return true;
    }

    bool MPRIS2::CanRaise()const
    {
        return true;
    }

    void MPRIS2::raise()
    {
        Q_EMIT RaiseMainWindow();
    }

    void MPRIS2::quit()
    {
        Q_EMIT quitActivated();
    }

    bool MPRIS2::HasTrackList()const
    {
        return false;
    }

    QString MPRIS2::Identity()const
    {
        return AppName;
    }

    QString MPRIS2::DesktopEntry()const
    {
        // The basename of an installed .desktop
        return QString("player");
    }

    QStringList MPRIS2::SupportedUriSchemes() const {
      static QStringList res = QStringList() << "file"
                                             << "http"
                                             << "cdda"
                                             << "smb"
                                             << "sftp";
      return res;
    }


    QStringList MPRIS2::SupportedMimeTypes() const {
      static QStringList mimes = QStringList() << "application/ogg"
                                             << "application/x-ogg"
                                             << "application/x-ogm-audio"
                                             << "audio/aac"
                                             << "audio/mp4"
                                             << "audio/mpeg"
                                             << "audio/mpegurl"
                                             << "audio/ogg"
                                             << "audio/vnd.rn-realaudio"
                                             << "audio/vorbis"
                                             << "audio/x-flac"
                                             << "audio/x-mp3"
                                             << "audio/x-mpeg"
                                             << "audio/x-mpegurl"
                                             << "audio/x-ms-wma"
                                             << "audio/x-musepack"
                                             << "audio/x-oggflac"
                                             << "audio/x-pn-realaudio"
                                             << "audio/x-scpls"
                                             << "audio/x-speex"
                                             << "audio/x-vorbis"
                                             << "audio/x-vorbis+ogg"
                                             << "audio/x-wav"
                                             << "video/x-ms-asf"
                                             << "x-content/audio-player";
      return mimes;
    }

    void MPRIS2::signalReceived()const
    {
        qDebug() << " MPRIS2 : Signal received from DBUS" ;
    }

    // MPRISV2 MEDIAPLAYER2.Player Interface


