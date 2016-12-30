#include "tagreader.h"


Track Tagreader::tagreader(const QString &file)
{
    QFileInfo tmpfile (file);
    Track track;
    if(tmpfile.isFile())
    {
        QFileInfo audiofile(file);
        QString ext = audiofile.suffix().toLower();
        if (ext == "mp3")
        {
            TagLib::MPEG::File tagLibFile (file.toStdString().c_str());

            track = Tagreader::readTag(tagLibFile);
        }
        else
        if (ext == "flac")
        {
            TagLib::FLAC::File tagLibFile (file.toStdString().c_str());

            track = Tagreader::readTag(tagLibFile);

        }
        else
        if (ext == "wma")
        {
            TagLib::ASF::File tagLibFile (file.toStdString().c_str());

            track = Tagreader::readTag(tagLibFile);

        }
        else
        if (ext == "ogg")
        {
            TagLib::Ogg::Vorbis::File tagLibFile (file.toStdString().c_str());

            track = Tagreader::readTag(tagLibFile);


        }
        else
        if ((ext == "m4a" )|| (ext == "mp4"))
        {
            TagLib::MP4::File tagLibFile (file.toStdString().c_str());

            track = Tagreader::readTag(tagLibFile);

        }
        else
        {

             track.length = -1;
             qDebug()<< "ERROR: File type ."+ ext + " is not supported" ;
             return track;
        }
    }
    else
    {

        track.length = -1;
        qDebug()<< "ERROR: " << file  << " is not a file";
        return track;
    }

    track.cover = cover(file);
    track.path = file;
    return track;

}

Track Tagreader::readTag(TagLib::File &file)
{
    auto propertiesMap = file.properties();
    auto tag = file.tag();
    auto audioproperties =file.audioProperties();
    Track track;

    auto list = propertiesMap["ALBUM"];
    if(!list.isEmpty())
    {
        track.albumTitle = list.front().toCString(true);
    }
    else
        track.albumTitle = tag->album().toCString(true);

    list = propertiesMap["ARTIST"];
    if(!list.isEmpty())
    {
        track.artist = list.front().toCString(true);
    }
    else
        track.artist = tag->artist().toCString(true);

        if(track.artist.count() > 50)
        {
            track.artist = QString("VARIOUS ARTISTS");
        }
    list = propertiesMap["ALBUMARTIST"];
    if(!list.isEmpty())
    {
        track.albumArtist = list.front().toCString(true);
    }
    else
    {
        track.albumArtist = track.artist;
    }

    list = propertiesMap["TITLE"];
    if(!list.isEmpty())
    {
        track.title = list.front().toCString(true);
    }
    else
        track.title = tag->title().toCString(true);

    list = propertiesMap["GENRE"];
    if(!list.isEmpty())
    {
        track.genre = list.front().toCString(true);
    }
    else
    {
        track.genre = QString("UNKNOWN");
    }
    track.bitRate = audioproperties->bitrate();
    track.length = audioproperties->length();
    track.year = tag->year();
    track.position = tag->track();

    tag = NULL;
    audioproperties = NULL;
    delete tag;
    delete audioproperties;
    return track;
}


QString Tagreader::cover(const QString &audioFile)
{
    QFileInfo file(audioFile);
    QString path = file.path();
    QDirIterator workingDir (path, QStringList()<< "*.jpeg"<< "*.jpg" << "*.png" << "*.svg",QDir::Files | QDir::NoDotAndDotDot,QDirIterator::Subdirectories);
    if (workingDir.hasNext())
    {
        return QString(workingDir.next());
    }
    else
        return QString("");
}

