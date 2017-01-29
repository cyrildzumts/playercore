#include "playlists.h"

// Playlist Basis Class Implementations
Playlist::Playlist(AbstractDataAccessObject *data_access)
{
    this->data_access = data_access;
    init();
}


Playlist::Playlist(const QString name)
{
    init();
    _playlistTitle = name;
    data_access = nullptr;
}

Playlist::Playlist(const Playlist &pls)
{
    init();
    _playlistTitle = pls.playlistTitle();
    _title = pls.title();
    _tracks = pls.tracklist();
    _currentIndex = pls.currentIndex();
    _playbackMode = pls.playbackMode();
}

void Playlist::init(){
    clear();
    setRoles(AbstractModel::trackRoles());
    //connect (this,&Playlist::currentIndexChanged,this,&Playlist::emitAllMetadata);

    //connect (this,&Playlist::currentIndexChanged,[=](){this->setCurrentMedia(_currentIndex);});
    connect(this, &Playlist::dataChanged,this,&Playlist::durationChanged);

}

Track Playlist::getTrack(int trackID)
{
    Track track ;
    track.ID = -1;
    QSqlQuery query = data_access->queryTrack(trackID);
    if(query.isActive())
        {
            if(query.first())
            {
                track.ID = trackID;
                track.albumTitle = query.value("albumTitle").toString();
                track.artist = query.value("artist").toString();
                track.albumArtist = query.value("albumArtist").toString();
                track.title = query.value("title").toString();
                track.path = query.value("trackUrl").toString();
                track.genre = query.value("genre").toString();
                track.cover = query.value("cover").toString();
                track.position = query.value("trackNumber").toInt();
                track.length = query.value("length").toInt();
                track.playCount = query.value("playCount").toInt();
                track.liked = query.value("favorite").toInt();
                track.bitRate = query.value("bitrate").toInt();
                track.year = query.value("year").toInt();
                track.addedDate = query.value("addedDate").toInt();
                track.modifiedDate = query.value("modifiedDate").toInt();
            }

        }
        else
        {
            qDebug() << __PRETTY_FUNCTION__ << " Query Error : "
                     << query.lastError();
            qDebug() << "Query Error Last command 1 : "
                      << query.lastQuery();
            qDebug() << "Query Error Last command 2 : "
                     << query.executedQuery();

        }
    return track;
}

/*
void Playlist::emitAllMetadata()
{
    Q_EMIT titleChanged();
    Q_EMIT albumChanged();
    Q_EMIT artistChanged();
    Q_EMIT yearChanged();
    Q_EMIT coverChanged();
}
*/
QString Playlist::playlistTitle() const
{
    return _playlistTitle;
}

void Playlist::setPlaylistTitle(const QString &playlistTitle)
{
    _playlistTitle = playlistTitle;
    Q_EMIT playlistTitleChanged();
}

void Playlist::clear()
{


    _title.clear();
    _currentIndex = -1;
    _randomCurrentIndex = -1;
    setPlaybackMode(2);
    int start = 0;
    int end = isEmpty()? 0: mediaCount() - 1 ;
    beginRemoveRows(QModelIndex(),start, end);
      _tracks.clear();
     endRemoveRows();


}
QString Playlist::durationStr()const
{
    QString str = "";
    if(!isEmpty())
        ;
    else
    {
        const int hour = 3600;
        int d = duration();
        QTime tmpTime (0,0);
        QTime t =  tmpTime.addSecs(d);

        if(d < hour)
            str = t.toString("mm:ss");
        else
            str = t.toString("hh:mm:ss");
    }
    return str;
}
int Playlist::duration()const
{
    int d = 0;
    if(!isEmpty())
    {
        for(Track track : _tracks)
        {
            d+= track.length;
        }
    }
    return d;
}
void Playlist::setTitle(QString t)
{
    _title = t;
    Q_EMIT titleChanged();
}


