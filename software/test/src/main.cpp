
#include <QtGui>
#include <QApplication>
#include "voltamp_io.h"

#include "qextserialport.h"


int main( int argc, char * argv[] )
{
    QApplication app( argc, argv );

    VoltampIo io;
    bool res;
    QStringList l = io.enumDevices();
    qDebug() << l;
    res = io.open( 4 );
    if ( !res )
        return -1;
    QVector<quint16> data;
    QString stri;
    res = io.hardware_version( stri );
    qDebug() << stri;
    res = io.firmware_version( stri );
    qDebug() << stri;


    io.close();
    
    return 0;
}

