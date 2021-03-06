#include "musicdatamodel.h"

MusicDataModel::MusicDataModel(QObject *parent) :
    QAbstractTableModel(parent) {
    tagReader = new TagReader(this);

    connect(tagReader, SIGNAL(metaDataAvailable(Meta::AudioTag)),
            this, SLOT(metaDataAvailable(Meta::AudioTag)));
}

QVariant MusicDataModel::headerData(int section, Qt::Orientation orientation,
                                    int role) const {
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal) {
        return QVariant();
    }

    switch (section) {
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
}

int MusicDataModel::columnCount(const QModelIndex & /* parent */) const {
    return COLUMNCOUNT;
}

int MusicDataModel::rowCount(const QModelIndex & /* parent */) const {
    return list.count();
}

QVariant MusicDataModel::data(const QModelIndex &index, int role) const {
    int nRow = index.row();
    int nColumn = index.column();

    // Handle the DecorationRole only for the STATEICON column
    //if ((nColumn == 0) && (role == Qt::DecorationRole)) {
    //    return QIcon(list.at(nRow)->icon);
    //}

    switch (role) {
    case Qt::DisplayRole:
        return list.at(nRow)->valueAt(nColumn);
    case FILEPATHROLE:
        return list.at(nRow)->tag.filepath;
    default:
        return QVariant();
    }
}

bool MusicDataModel::insertRows(int row, int count, const QModelIndex &parent) {
    beginInsertRows(parent, row, row + count - 1);
    for(int i = 0; i < count; ++i) {
        list.insert(row, new MusicData());
    }
    endInsertRows();
    return true;
}

bool MusicDataModel::removeRows(int row, int count, const QModelIndex &parent) {
    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        list.removeAt(row);
    }
    endRemoveRows();
    return true;
}

Qt::ItemFlags MusicDataModel::flags(const QModelIndex &/*index*/) const {
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled;
}

Qt::DropActions MusicDataModel::supportedDropActions() const {
    qDebug() << "supportedDropActions()";
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList MusicDataModel::mimeTypes() const {
    qDebug() << "mimeTypes()";
    QStringList mimeList;
    mimeList.append("text/uri-list");
    mimeList.append("text/plain");
    return mimeList;
}

bool MusicDataModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
                                  int /* row */, int /* column */,
                                  const QModelIndex & /* parent */) {
    QString filepath;
    QFileInfo f;
    if (action == Qt::IgnoreAction) {
        return true;
    }

    if (data->hasUrls()) {
        foreach (QUrl url, data->urls()) {
            filepath = url.toLocalFile();
            f.setFile(filepath);

            if (f.isDir()) {
                QDirIterator *it = new QDirIterator(
                            filepath, QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot,
                            QDirIterator::Subdirectories);

                while (it->hasNext()) {
                    appendData(it->next());
                    qApp->processEvents(QEventLoop::AllEvents);
                }

                delete it;
            }
            else {
                appendData(filepath);
            }
        }
    }
    return true;
}

void MusicDataModel::appendData(const QString &filepath)
{
    if (!isSupportedFormat(filepath)) {
        return;
    }

    QEventLoop loop;
    loop.connect(tagReader, SIGNAL(metaDataAvailable(Meta::AudioTag)), SLOT(quit()));
    tagReader->readTag(filepath);
    loop.exec();
}

void MusicDataModel::appendData(const MusicData &data) {
    int row = rowCount();
    beginInsertRows(QModelIndex(), row, row);
    list.insert(row, new MusicData(data));
    endInsertRows();
}

void MusicDataModel::updateIcon(int row, Mui::IconState newState) {
    //list[row]->setIconState(newState);
    emit dataChanged(index(row, 0), index(row, COLUMNCOUNT));
}

void MusicDataModel::load() {
    QString filepath = QCoreApplication::applicationDirPath();
    filepath.append(Mui::ModelDataFileName);

    QFile file(filepath);
    MusicData d;

    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QDataStream in(&file);
    qint32 magicNumber;
    in.setVersion(QDataStream::Qt_4_5);

    in >> magicNumber;

    if (magicNumber != Mui::MagicNumber) {
        qDebug() << "Unsupported file format";
        file.close();
        return;
    }

    while (!in.atEnd()) {
        in >> d.tag.title >> d.tag.artist >> d.tag.album >> d.tag.duration >> d.tag.filepath;
        appendData(d);
    }

    file.close();
}

void MusicDataModel::save() {
    QString filepath = QCoreApplication::applicationDirPath();
    filepath.append(Mui::ModelDataFileName);

    QFile file(filepath);

    if (!file.open(QIODevice::WriteOnly)) {
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_5);

    out << Mui::MagicNumber;

    for (int i = 0; i < list.count(); ++i) {
        out << list[i]->tag.title << list[i]->tag.artist << list[i]->tag.album <<
               list[i]->tag.duration << list[i]->tag.filepath;
    }

    file.close();
}

void MusicDataModel::resetData() {
    list.clear();
    qDeleteAll(list);
}

bool MusicDataModel::isSupportedFormat(const QString &filepath) {
    QString suffix = QFileInfo(filepath).suffix();
    if (Mui::SupportedFormats.contains(suffix, Qt::CaseInsensitive)) {
        return true;
    }
    return false;
}

void MusicDataModel::metaDataAvailable(const Meta::AudioTag &tag) {
    int row = rowCount();
    beginInsertRows(QModelIndex(), row, row);
    list.insert(row, new MusicData(tag));
    endInsertRows();
}

Meta::AudioTag MusicDataModel::audioTagAtIndex(const QModelIndex &index) const {
    MusicData *d = list.at(index.row());
    return d->tag;
}

MusicDataModel::~MusicDataModel() {
    qDeleteAll(list);
    delete tagReader;
}
