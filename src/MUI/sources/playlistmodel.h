#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QStandardItemModel>
#include <QString>
#include <QStandardItem>
#include <QMessageBox>

#include "cplaylist.hpp"

#define TITLE 0
#define DURATION 1
#define FILENAME 2

class PlaylistModel : public QStandardItemModel
{
    public:
        PlaylistModel();
        void append(const QString &);
        void appendPlaylist(const QString &);
        void savePlaylist(const QString &);
        void clear();
        
    private:
        int checkFile(QString);
        QString getFilenameOnly(QString) const;
};

#endif
