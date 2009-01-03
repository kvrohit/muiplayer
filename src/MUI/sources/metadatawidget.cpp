#include "metadatawidget.h"

MetaDataWidget::MetaDataWidget(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
}

void MetaDataWidget::setTag(AudioTag::GenericTag tag)
{
    QPixmap qp;
    qp.load(":/images/images/noart.jpg");
    
    ui.labelTitle->setText(convertToUnicode(tag.title.c_str()));
    ui.labelArtist->setText(convertToUnicode(tag.artist.c_str()));
    ui.labelAlbum->setText(convertToUnicode(tag.album.c_str()));
    ui.labelYear->setText( tag.year ? QString("%1").arg(tag.year) : QString("Unknown") );
    ui.labelAlbumArt->setPixmap(qp);
    
    if(tag.artfile != "")
    {           
        qp.load(tag.artfile.c_str());
        ui.labelAlbumArt->setPixmap(qp);
    }
}
