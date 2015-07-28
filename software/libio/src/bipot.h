
#ifndef __BIPOT_H_
#define __BIPOT_H_

#include <QtCore>

class Bipot
{
public:
    Bipot();
    ~Bipot();

    // Opening. closing the hardware.
    bool open();
    void close();

    // Simple operations and oscilloscope.
    bool setWorkRaw( int a, int b );
    bool setWorkMv( qreal mv );
    bool setProbeRaw( int a, int b );
    bool setProbeMv( qreal mv );
    bool setOscPeriod( int ptsCnt, qreal msTotal );
    bool setOscSigs( bool workV=true, bool probeV=false, bool workI=false, bool probeI=false );
    bool oscData( QVector<qreal> & workV, QVector<qreal> & probeV, QVector<qreal> & workI, QVector<qreal> & probeI );
    bool instantDataRaw( int & workV, int & probeV, int & workI, int & probeI );
    bool instantData( qreal & workV, qreal & probeV, qreal & workI, qreal & probeI );

    // Some active movements.
    bool sweepWork( qreal from, qreal to, qreal ms );
    bool triggerSweepWork( qreal from, qreal to, int ptsCnt );

    // Calibrations.
    void setmV2mA( qreal workA, qreal workB, qreal probeA, qreal probeB );

    void clearCalibrationWorkDac();
    bool loadCalibrationWorkDac( const QString & fileName = "./work_dac.dat" );
    bool saveCalibrationWorkDac( const QString & fileName = "./work_dac.dat" );
    void addCalibrationWorkDac( int dacA, int DacB, qreal mV );

    void clearCalibrationProbeDac();
    bool loadCalibrationProbeDac( const QString & fileName = "./probe_dac.dat" );
    bool saveCalibrationProbeDac( const QString & fileName = "./probe_dac.dat" );
    void addCalibrationProbeDac( int dacA, int DacB, qreal mV );

    void clearCalibrationAdc();
    bool loadCalibrationAdc( const QString & fileName = "./adc.dat" );
    bool saveCalibrationAdc( const QString & fileName = "./adc.dat" );
    void addCalibrationAdc( qreal mv0, qreal mv1, qreal mv2, qreal mv3 );

    bool calibrationCalc();
    bool calibrationLoad( const QString & fileName = "./claibration.dat" );
    bool calibrationSave( const QString & fileName = "./claibration.dat" );
private:
    class PD;
    PD * pd;
};



#endif






