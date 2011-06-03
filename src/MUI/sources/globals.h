#ifndef GLOBALS_H
#define GLOBALS_H

#include <QString>
#include <QDebug>

namespace Mui {
    const QString PlayIcon          =   ":/images/play_2.png";
    const QString PauseIcon         =   ":/images/pause_2.png";
    const QString MediaPlaybackIcon =   ":/images/media_playback_start.png";
    const QString MediaPauseIcon    =   ":/images/media_playback_pause.png";
    const QString NoAlbumArt        =   ":/images/noart.jpg";
    const QString MUIVersion        =   "0.0.3";
    const QString WelcomeText       =   "Welcome to MUI";
    const QString ZeroTime          =   "00:00";
    const QString ModelDataFileName =   "/mui_music_data.dat";
    const QString PlaylistFilter    =   "Playlist (*.m3u);;All Files (*.*)";
    const QString AudioFilter       =   "Audio (*.mp3 *.aac *.mp4 *.ogg);;All files (*.*)";

    const qint32 MagicNumber        =   0x69696969;

    enum { STATEICON, SONGTITLE, ARTIST, ALBUM, DURATION, FILEPATH };
    enum IconState { PLAY_STATE, PAUSED_STATE, STOPPED_STATE };

    QString convertToUnicode(const std::string &data);
    QString formatTimeToQString(quint32 duration);
};

#endif
