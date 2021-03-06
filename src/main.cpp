#include <QtWidgets/QApplication>
#include "mui.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("MUI");
    a.addLibraryPath(QCoreApplication::applicationDirPath());
    MUI w;
    w.setWindowIcon(QIcon(Mui::MediaPlaybackIcon));
    w.show();
    return a.exec();
}
