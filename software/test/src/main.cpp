
#include <QtGui>
#include <QApplication>
#include "voltamp_io.h"
#include <iostream>

#include "qextserialport.h"


int main( int argc, char * argv[] )
{
    QApplication app( argc, argv );

    QTime t;
    t.start();
    VoltampIo io;
    bool res;
    std::cout << "enum\n";
    qDebug() << t;
    t.start();
    QStringList l = io.enumDevices();
    qDebug() << t;
    t.start();
    //qDebug() << l;
    std::cout << "open\n";
    qDebug() << t;
    t.start();
    res = io.open( 4 );
    qDebug() << t;
    t.start();
    if ( !res )
        return -1;

    std::cout << "opened!\n";

    qDebug() << t;
    t.start();
    res = io.firmwareUpgrade( "./digitizer.bin" );
    qDebug() << t;
    t.start();

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

