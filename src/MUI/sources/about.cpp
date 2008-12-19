#include "about.h"

About::About()
{
    ui.setupUi(this);
    ui.labelCFMODVersion->setText(QString::fromStdString(FMOD::libversion));
    ui.labelCPlaylistVersion->setText(QString::fromStdString(MUIPlaylist::libversion));
    QString qtVer = "Using Qt ";
    ui.labelQtVersion->setText(qtVer.append(qVersion()));
}
