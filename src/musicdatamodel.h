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
#include "metadata.h"
#include "tagreader.h"
#include "globals.h"

const int COLUMNCOUNT = 6;
const int FILEPATHROLE = Qt::UserRole + 1;

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
    void appendData(const MusicData &data);
    void updateIcon(int row, Mui::IconState newState);
    void resetData();

    void load();
    void save();

    Meta::AudioTag audioTagAtIndex(const QModelIndex &) const;

    ~MusicDataModel();

signals:

public slots:
    void metaDataAvailable(const Meta::AudioTag &);

private:
    QList<MusicData*> list;
    bool isSupportedFormat(const QString &filepath);
    TagReader *tagReader;
};

#endif // MUSICDATAMODEL_H
