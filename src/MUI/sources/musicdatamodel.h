#ifndef MUSICDATAMODEL_H
#define MUSICDATAMODEL_H

#include <QCoreApplication>
#include <QString>
#include <QIcon>
#include <QAbstractTableModel>
#include <QList>
#include <QDebug>
#include <QMimeData>
#include <QUrl>
#include <QFileInfo>
#include <QDirIterator>

#include "musicdata.h"
#include "caudiotags.hpp"
#include "cfmod.hpp"
#include "cplaylist.hpp"

const int COLUMNCOUNT = 6;
const int FILEPATHROLE = Qt::UserRole + 1;
const QString SUPPORTEDFORMATS = "aiff asf flac fsb it mid midi mod mp2 mp3 ogg raw s3m xm";

class MusicDataModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MusicDataModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool insertRows(int row, int count, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    Qt::DropActions supportedDropActions() const;
    QStringList mimeTypes() const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row,
                      int column, const QModelIndex &parent);

    void appendData(const QString &filepath);
    void appendPlaylist(const QString &filepath);
    void appendData(const MusicData &data);
    void updateIcon(int row, Mui::IconState newState);
    void resetData();

    ~MusicDataModel();

signals:

public slots:

private:
    QList<MusicData*> list;

    bool isSupportedFormat(const QString &filepath);

};

#endif // MUSICDATAMODEL_H
