#include "musicdata.h"

MusicData::MusicData() {

}

MusicData::MusicData(const Meta::AudioTag &tag) {
    this->tag = tag;
}

QString MusicData::valueAt(int index) const {
    switch (index) {
    case Mui::STATEICON:
        return NULL;
    case Mui::SONGTITLE:
        return tag.title;
    case Mui::ARTIST:
        return tag.artist;
    case Mui::ALBUM:
        return tag.album;
    case Mui::DURATION:
        return Mui::formatTimeToQString(tag.duration);
    case Mui::FILEPATH:
        return tag.filepath;
    default:
        return QString();
    }
}

void MusicData::setData(const Meta::AudioTag &tag) {
    this->tag = tag;
}

MusicData::~MusicData() {
}