bool Playlist::isIndexValid(int index) const
{
    return (index >= 0 ) &&( index < mediaCount());
}

QString Playlist::album()const
{
    if (!isEmpty() )
        return _tracks.at(_currentIndex).albumTitle;
    return QString();
}

int Playlist::year()const
{
    if (!isEmpty() )
        return _tracks.at(_currentIndex).year;
    return 0;
}

int Playlist::bitrate() const
{
    if (!isEmpty() )
        return _tracks.at(_currentIndex).bitRate;
    return 0;
}

int Playlist::favorite() const
{
    if (!isEmpty() )
        return _tracks.at(_currentIndex).liked;
    return 0;
}

int Playlist::addToFavorite(int trackID)
{
    auto query = data_access->query(QString("UPDATE BaseTableTracks SET favorite = 1 WHERE trackID = %1").arg(QString::number(trackID)));

    return query.lastError().isValid() ? 1 : 0 ;
}

QString Playlist::title()const
{
    QString str = "";
    if (!isEmpty())
        str = _tracks.at(_currentIndex).title;
    return str;
}

QString Playlist::artist()const
{
    QString str = "";
    if (!isEmpty() )
        str = _tracks.at(_currentIndex).artist;
    return str;
}

Playlist &Playlist::operator=(const Playlist &pls)
{

    _playlistTitle = pls.playlistTitle();
    _tracks.clear();
    _title = pls.title();
    this->addMedia(pls.tracklist());
    _currentIndex = pls.currentIndex();
    _playbackMode = pls.playbackMode();
    return *(this);
}

Playlist::~Playlist()
{
    qDebug() << __PRETTY_FUNCTION__ << " deleting playlist";
    clear();
}
int Playlist::currentRandomIndex()const
{
    return _randomCurrentIndex;
}

int Playlist::currentIndex()const
{
    return _currentIndex;
}

void Playlist::setCurrentIndex(int index)
{
    if (!isEmpty() && isIndexValid(index) )
    {
        _currentIndex = index;
        setTitle(_tracks.at(_currentIndex).albumTitle);
        Q_EMIT currentIndexChanged();
    }

}

int Playlist::playbackMode()const
{
    return _playbackMode;
}

void Playlist::setPlaybackMode(int  mode)
{
    _playbackMode = mode;
    Q_EMIT playbackModeChanged();
}

void Playlist::addMedia(Track track)
{
    if(isEmpty())
        _currentIndex = 0;
    beginInsertRows(QModelIndex(), rowCount(),rowCount());
    _tracks.push_back(track);
    endInsertRows();
}

void Playlist::addAlbum(const QString &album)
{

    Track track ;
    QSqlQuery query = data_access->query(
                QString("SELECT * FROM BaseTableTracks where AlbumTitle='%1' ORDER BY trackNumber;").arg(album));
    if(query.isActive())
        {
            while(query.next())
            {
                track.ID = query.value("trackID").toInt();
                track.albumTitle = query.value("albumTitle").toString();
                track.artist = query.value("artist").toString();
                track.albumArtist = query.value("albumArtist").toString();
                track.title = query.value("title").toString();
                track.path = query.value("trackUrl").toString();
                track.genre = query.value("genre").toString();
                track.cover = query.value("cover").toString();
                track.position = query.value("trackNumber").toInt();
                track.length = query.value("length").toInt();
                track.playCount = query.value("playCount").toInt();
                track.liked = query.value("favorite").toInt();
                track.bitRate = query.value("bitrate").toInt();
                track.year = query.value("year").toInt();
                track.addedDate = query.value("addedDate").toInt();
                track.modifiedDate = query.value("modifiedDate").toInt();
                addMedia(track);
            }

        }
        else
        {
            qDebug() << __PRETTY_FUNCTION__ << " Query Error : "
                     << query.lastError();
            qDebug() << "Query Error Last command 1 : "
                      << query.lastQuery();
            qDebug() << "Query Error Last command 2 : "
                     << query.executedQuery();
        }
}

