#include "about.h"

About::About(QWidget *parent) : QDialog(parent)
{
    QString qtVer = "Using Qt ";

    ui.setupUi(this);

    QStringList componentHeaderLabels;
    componentHeaderLabels << "Component" << "Version";
    model.setHorizontalHeaderLabels(componentHeaderLabels);

    ui.treeView->setModel(&model);

    // base
    model.setItem(0, 0, new QStandardItem("MUI base"));
    model.setItem(0, 1, new QStandardItem(Mui::MUIVersion));
    ui.labelMUIVersion->setText(QString("Version %1").arg(Mui::MUIVersion));

    // cfmod
    model.setItem(1, 0, new QStandardItem("cfmod"));
    model.setItem(1, 1, new QStandardItem(QString::fromStdString(FMOD::libversion)));

    // cplaylist
    model.setItem(2, 0, new QStandardItem("cplaylist"));
    model.setItem(2, 1, new QStandardItem(QString::fromStdString(MUIPlaylist::libversion)));

    //caudiotags
    model.setItem(3, 0, new QStandardItem("caudiotags"));
    model.setItem(3, 1, new QStandardItem(QString::fromStdString(AudioTag::libversion)));

    ui.labelQtVersion->setText(qtVer.append(qVersion()));
}
