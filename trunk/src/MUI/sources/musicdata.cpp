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
    case STATEICON:
        return QString();
    case SONGTITLE:
        return songtitle;
    case ARTIST:
        return artist;
    case ALBUM:
        return album;
    case DURATION:
        return duration;
    case FILEPATH:
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

void MusicData::setIconState(IconState state)
{
    switch(state) {
    case PLAY_STATE:
        icon = QIcon(Mui::PlayIcon);
        return;
    case PAUSED_STATE:
        icon = QIcon(Mui::PauseIcon);
        return;
    case STOPPED_STATE:
    default:
        icon = QIcon();
    }
}

MusicData::~MusicData()
{
}
