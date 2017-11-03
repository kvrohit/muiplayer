#ifndef METADATA_H
#define METADATA_H

namespace Meta {
    typedef struct __audiotag {
        QString title;
        QString album;
        QString artist;
        QImage albumArt;
        int year;
    } AudioTag;
}

#endif // METADATA_H
