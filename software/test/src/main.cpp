
#include <QtGui>
#include <QApplication>
#include "voltamp_io.h"
#include <iostream>

#include "qextserialport.h"


int main( int argc, char * argv[] )
{
    QApplication app( argc, argv );

    VoltampIo io;
    bool res;
    std::cout << "enum\n";
    QStringList l = io.enumDevices();
    //qDebug() << l;
    std::cout << "open\n";
    res = io.open( 4 );
    if ( !res )
        return -1;

    std::cout << "opened!\n";

    res = io.firmwareUpgrade( "./digitizer.bin" );

    std::cout << "res: " << (res ? "ok" : "failed");

    /*
    quint8 v;
    bool eom;
    v = 234;
    int cnt;
    cnt = io.write( &v, 1 );
    cnt = io.read( &v, 1, eom );
    qDebug() << v;

    for ( v=0; v<30; v++ )
    {
        cnt = io.write( &v, 1 );
        cnt = io.read( &v, 1, eom );
        qDebug() << v;
    }
    */

    //QString stri;

    //QVector<int> data;
    //res = io.setTriggerEn( true );
    //res = io.setDac1( 32767, 32767 );
    //res = io.setDac1( 32767, 41000 );
    //res = io.setDac1( 32767, 32767 );
    //res = io.setDac1( 32767, 41000 );
    //res = io.setDac1( 32767, 32767 );
    //res = io.setDac1( 32767, 41000 );
    //res = io.sweepData( data );

    io.close();
    
    return 0;
}

