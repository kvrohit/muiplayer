#ifndef TAGREADER_H
#define TAGREADER_H

#include <QObject>
#include <QMediaPlayer>
#include <QMediaMetaData>

#include "metadata.h"

class TagReader : public QObject
{
    Q_OBJECT
public:
    explicit TagReader(QObject *parent = nullptr);
    void readTag(const QString &);
    ~TagReader();

signals:
    void metaDataAvailable(const Meta::AudioTag &);

public slots:
    void mediaStatusChanged(QMediaPlayer::MediaStatus status);

private:
    QMediaPlayer *player;
    Meta::AudioTag getMetaData() const;
};

#endif // TAGREADER_H
