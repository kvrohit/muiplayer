#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QStandardItemModel>
#include <QString>
#include <QStandardItem>
#include <QDebug>

#include "cplaylist.hpp"
#include "constants.h"

class PlaylistModel : public QStandardItemModel
{
    public:
        PlaylistModel();
        void append(const QString &, unsigned int);
        void appendPlaylist(const QString &);
        void savePlaylist(const QString &);
        void clear();
        
    private:
        int checkFile(QString);
        QString getFilenameOnly(QString) const;
};

#endif
