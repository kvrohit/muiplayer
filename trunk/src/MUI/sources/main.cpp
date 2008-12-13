#include <QtGui/QApplication>
#include "mui.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MUI w;
    w.show();
    return a.exec();
}
