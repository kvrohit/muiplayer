#include "musicdatamodel.h"

MusicDataModel::MusicDataModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

QVariant MusicDataModel::headerData(int section, Qt::Orientation orientation,
                                    int role) const
{
    if(role != Qt::DisplayRole || orientation != Qt::Horizontal) {
        return QVariant();
    }

    switch(section) {
    case Mui::STATEICON:
        return QString("");
    case Mui::SONGTITLE:
        return QString("Songtitle");
    case Mui::ARTIST:
        return QString("Artist");
    case Mui::ALBUM:
        return QString("Album");
    case Mui::DURATION:
        return QString("Duration");
    case Mui::FILEPATH:
        return QString("Filepath");
    default:
        return QVariant();
    }
};

int MusicDataModel::columnCount(const QModelIndex & /* parent */) const
{
    return COLUMNCOUNT;
}

int MusicDataModel::rowCount(const QModelIndex & /* parent */) const
{
    return list.count();
}

QVariant MusicDataModel::data(const QModelIndex &index, int role) const
{
    int nRow = index.row();
    int nColumn = index.column();

    // Handle the DecorationRole only for the STATEICON column
    if((nColumn == 0) && (role == Qt::DecorationRole)) {
        return QIcon(list.at(nRow)->icon);
    }

    switch(role) {
    case Qt::DisplayRole:
        return list.at(nRow)->valueAt(nColumn);
    case FILEPATHROLE:
        return list.at(nRow)->filepath;
    default:
        return QVariant();
    }
}

bool MusicDataModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    for(int i = 0; i < count; ++i) {
        list.insert(row, new MusicData());
    }
    endInsertRows();
    return true;
}

bool MusicDataModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    for(int i = 0; i < count; ++i) {
        list.removeAt(row);
    }
    endRemoveRows();
    return true;
}

Qt::ItemFlags MusicDataModel::flags(const QModelIndex &/*index*/) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled;
}

Qt::DropActions MusicDataModel::supportedDropActions() const
{
    qDebug() << "supportedDropActions()";
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList MusicDataModel::mimeTypes() const
{
    qDebug() << "mimeTypes()";
    QStringList mimeList;
    mimeList.append("text/uri-list");
    mimeList.append("text/plain");
    return mimeList;
}

bool MusicDataModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
                                  int /* row */, int /* column */,
                                  const QModelIndex & /* parent */)
{
    QString filepath;
    QFileInfo f;
    if(action == Qt::IgnoreAction) {
        return true;
    }

    if(data->hasUrls()) {
        foreach(QUrl url, data->urls()) {
            filepath = url.toLocalFile();
            f.setFile(filepath);

            if(f.isDir()) {
                QDirIterator *it = new QDirIterator(
                            filepath, QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot,
                            QDirIterator::Subdirectories);

                while(it->hasNext()) {
                    appendData(it->next());
                    qApp->processEvents(QEventLoop::AllEvents);
                }

                delete it;
            }
            else {
                appendData(url.toLocalFile());
            }
        }
    }
    return true;
}

void MusicDataModel::appendData(const QString &filepath)
{
    FMOD::Player p;
    if(!isSupportedFormat(filepath)) {
        return;
    }

    int row = rowCount();
    AudioTag::TagReader reader;
    AudioTag::GenericTag tag;
    quint32 lenms;

    try {
        reader.renderFile(filepath.toStdString());
        tag = reader.getTag();
        lenms = p.getLengthFromName(filepath.toStdString());

        beginInsertRows(QModelIndex(), row, row);
        list.insert(row, new MusicData(Mui::convertToUnicode(tag.title),
                                       Mui::convertToUnicode(tag.album),
                                       Mui::convertToUnicode(tag.artist),
                                       filepath,
                                       Mui::formatTimeToQString(lenms)));
        endInsertRows();
    }
    catch(AudioTag::TagException &ex) {
        qDebug() << QString::fromStdString(ex.what());
    }
}

void MusicDataModel::appendPlaylist(const QString &filepath)
{
    MUIPlaylist::Playlist p;
    int length;
    std::string strTitle, strFilepath;

    try {
        p.renderPlaylist(filepath.toStdString());

        while(p.getNextEntry(length, strTitle, strFilepath)) {
            appendData(QString::fromStdString(strFilepath));
        }
    }
    catch(MUIPlaylist::PlaylistException &e) {
        qDebug() << QString::fromStdString(e.what());
    }
}

void MusicDataModel::appendData(const MusicData &data)
{
    int row = rowCount();
    beginInsertRows(QModelIndex(), row, row);
    list.insert(row, new MusicData(data));
    endInsertRows();
}

void MusicDataModel::updateIcon(int row, Mui::IconState newState)
{
    list[row]->setIconState(newState);
    emit dataChanged(index(row, 0), index(row, COLUMNCOUNT));
}

void MusicDataModel::resetData()
{
    list.clear();
    qDeleteAll(list);
    reset();
}

bool MusicDataModel::isSupportedFormat(const QString &filepath)
{
    QString suffix = QFileInfo(filepath).suffix();
    if(SUPPORTEDFORMATS.contains(suffix, Qt::CaseInsensitive)) {
        return true;
    }
    return false;
}

MusicDataModel::~MusicDataModel()
{
    qDeleteAll(list);
}
