
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

    bool setDac1( int dacA, dacB );
    bool setDac2( int dacA, dacB );

    bool osc_eaux( QVector<quint16> & vals );
    bool osc_eref( QVector<quint16> & vals );
    bool osc_iaux( QVector<quint16> & vals );
    bool osc_set_period( qreal secs, int per_pts );
    bool set_leds( int leds );
    bool set_out_relay( bool en );
    bool set_sc_relay( bool en );
    bool set_dac_raw( int dacLow, int dacHigh );
    bool set_one_pulse_raw( int dacLow, int dacHigh, qreal ms );
    bool set_meandr_raw( int dacLow1, int dacHigh1, qreal timeMs1, int dacLow2, int dacHigh2, qreal timeMs2 );
    bool set_sweep_raw( int dacLow1, int dacHigh1, int dacLow2, int dacHigh2, qreal timeMs2 );



    bool setArgs( quint8 * data, int dataSz );
    bool execFunc( quint8 funcId );
    int  readData( quint8 * data, int dataSz );
protected:
    int write( quint8 * data, int dataSz );
    int read( quint8 * data, int dataSz, bool & eom );

private:
    class PD;
    PD * pd;
};


#endif


