#ifndef ARTDATA_H
#define ARTDATA_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QImage>
#include <QPixmap>
#include <QFileInfo>
#include <QDebug>

#include "musicdata.h"
#include "globals.h"

class ArtData : public QWidget
{
    Q_OBJECT
private:
    QLabel *m_totalTimeLabel;
	QLabel *m_currentTimeLabel;
    QLabel *m_songTitleLabel;
	QLabel *m_albumArtLabel;

public:
    ArtData(QWidget *parent = 0);

    void setTotalTime(const QString &totalTime);
    void setCurrentTime(const QString &currentTime);
    void setSongTitle(const QString &songTitle);

    void setAlbumArt(const QImage &);
    void setAlbumArt(const QString &filePath, const QString &albumArtPath);
    void reset();

    QSlider *m_seekBar;

    ~ArtData();
};

#endif // ARTDATA_H
