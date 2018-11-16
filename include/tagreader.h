#ifndef TAGREADER_H
#define TAGREADER_H

#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QDirIterator>
#include <taglib/taglib.h>

#include <taglib/tag.h>
#include <taglib/id3v2tag.h>
#include <taglib/id3v1tag.h>
#include <taglib/fileref.h>
#include <taglib/tfile.h>
#include <taglib/mpegfile.h> // for MPEG::File
#include <taglib/asffile.h> // WMA::File
#include <taglib/flacfile.h> // FLAC::File
#include <taglib/mp4file.h>
#include <taglib/oggfile.h>
#include <taglib/vorbisfile.h>
#include <taglib/oggflacfile.h>
#include <taglib/vorbisproperties.h>
#include <taglib/tpropertymap.h>
#include <taglib/tstringlist.h>

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
