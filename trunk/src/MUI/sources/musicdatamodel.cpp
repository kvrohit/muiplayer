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
    case STATEICON:
        return QString("");
    case SONGTITLE:
        return QString("Songtitle");
    case ARTIST:
        return QString("Artist");
    case ALBUM:
        return QString("Album");
    case DURATION:
        return QString("Duration");
    case FILEPATH:
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
    // Handle the DecorationRole only for the STATEICON column
    if((index.column() == 0) && (role == Qt::DecorationRole)) {
        return QIcon(list.at(index.row())->icon);
    }

    switch(role) {
    case Qt::DisplayRole:
        return list.at(index.row())->valueAt(index.column());
    case FILEPATHROLE:
        return list.at(index.row())->filepath;
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

Qt::ItemFlags MusicDataModel::flags(const QModelIndex &index) const
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
                    appendData(it->next(), "00:00");
                    qApp->processEvents(QEventLoop::AllEvents);
                }

                delete it;
            }
            else {
                appendData(url.toLocalFile(), "00:00");
            }
        }
    }
    return true;
}

void MusicDataModel::appendData(const QString &filepath, const QString &totaltime)
{
    FMOD::Player p;
    if(!isSupportedFormat(filepath)) {
        return;
    }

    int row = rowCount();
    AudioTag::TagReader reader;
    AudioTag::GenericTag tag;

    try {
        reader.renderFile(filepath.toStdString());
        tag = reader.getTag();

        beginInsertRows(QModelIndex(), row, row);
        list.insert(row, new MusicData(convertToUnicode(tag.title),
                                       convertToUnicode(tag.album),
                                       convertToUnicode(tag.artist),
                                       filepath,
                                       formatTimeToQString(p.getLengthFromName(filepath.toStdString())))
                    );
        endInsertRows();
    }
    catch(AudioTag::TagException &ex) {
        qDebug() << QString::fromStdString(ex.what());
    }
}

void MusicDataModel::appendData(const MusicData &data)
{
    int row = rowCount();
    beginInsertRows(QModelIndex(), row, row);
    list.insert(row, new MusicData(data));
    endInsertRows();
}

void MusicDataModel::updateIcon(int row, IconState newState)
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
