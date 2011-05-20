#include "artdata.h"

ArtData::ArtData(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *hBoxLayout = new QHBoxLayout(this);
    m_albumArtLabel = new QLabel(this);

    m_songTitleLabel = new QLabel("Songtitle");

    QWidget *placeHolder = new QWidget(this);
    QWidget *seekBarPlaceHolder = new QWidget(this);

    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    QHBoxLayout *hSeekBarLayout = new QHBoxLayout(this);

    m_totalTimeLabel = new QLabel("00:00");
    m_currentTimeLabel = new QLabel("00:00");
    m_seekBar = new QSlider(Qt::Horizontal, this);

    hSeekBarLayout->addWidget(m_currentTimeLabel);
    hSeekBarLayout->addWidget(m_seekBar);
    hSeekBarLayout->addWidget(m_totalTimeLabel);

    seekBarPlaceHolder->setLayout(hSeekBarLayout);
    hSeekBarLayout->setContentsMargins(0, 0, 0, 0);

    vBoxLayout->addWidget(m_songTitleLabel);
    vBoxLayout->addWidget(seekBarPlaceHolder);

    vBoxLayout->setContentsMargins(0, 0, 0, 0);
    vBoxLayout->setSpacing(0);

    placeHolder->setLayout(vBoxLayout);

    QImage qImage(Mui::NoAlbumArt);
    m_albumArtLabel->setPixmap(QPixmap::fromImage(qImage));
    m_albumArtLabel->setFixedSize(48, 48);
    m_albumArtLabel->setScaledContents(true);

    QFont font("Segoe UI", 9, 0, false);

    m_songTitleLabel->setFont(font);
    m_currentTimeLabel->setFont(font);
    m_totalTimeLabel->setFont(font);

    hBoxLayout->addWidget(m_albumArtLabel);
    hBoxLayout->addWidget(placeHolder);

    hBoxLayout->setContentsMargins(2, 2, 2, 2);

    this->setLayout(hBoxLayout);
}

void ArtData::setTotalTime(const QString &totalTime)
{
    m_totalTimeLabel->setText(totalTime);
}

void ArtData::setCurrentTime(const QString &currentTime)
{
    m_currentTimeLabel->setText(currentTime);
}

void ArtData::setSongTitle(const QString &songTitle)
{
    m_songTitleLabel->setText(songTitle);
}

void ArtData::setAlbumArt(const QString &filePath, const QString &albumArtPath)
{
    QImage qImage;
	int index = filePath.lastIndexOf("/");
    QString coverPath = filePath.left(index) + "/cover.jpg";
    QFileInfo f(coverPath);

    qImage.load(Mui::NoAlbumArt);
    qDebug() << albumArtPath;

    if(!albumArtPath.isEmpty()) {
        if(!qImage.load(albumArtPath) && f.exists()) {
            qImage.load(coverPath);
        }
    }
    else if(f.exists()) {
        qImage.load(coverPath);
    }

    qDebug() << "Setting pixmap";
    m_albumArtLabel->setPixmap(QPixmap::fromImage(qImage));
}

void ArtData::reset()
{
    QImage qImage;
	m_currentTimeLabel->setText("00:00");
    m_totalTimeLabel->setText("00:00");
	
    qImage.load(Mui::NoAlbumArt);
    m_albumArtLabel->setPixmap(QPixmap::fromImage(qImage));
    
	m_seekBar->setValue(0);
    m_seekBar->setMaximum(0);
    m_seekBar->setMinimum(0);
}

ArtData::~ArtData()
{
}