void Playlist::addMedia(int trackID)
{

    Track track = getTrack(trackID);
    addMedia(track);

    //Q_EMIT dataChanged();
}

void Playlist::addMedia(std::vector<Track> tracks)
{
    if(isEmpty()  )
        _currentIndex = 0;
    beginInsertRows(QModelIndex(),rowCount(),tracks.size() -1 );
    _tracks.insert(_tracks.end(),tracks.begin(),tracks.end());
    endInsertRows();
}

Track Playlist::currentTrack()const
{
    Track track = Track();
    if(!isEmpty())
    {
        track =  _tracks.at(_currentIndex);
    }
    return track;
}

bool Playlist::isEmpty()const
{
    return _tracks.empty();
}

void Playlist::insertMedia(int pos, Track track)
{
    if(isIndexValid(pos))
    {
        beginInsertRows(QModelIndex(),pos,pos);
        _tracks.insert(_tracks.begin() + pos, track);
        endInsertRows();
    }

}

void Playlist::insertMedia(int pos, std::vector<Track> tracks)
{
    if(isIndexValid(pos))
    {
        beginInsertRows(QModelIndex(),pos,tracks.size() - 1);
        _tracks.insert(_tracks.begin()+ pos, tracks.begin(),tracks.end());
        endInsertRows();
    }

}

void Playlist::insertMedia(int pos, int trackID)
{
    Track track = getTrack(trackID);
    if(track.ID != -1)
    {
        insertMedia(pos, track);
    }
}

QString Playlist::media(int index) const
{
    QString str = "";
    return isIndexValid(index) ? _tracks.at(index).path : str;
}

QString Playlist::media() const
{
   return isIndexValid(_currentIndex) ? _tracks.at(_currentIndex).path : QString();
}

void Playlist::setCurrentMedia(int index)
{
    if(isIndexValid(index))
    {
        _currentIndex = index;
        _currentMedia = _tracks.at(_currentIndex).path;
        Q_EMIT currentIndexChanged();
    }
}

int Playlist::mediaCount()const
{
    return (int)_tracks.size();
}

int Playlist::nextIndex() const
{
    return !isEmpty() ? _currentIndex + 1: -1;
}

int Playlist::previousIndex() const
{
    return !isEmpty() ? _currentIndex - 1: -1;
}

bool Playlist::removeMedia(int pos)
{
    bool removed = false;
        if( isIndexValid(pos))
        {

            beginRemoveRows(QModelIndex(),pos,pos);
            _tracks.erase(_tracks.begin() + pos);
            endRemoveRows();
            if((_currentIndex == (_tracks.size() - 1 )) || (pos < _currentIndex))
                _currentIndex--;
            else if (_currentIndex == 0  && mediaCount() > 1) {
                    ;
            }
            else _currentIndex = -1;

            removed = true;
        }
    return removed;
}

void Playlist::save(const QUrl &location, const char *format)
{

}

void Playlist::save(QIODevice *device, const char *format)
{

}


/**
 * @brief Playlist::next advances the index up to the next track on the list.
 * next pays attention to the playbackmode.
 * This methode is mainly used to receive the signal when the user has
 * clicked on the next button.
 * @see Playlist::nextMedia() if you want to get the next track.
 */
