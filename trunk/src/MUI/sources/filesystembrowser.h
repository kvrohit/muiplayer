#ifndef FILESYSTEMBROWSER_H
#define FILESYSTEMBROWSER_H

#include <QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QVBoxLayout>

class FileSystemBrowser : public QWidget
{
Q_OBJECT
public:
    explicit FileSystemBrowser(QWidget *parent = 0);

signals:

private:
    QFileSystemModel *fileModel;
    QTreeView *fileView;
};

#endif // FILESYSTEMBROWSER_H
