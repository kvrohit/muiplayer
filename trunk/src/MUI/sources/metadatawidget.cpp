#include "metadatawidget.h"

MetaDataWidget::MetaDataWidget(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
}

void MetaDataWidget::setTag(const AudioTag::GenericTag &tag, const std::string &path)
{
    std::string cover_file;
    size_t pos;
    std::ifstream cover;
    QImage img;

    ui.labelTitle->setText(Mui::convertToUnicode(tag.title));
    ui.labelArtist->setText(Mui::convertToUnicode(tag.artist));
    ui.labelAlbum->setText(Mui::convertToUnicode(tag.album));
    ui.labelYear->setText(tag.year ? QString("%1").arg(tag.year) : QString("Unknown"));

    pos = path.find_last_of("/\\");
    cover_file = path.substr(0, pos) + "/cover.jpg";
    qDebug() << cover_file.c_str();

    m_qImage.load(Mui::NoAlbumArt);

    cover.open(cover_file.c_str());
    if(tag.artfile != "") {
        if(!m_qImage.load(tag.artfile.c_str()) && cover.is_open()) {
            m_qImage.load(cover_file.c_str());
        }
    }
    else if(cover.is_open()) {
        m_qImage.load(cover_file.c_str());
    }
    cover.close();
    ui.labelAlbumArt->setPixmap(QPixmap::fromImage(m_qImage));
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
