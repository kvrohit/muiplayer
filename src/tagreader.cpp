#include "tagreader.h"

TagReader::TagReader(QObject *parent) : QObject(parent)
{
    player = new QMediaPlayer(this);

    connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            this, SLOT(mediaStatusChanged(QMediaPlayer::MediaStatus)));
}

void TagReader::readTag(const QString &filepath) {
    player->setMedia(QUrl::fromLocalFile(filepath));
}

void TagReader::mediaStatusChanged(QMediaPlayer::MediaStatus status) {
    switch (status) {
    case QMediaPlayer::LoadedMedia:
        emit metaDataAvailable(getMetaData());
        break;
    default:
        break;
    }
}

Meta::AudioTag TagReader::getMetaData() const {
    Meta::AudioTag tag;

    tag.title = player->metaData(QMediaMetaData::Title).toString();
    tag.album = player->metaData(QMediaMetaData::AlbumTitle).toString();

    QVariant artist = player->metaData(QMediaMetaData::ContributingArtist);
    if (artist.isNull()) {
        artist = player->metaData(QMediaMetaData::AlbumArtist);
    }
    tag.artist = artist.toString();

    // tag.albumArt = player->metaData(QMediaMetaData::CoverArtImage).value<QImage>();
    tag.year = player->metaData(QMediaMetaData::Year).toInt();
    tag.filepath = player->media().canonicalUrl().toLocalFile();
    tag.duration = player->duration();

    return tag;
}

TagReader::~TagReader() {
    delete player;
}
