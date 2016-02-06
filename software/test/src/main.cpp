
#include <QtGui>
#include <QApplication>
#include "io.h"


int main( int argc, char * argv[] )
{
    QApplication app( argc, argv );

    Io io;
    QStringList l = io.enumDevices();
    qDebug() << l;

    bool res = io.open( 0 );
    io.close();

    return 0;
}