void Playlist::next()
{
    /**
     * Before I go to the next song, I have to check the playbackMode:
     * On CurrentItemOnce : I set currentIndex at the end and stop the player.
     * On CurrentItemLoop : I loop back to the same currentIndex
     * On Sequential : I navigate sequencialy the playlist until the end.
     * On Loop :  At the end of the playlist I set currentIndex to the first position
     * an continue playing the file from there.
     * On Rondom : the next random track index is set as currentIndex
     *
    */

    if(!isEmpty())
    {
        switch (_playbackMode)
        {
        case PlaybackMode::CurrentItemOnce :
        {
            Q_EMIT nomedia();
        }
            break;

        case PlaybackMode::CurrentItemInLoop :
        {
            Q_EMIT currentIndexChanged();
        }
            break;
        case PlaybackMode::Sequential :
        {
            if (isIndexValid(_currentIndex))
            {
                _currentIndex++;
                Q_EMIT currentIndexChanged();
            }

            else
            {
                Q_EMIT nomedia();
            }
        }
            break;

        case PlaybackMode::Loop :
        {
            if(_currentIndex <_tracks.size() -1)
            {
                _currentIndex++;
            }
            else
            {
                _currentIndex = 0;
            }
            Q_EMIT currentIndexChanged();
        }
            break;
        case PlaybackMode::Random:
        {
            if( ( _randomCurrentIndex  ) < _tracks.size() - 1)
            {
                _randomCurrentIndex++;
                _currentIndex =  randomIndices.at(_randomCurrentIndex);
                Q_EMIT currentIndexChanged();
            }
            else
            {
                Q_EMIT nomedia();
            }
        }
            break;
        }
    }
}

/**
 * @brief Playlist::previous set the current index  to the previous track on the list.
 * previous pays attention to the playbackmode.
 * This methode is mainly used to receive the signal when the user has
 * clicked on the previous button.
 */
void Playlist::previous()
{
    if(!isEmpty())
    {
        if(_currentIndex >= 0 && _currentIndex < mediaCount())
        {
            switch (_playbackMode)
            {
            case PlaybackMode::CurrentItemOnce :
            case PlaybackMode::CurrentItemInLoop :
            case PlaybackMode::Sequential :
                if(_currentIndex - 1 >= 0 )
                {
                    _currentIndex--;
                    Q_EMIT currentIndexChanged();
                }
                break;
            case PlaybackMode::Loop :
            {
                if(_currentIndex - 1 < 0)
                {
                    _currentIndex = _tracks.size() - 1;

                }
                else
                {
                    _currentIndex--;

                }
                Q_EMIT currentIndexChanged();
            }
                break;
            case PlaybackMode::Random:
            {
                if( (_randomCurrentIndex  - 1 ) >= 0 )
                {
                    _randomCurrentIndex--;
                    _currentIndex = randomIndices.at(_randomCurrentIndex);
                    Q_EMIT currentIndexChanged();
                }
                break;
            }
            }
        }

    }
}

void Playlist::shuffle()
{
    switch(_playbackMode)
    {
        case PlaybackMode::Random:
            _currentIndex = _randomCurrentIndex;
            setPlaybackMode( PlaybackMode::Sequential);
            break;

        default:
        {
            randomIndices = RandomGenerator::Instance()->getShuffleList(_tracks.size());
            _randomCurrentIndex = 0;
            setPlaybackMode( PlaybackMode::Random);
            break;
        }

    }
}

void Playlist::setRoles(const QHash<int, QByteArray> &roles)
{
    _roles = roles;
}


QHash<int, QByteArray> Playlist::roleNames()const
{
    return _roles;
}

void Playlist::resetInternalData()
{
    _tracks.clear();
    beginResetModel();
    _currentIndex = 0;
    endResetModel();
}

int Playlist::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _tracks.size();
}


