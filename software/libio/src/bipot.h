
#ifndef __BIPOT_H_
#define __BIPOT_H_

#include <QtCore>

class Bipot
{
public:
    Bipot();
    ~Bipot();

    // Opening. closing the hardware.
    QStringList deviceList();
    bool open( int index = 0 );
    bool isOpen();
    void close();

    QString hardwareVersion();
    QString firmwareVersion();

    // Simple operations and oscilloscope.
    bool setWorkRaw( int a, int b );
    bool setWorkMv( qreal mv );
    bool setProbeRaw( int a, int b );
    bool setProbeMv( qreal mv );
    bool setOscPeriod( int ptsCnt, qreal msTotal );
    //bool setOscSigs( bool workV=true, bool probeV=false, bool workI=false, bool probeI=false );
    bool oscData( QVector<qreal> & workV, QVector<qreal> & probeV, QVector<qreal> & workI, QVector<qreal> & probeI );
    bool instantDataRaw( int & workV, int & probeV, int & workI, int & probeI );
    bool instantData( qreal & workV, qreal & probeV, qreal & workI, qreal & probeI );
    bool temperature( qreal & t );

    // Capture by trigger.
    bool setTriggerEn( bool en );

    // Some active movements.
    bool setSweepRange( qreal workV, qreal probeV );
    bool setSweepTime( int ptsCnt, qreal periodMs );
    bool setSweepEn( bool en );
    bool sweepEn( bool & en );
    bool sweepData( QVector<qreal> & workV, QVector<qreal> & probeV, QVector<qreal> & workI, QVector<qreal> & probeI );

    bool setOutput( int o );

    bool runBootloader();
    bool bootloaderHardwareVersion( QString & stri );
    bool bootloaderFirmwareVersion( QString & stri );
    bool firmwareUpgrade( const QString & fileName );


    // Calibrations.
    void setmV2mA( qreal workA, qreal workB, qreal probeA, qreal probeB );
    void setVoltScale( qreal scaleWork, qreal scaleProbe );
    void setTemperature( qreal temp ); // Yes, thermal compensation. So what?

    void clearCalibrationWorkDac();
    bool loadCalibrationWorkDac( const QString & fileName = "./work_dac.dat" );
    bool saveCalibrationWorkDac( const QString & fileName = "./work_dac.dat" );
    bool addCalibrationWorkDac( int dacA, int dacB, int dacC, int dacD, qreal mV );
    void setCalibrationWorkDac( qreal a1, qreal a2, qreal b );

    void clearCalibrationProbeDac();
    bool loadCalibrationProbeDac( const QString & fileName = "./probe_dac.dat" );
    bool saveCalibrationProbeDac( const QString & fileName = "./probe_dac.dat" );
    bool addCalibrationProbeDac( int dacA, int dacB, int dacC, int dacD, qreal mV );
    void setCalibrationProbeDac( qreal a1, qreal a2, qreal b );

    void clearCalibrationAdc();
    bool loadCalibrationAdc( const QString & fileName = "./adc.dat" );
    bool saveCalibrationAdc( const QString & fileName = "./adc.dat" );
    bool addCalibrationAdc( qreal mv0, qreal mv1, qreal mv2, qreal mv3 );

    bool calibrationCalc();
    bool calibrationLoad( const QString & fileName = "./calibration.dat" );
    bool calibrationSave( const QString & fileName = "./calibration.dat" );
private:
    class PD;
    PD * pd;
};



#endif






