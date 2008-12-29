#include "metadatawidget.h"

MetaDataWidget::MetaDataWidget(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
}

void MetaDataWidget::setTag(AudioTag::GenericTag tag)
{
    QPixmap qp;
    
    ui.labelTitle->setText(tag.title.c_str());
    ui.labelArtist->setText(tag.artist.c_str());
    ui.labelAlbum->setText(tag.album.c_str());
    ui.labelYear->setText(QString("%1").arg(tag.year));
    
    if(tag.artfile != "")
    {           
        qp.load(tag.artfile.c_str());
        ui.labelAlbumArt->setPixmap(qp);
    }
}