QVariant Playlist::data(const QModelIndex &index, int role) const
{
    if (!isIndexValid(index.row()))
    {
        return QVariant();
    }

    Track track = _tracks.at(index.row());

    switch (role)
    {
    case TrackRole::TrackTitle:
        return track.title;
    case Path:
        return track.path;
    case ArtistTrack:
        return track.artist;
    case Position:
        return track.position;
    case Length:
        return track.length;
    case LengthString:
    {
        const int hour = 3600;
        QTime tmpTime (0,0);
        QTime t =  tmpTime.addSecs(track.length);

        if(track.length < hour)
            return t.toString("mm:ss");
        else
            return t.toString("hh:mm:ss");
    }
    case TrackID:
        return track.ID;
    case Bitrate:
        return track.bitRate;
    //case PlaylistRole:
        //return _title;
    //case  YearRole:
        //return track.year;
    case Favorite:
        return track.liked;
    case Playcount:
        return track.playCount;
    case Cover:
        return track.cover;
    case AlbumArtist:
        return track.albumArtist;
    case TrackGenre:
        return track.genre;

    default :
           return QVariant();
    }
}
void Playlist::setTracklist(std::vector<Track> tracks)
{
    clear();

    randomIndices = RandomGenerator::Instance()->getShuffleList(tracks.size());
    addMedia(tracks);

    Q_EMIT tracklistChanged();
}


void Playlist::setTracklist(const std::vector<int> &ids)
{
    if(!ids.empty())
    {
        clear();

        randomIndices = RandomGenerator::Instance()->getShuffleList(ids.size());

        for(auto id: ids)
        {
            addMedia(id);
        }

        Q_EMIT tracklistChanged();
    }
}

std::vector<Track> Playlist::tracklist()const
{
    return _tracks;
}

void Playlist::setCover(QString path)
{
    _cover = path;
    Q_EMIT coverChanged();
}

QString Playlist::cover()const
{
    return isIndexValid(_currentIndex) ? _tracks.at(_currentIndex).cover : QString();
}

void Playlist::showContents()
{
    qDebug() << " The Playlist contains " << _tracks.size() << " Tracks." ;
    for (auto track : _tracks)
    {
        qDebug() << track.title;
    }
}

int Playlist::currentTrackID()const
{
   return isIndexValid(_currentIndex) ? _tracks.at(_currentIndex).ID : -1;
}



// PLAYLIST2 Implementations

Playlist2::Playlist2(AbstractDataAccessObject *data_access): Playlist(data_access)
{
    setPlaybackMode(2);
}

Playlist2::~Playlist2()
{
    qDebug() << __PRETTY_FUNCTION__ << " deleting Playlist2";
}
void Playlist2::changeState(PlaylistState *state)
{
    this->state = state;
}

void Playlist2::setPlaybackMode(int mode)
{
    switch (mode) {
    case PlaybackMode::CurrentItemOnce:
        changeState(CurrentItemOnceState::instance());
        break;
    case PlaybackMode::CurrentItemInLoop:
        changeState(CurrentItemInLoopState::instance());
        break;
    case PlaybackMode::Sequential:
        changeState(SequentialState::instance());
        break;
    case PlaybackMode::Loop:
        changeState(LoopState::instance());
        break;
    case PlaybackMode::Random:
    case PlaybackMode::Preview:
    case PlaybackMode::RADIO:
        changeState(RandomState::instance());
        break;
    default:
        break;
    }
}

int Playlist2::playbackMode()const
{
    return state->playbackMode();
}

void Playlist2::next()
{
    state->next(this);

}

void Playlist2::previous()
{
    state->previous(this);
}


// STATES IMPLEMENTATIONS

// SEQUENTIAL STATE
SequentialState *SequentialState::_instance = nullptr;

SequentialState::SequentialState()
{
    mode = PlaybackMode::Sequential;
}

SequentialState::~SequentialState()
{
    delete _instance;
    _instance = nullptr;
}

SequentialState *SequentialState::instance()
{
    if(_instance == nullptr)
         _instance = new SequentialState();

    return _instance;
}


void SequentialState::next(Playlist2 *pls)
{
    int index = -1;
    index = pls->currentIndex() + 1;
    if (pls->isIndexValid(index))
    {
        pls->setCurrentIndex(index);
    }

    else
    {
        Q_EMIT pls->nomedia();
    }
}

void SequentialState::previous(Playlist2 *pls)
{
    int index = -1;
    index = pls->currentIndex();
    if((index - 1 ) >= 0)
    {
            index--;
            pls->setCurrentIndex(index);
    }

}


