#include "filesystembrowser.h"

FileSystemBrowser::FileSystemBrowser(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *vBox = new QVBoxLayout(this);

    fileView = new QTreeView(this);
    fileModel = new QFileSystemModel(this);

    fileModel->setRootPath("/");
    fileModel->sort(0, Qt::AscendingOrder);
    fileModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);

    fileView->setModel(fileModel);
    fileView->hideColumn(1);
    fileView->hideColumn(2);
    fileView->hideColumn(3);
    fileView->setDragEnabled(true);
    fileView->setHeaderHidden(true);

    vBox->setContentsMargins(0, 0, 0, 0);
    vBox->addWidget(fileView);
    setLayout(vBox);
}
