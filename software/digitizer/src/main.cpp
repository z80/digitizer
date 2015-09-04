
#include <QtGui>
#include <QApplication>
#include "host_tray.h"

int main( int argc, char * argv[])
{
    QApplication app( argc, argv );

    HostTray * ht = new HostTray( 0 );
    ht->show();

    int res = app.exec();
    return res;
}




