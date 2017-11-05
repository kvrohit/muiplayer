#include "artdata.h"

ArtData::ArtData(QWidget *parent) : QWidget(parent) {
    defaultAlbumArt = QImage(Mui::NoAlbumArt);
    QHBoxLayout *hBoxLayout = new QHBoxLayout(this);
    m_albumArtLabel = new QLabel(this);

    m_songTitleLabel = new QLabel(Mui::WelcomeText);

    QWidget *placeHolder = new QWidget(this);
    QWidget *seekBarPlaceHolder = new QWidget(this);

    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    QHBoxLayout *hSeekBarLayout = new QHBoxLayout(this);

    m_totalTimeLabel = new QLabel(Mui::ZeroTime);
    m_currentTimeLabel = new QLabel(Mui::ZeroTime);
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

    m_albumArtLabel->setPixmap(QPixmap::fromImage(defaultAlbumArt));
    m_albumArtLabel->setFixedSize(48, 48);
    m_albumArtLabel->setScaledContents(true);

    hBoxLayout->addWidget(m_albumArtLabel);
    hBoxLayout->addWidget(placeHolder);
    hBoxLayout->setContentsMargins(2, 2, 2, 2);

    this->setLayout(hBoxLayout);
}

void ArtData::setTotalTime(const QString &totalTime) {
    m_totalTimeLabel->setText(totalTime);
}

void ArtData::setCurrentTime(const QString &currentTime) {
    m_currentTimeLabel->setText(currentTime);
}

void ArtData::setSongTitle(const QString &songTitle) {
    m_songTitleLabel->setText(songTitle);
}

void ArtData::setAlbumArt(const QImage &image) {
    m_albumArtLabel->setPixmap(QPixmap::fromImage(image));
}

void ArtData::resetAlbumArt() {
    m_albumArtLabel->setPixmap(QPixmap::fromImage(defaultAlbumArt));
}

void ArtData::reset() {
    m_currentTimeLabel->setText(Mui::ZeroTime);
    m_totalTimeLabel->setText(Mui::ZeroTime);
    m_songTitleLabel->setText(Mui::WelcomeText);

    m_seekBar->setValue(0);
    m_seekBar->setMaximum(0);
    m_seekBar->setMinimum(0);

    resetAlbumArt();
}

ArtData::~ArtData() {
}
