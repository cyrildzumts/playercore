#ifndef TAGREADER_H
#define TAGREADER_H

#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QDirIterator>
#include <taglib.h>
#include <tag.h>
#include <id3v2tag.h>
#include <id3v1tag.h>
#include <taglib/fileref.h>
#include <tfile.h>
#include <mpegfile.h> // for MPEG::File
#include <asffile.h> // WMA::File
#include <flacfile.h> // FLAC::File
#include <mp4file.h>
#include <oggfile.h>
#include <vorbisfile.h>
#include <oggflacfile.h>
#include <vorbisproperties.h>
#include <tpropertymap.h>
#include <tstringlist.h>

#include <QStringList>
#include <QTextStream>
#include <QDebug>
#include "common.h"
//ID3 LIB headers
//#include <id3/tag.h>
//#include <id3/misc_support.h>
#include <QDir>


    class Tagreader {
    public:
        static Track tagreader( const QString &file);
        static Track readTag(TagLib::File& file);
        static QString cover(const QString &audioFile);
    };

#endif // TAGREADER_H
