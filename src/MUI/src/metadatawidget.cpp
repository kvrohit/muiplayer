#include "metadatawidget.h"

MetaDataWidget::MetaDataWidget(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
}

void MetaDataWidget::setTag(const Meta::AudioTag &tag) {
    ui.labelTitle->setText(tag.title);
    ui.labelArtist->setText(tag.artist);
    ui.labelAlbum->setText(tag.album);
    ui.labelYear->setText(tag.year ? QString("%1").arg(tag.year) : QString("Unknown"));
    ui.labelAlbumArt->setPixmap(QPixmap::fromImage(tag.albumArt));
}

void MetaDataWidget::reset()
{
    ui.labelAlbum->setText("");
    ui.labelArtist->setText("");
    ui.labelTitle->setText("");
    ui.labelYear->setText("");

    m_qImage.load(Mui::NoAlbumArt);
    ui.labelAlbumArt->setPixmap(QPixmap::fromImage(m_qImage));
}
