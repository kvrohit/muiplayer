#include "metadatawidget.h"

MetaDataWidget::MetaDataWidget(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
    defaultAlbumArt = QImage(Mui::NoAlbumArt);
}

void MetaDataWidget::setTag(const Meta::AudioTag &tag) {
    ui.labelTitle->setText(tag.title);
    ui.labelArtist->setText(tag.artist);
    ui.labelAlbum->setText(tag.album);
    ui.labelYear->setText(tag.year ? QString("%1").arg(tag.year) : QString("Unknown"));
}

void MetaDataWidget::setAlbumArt(const QImage &albumArt) {
    ui.labelAlbumArt->setPixmap(QPixmap::fromImage(albumArt));
}

void MetaDataWidget::resetAlbumArt() {
    ui.labelAlbumArt->setPixmap(QPixmap::fromImage(defaultAlbumArt));
}

void MetaDataWidget::reset()
{
    ui.labelAlbum->setText("");
    ui.labelArtist->setText("");
    ui.labelTitle->setText("");
    ui.labelYear->setText("");

    resetAlbumArt();
}
