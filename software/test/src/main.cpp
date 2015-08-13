
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

    QVector<quint16> data;
    QString stri;
    res = io.hardware_version( stri );
    qDebug() << stri;
    res = io.firmware_version( stri );
    qDebug() << stri;
    int adc[4];
    for ( int i=0; i<64; i++ )
    {
        res = io.instantAdc( adc );
        QString s0= QString( "0x%1" ).arg( adc[0] , 8, 16, QChar( '0' ) );
        QString s1= QString( "0x%1" ).arg( adc[1] , 8, 16, QChar( '0' ) );
        QString s2= QString( "0x%1" ).arg( adc[2] , 8, 16, QChar( '0' ) );
        QString s3= QString( "0x%1" ).arg( adc[3] , 8, 16, QChar( '0' ) );
        qDebug() << s0 << s1 << s2 << s3;
    }

    io.close();
    
    return 0;
}

