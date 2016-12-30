#ifndef COMMON_H
#define COMMON_H

#include <QString>



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
