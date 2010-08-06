#include "playlistmodel.h"

PlaylistModel::PlaylistModel()
{
    QStringList headerLabels;
    headerLabels << "" << "Songtitle" << "Duration" << "Filepath";
    
    setHorizontalHeaderLabels(headerLabels);
}

void PlaylistModel::append(const QString &filename, unsigned int lenms)
{
    int row = rowCount();
    QString totalTime;
    
    if(checkFile(filename))
    {
        qDebug() << "Adding file : " << filename;
        totalTime.sprintf("%02d:%02d", lenms / 1000 / 60, lenms / 1000 % 60);
        setItem(row, Constants::TITLE, new QStandardItem(getFilenameOnly(filename)));
        setItem(row, Constants::DURATION, new QStandardItem(totalTime));
        setItem(row, Constants::FILENAME, new QStandardItem(filename));
        qDebug() << "File Added";
    }
}

void PlaylistModel::appendPlaylist(const QString &filename)
{
    MUIPlaylist::Playlist p;
    int length;
    QString len;
    std::string title, filepath, file;
    
    file = filename.toStdString();
    try {
        p.renderPlaylist(file);
        
        while(p.getNextEntry(length, title, filepath))
        {
            int row = rowCount();
            len.sprintf("%02d:%02d", length / 60, length % 60);
            setItem(row, Constants::TITLE, new QStandardItem(QString::fromStdString(title)));
            setItem(row, Constants::DURATION, new QStandardItem(len));
            setItem(row, Constants::FILENAME, new QStandardItem(QString::fromStdString(filepath)));
        }
    }
    catch(MUIPlaylist::PlaylistException &e) {
        qDebug() << QString::fromStdString(e.what());
    }
}

void PlaylistModel::savePlaylist(const QString &filename)
{
    MUIPlaylist::Playlist p;
    std::string title, path, len, file;
    int length, min, sec;
    
    file = filename.toStdString();
    
    try {
        p.createPlaylist(file, false);
            
        for(int i=0; i<rowCount(); i++)
        {
            title = QVariant(item(i, Constants::TITLE)->text()).toString().toStdString();
            len = QVariant(item(i, Constants::DURATION)->text()).toString().toStdString();
            path = QVariant(item(i, Constants::FILENAME)->text()).toString().toStdString();
            
            sscanf(len.c_str(), "%d:%d", &min, &sec);
            length = (min * 60) + sec;
            
            p.writeNextEntry(length, title, path);
        }
        p.endList();
    }
    catch(MUIPlaylist::PlaylistException &e) {
        qDebug() << QString::fromStdString(e.what());
    }
}

void PlaylistModel::clear()
{
    removeRows(0, rowCount());
}

int PlaylistModel::checkFile(QString filepath)
{
    const QString supportedFormats = "aiff asf flac fsb it mid midi mod mp2 mp3 ogg raw s3m xm";
    int l = filepath.length() - filepath.lastIndexOf(".") - 1;
    filepath = filepath.right(l);
    if (supportedFormats.contains(filepath, Qt::CaseInsensitive)) return 1;
    return 0;
}

QString PlaylistModel::getFilenameOnly(QString filepath) const
{
    filepath = filepath.section('/', -1);
    return (filepath.left(filepath.lastIndexOf(".")));
}
