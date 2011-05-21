#include "musicdata.h"

MusicData::MusicData(const MusicData &data)
{
    songtitle = data.songtitle;
    artist = data.artist;
    album = data.album;
    duration = data.duration;
    filepath = data.filepath;
    icon = QIcon(data.icon);
}

QString MusicData::valueAt(int index) const
{
    switch(index) {
    case Mui::STATEICON:
        return QString();
    case Mui::SONGTITLE:
        return songtitle;
    case Mui::ARTIST:
        return artist;
    case Mui::ALBUM:
        return album;
    case Mui::DURATION:
        return duration;
    case Mui::FILEPATH:
        return filepath;
    default:
        return QString();
    }
}

void MusicData::setData(const MusicData &data)
{
    icon = QIcon(data.icon);
    songtitle = data.songtitle;
    artist = data.artist;
    album = data.album;
    filepath = data.filepath;
    duration = data.duration;
}

void MusicData::setIconState(Mui::IconState state)
{
    switch(state) {
    case Mui::PLAY_STATE:
        icon = QIcon(Mui::PlayIcon);
        return;
    case Mui::PAUSED_STATE:
        icon = QIcon(Mui::PauseIcon);
        return;
    case Mui::STOPPED_STATE:
    default:
        icon = QIcon();
    }
}

MusicData::~MusicData()
{
}
