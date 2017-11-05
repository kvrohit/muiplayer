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
    ui.labelQtVersion->setText(qtVer.append(qVersion()));
}
