#ifndef COMMON_H
#define COMMON_H

#include <QString>


#define DAY_TO_MILLISECONDS ((unsigned int) (24*60*60*1000))
#define DAY_TO_SECONDS ((unsigned int) (24*60*60))

enum PlaybackMode
{
    CurrentItemOnce = 0,
    CurrentItemInLoop,
    Sequential,
    Loop,
    Random,
    Preview,
    RADIO
};

struct Track
{
    QString title;
    QString albumTitle;
    QString artist;
    QString albumArtist;
    QString path;
    QString genre;
    QString lengthString;
    QString cover;
    int ID; // if ID = -1 then it indicate that the track is not valide.
    int year ;
    int length ; // if length = -1 then it indicate that the track is not valide.
    int position ;
    int liked ; // By default a track is unlike
    int playCount ;
    int bitRate;
    int addedDate;
    int modifiedDate;
};

#endif // COMMON_H
