#ifndef METADATA_H
#define METADATA_H

#include <QImage>
#include <QString>

namespace Meta {
    typedef struct __audiotag {
        QString title;
        QString album;
        QString artist;
        QImage albumArt;
        int year;
        QString filepath;
        qint64 duration;

        __audiotag() : title(QString()), album(QString()), artist(QString()),
        albumArt(QImage()), year(0), filepath(QString()), duration(0) {

        }
    } AudioTag;
}

#endif // METADATA_H
