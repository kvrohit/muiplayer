#include "metadatawidget.h"

MetaDataWidget::MetaDataWidget(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
}

void MetaDataWidget::setTag(AudioTag::GenericTag tag, std::string path)
{
    QPixmap qp;
	std::string cover_file;
	size_t pos;
	std::ifstream cover;    
    
    ui.labelTitle->setText(convertToUnicode(tag.title.c_str()));
    ui.labelArtist->setText(convertToUnicode(tag.artist.c_str()));
    ui.labelAlbum->setText(convertToUnicode(tag.album.c_str()));
    ui.labelYear->setText( tag.year ? QString("%1").arg(tag.year) : QString("Unknown") );
    
	pos = path.find_last_of("/\\");
	cover_file = path.substr(0, pos) + "/cover.jpg";
	qDebug() << cover_file.c_str();
	
	cover.open(cover_file.c_str());
	if(cover.is_open())
	{
		qp.load(cover_file.c_str());
		cover.close();
	}
    else if(tag.artfile != "")
    {           
        qp.load(tag.artfile.c_str());
    }
	else
	{
		qp.load(":/images/images/noart.jpg");
	}
	ui.labelAlbumArt->setPixmap(qp);
}
