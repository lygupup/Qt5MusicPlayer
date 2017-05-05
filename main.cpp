#include "iceplayer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::addLibraryPath("./plugins");
    QApplication a(argc, argv);
    IcePlayer w;
    w.show();

    return a.exec();
}