// RANDOM STATES
RandomState *RandomState::_instance = nullptr;

RandomState::RandomState()
{
    mode = PlaybackMode::Random;
}


RandomState *RandomState::instance()
{
    if(_instance == nullptr)
        _instance = new RandomState();
    return _instance;
}

RandomState::~RandomState()
{
    delete _instance;
    _instance = nullptr;
}

void RandomState::next(Playlist2 *pls)
{
    int index = -1;
    index = pls->currentRandomIndex();
    if( index < (pls->mediaCount() - 1))
    {
        index++;
        index = pls->randomIndices.at(index);
        //pls->_currentIndex =  pls->randomIndices.at(pls->_randomCurrentIndex);
        pls->setCurrentIndex(index);
    }
    else
    {
        Q_EMIT pls->nomedia();
    }
}


void RandomState::previous(Playlist2 *pls)
{
    if( (pls->_randomCurrentIndex  - 1 ) >= 0 )
    {
        pls->_randomCurrentIndex--;
        pls->_currentIndex = pls->randomIndices.at(pls->_randomCurrentIndex);
        pls->setCurrentIndex(pls->currentIndex());
    }
}

// LOOPSTATE

LoopState* LoopState::_instance = nullptr;

LoopState::LoopState()
{
    mode = PlaybackMode::Loop;
}

LoopState * LoopState::instance()
{
    if(_instance == nullptr)
        _instance = new LoopState();
    return _instance;
}

LoopState::~LoopState()
{
    delete _instance;
    _instance = nullptr;
}


void LoopState::next(Playlist2 *pls)
{

    int index = -1;
    if(pls->currentIndex() < (pls->mediaCount() -1) )
    {
        index = pls->currentIndex() + 1;
    }
    else
    {
        index = 0;;
    }
    pls->setCurrentIndex(index);
}



void LoopState::previous(Playlist2 *pls)
{
    int index = -1;
    if(pls->currentIndex() - 1 < 0)
    {
        index = pls->mediaCount() - 1;
    }
    else
    {
        index = pls->currentIndex() - 1;
    }
    pls->setCurrentIndex(index);
}


// CurrentItemOnceState

CurrentItemOnceState *CurrentItemOnceState::_instance = nullptr;


CurrentItemOnceState::CurrentItemOnceState()
{
    mode = PlaybackMode::CurrentItemOnce;
}

CurrentItemOnceState::~CurrentItemOnceState()
{
    delete _instance;
    _instance = nullptr;
}

CurrentItemOnceState *CurrentItemOnceState::instance()
{
    if(_instance == nullptr)
        _instance = new CurrentItemOnceState();
    return _instance;
}

void CurrentItemOnceState::next(Playlist2 *pls)
{
    Q_EMIT pls->nomedia();
}

void CurrentItemOnceState::previous(Playlist2 *pls)
{
    int index = pls->currentIndex() - 1;
    if(index >= 0)
    {
      pls->setCurrentIndex(index);
    }
}


// CurrentItemInLoopState

CurrentItemInLoopState *CurrentItemInLoopState::_instance = nullptr;


CurrentItemInLoopState::CurrentItemInLoopState()
{
    mode = PlaybackMode::CurrentItemInLoop;
}

CurrentItemInLoopState* CurrentItemInLoopState::instance()
{
    if(_instance == nullptr)
        _instance = new CurrentItemInLoopState();
    return _instance;
}


CurrentItemInLoopState::~CurrentItemInLoopState()
{
    delete _instance;
    _instance = nullptr;
}


void CurrentItemInLoopState::next(Playlist2 *pls)
{
    Q_EMIT pls->currentIndexChanged();
}


void CurrentItemInLoopState::previous(Playlist2 *pls)
{
    if((pls->currentIndex() - 1 >= 0))
    {
       pls->setCurrentIndex(pls->currentIndex() - 1);
    }
}
