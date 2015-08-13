
#ifndef __VOLTAMP_IO_H_
#define __VOLTAMP_IO_H_

#include <QtCore>

class VoltampIo
{
public:
    VoltampIo();
    ~VoltampIo();

    QStringList enumDevices();
    bool open( int index = 0 );
    bool isOpen() const;
    void close();

    bool hardware_version( QString & stri );
    bool firmware_version( QString & stri );

    bool setLed( int leds );
    bool setDac1( int dacA, int dacB );
    bool setDac2( int dacA, int dacB );
    bool instantAdc( int * data );

    bool setOscSignals( bool * en );
    bool setOscPeriod( int ptsCnt, qreal periodMs );
    bool oscData( QVector<int> & data );

    bool setArgs( quint8 * data, int dataSz );
    bool execFunc( quint16 funcId );
    int  readData( quint8 * data, int dataSz );
public:
    int write( quint8 * data, int dataSz );
    int read( quint8 * data, int dataSz, bool & eom );

private:
    class PD;
    PD * pd;
};


#endif


