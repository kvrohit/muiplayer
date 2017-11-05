#ifndef MUSICDATA_H
#define MUSICDATA_H

#include <QString>
#include <QIcon>

#include "globals.h"
#include "metadata.h"

class MusicData
{
public:
    Meta::AudioTag tag;

    explicit MusicData();
    explicit MusicData(const Meta::AudioTag &);
    ~MusicData();

    QString valueAt(int index) const;
    void setData(const Meta::AudioTag &);
};

#endif // MUSICDATA_H
