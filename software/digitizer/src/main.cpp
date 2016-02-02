
#include <QtGui>
#include <QApplication>
#include "host_tray.h"

int main( int argc, char * argv[])
{
    QStringList paths = QApplication::libraryPaths();
    paths.append(".");
    paths.append("imageformats");
    paths.append("platforms");
    paths.append("sqldrivers");
    QApplication::setLibraryPaths( paths );

    QApplication app( argc, argv );

    HostTray * ht = new HostTray( 0 );
    ht->show();

    int res = app.exec();
    return res;
}




