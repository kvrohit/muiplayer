#ifndef MUSICDATA_H
#define MUSICDATA_H

#include <QString>
#include <QIcon>

#include "globals.h"

class MusicData
{
public:
    QString songtitle;
    QString album;
    QString artist;
    QString filepath;
    QString duration;
    QIcon icon;

    explicit MusicData(QString songtitle_ = QString(),
                       QString album_ = QString(),
                       QString artist_ = QString(),
                       QString filepath_ = QString(),
                       QString totaltime_ = QString(),
                       QIcon _icon = QIcon()) : songtitle(songtitle_),
        album(album_), artist(artist_), filepath(filepath_),
        duration(totaltime_), icon(_icon)
    {

    }
    explicit MusicData(const MusicData &data);
    ~MusicData();

    QString valueAt(int index) const;
    void setData(const MusicData &data);
    void setIconState(Mui::IconState state);
};

#endif // MUSICDATA_H
