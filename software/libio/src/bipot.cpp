
#include "bipot.h"
#include "voltamp_io.h"
#include <QMessageBox>

struct CalibrationDac
{
    int   dacA;
    int   dacB;
    int   dacC;
    int   dacD;

    qreal temp;

    qreal volt;
};

struct CalibrationAdc
{
    qreal adcA;
    qreal adcB;
    qreal adcC;
    qreal adcD;

    qreal temp;

    qreal voltA;
    qreal voltB;
    qreal voltC;
    qreal voltD;
};

struct CoefDac
{
    qreal a1; 
    qreal a2;
    qreal at;
    qreal b;
};

class Bipot::PD
{
public:
    PD() {}
    ~PD() {}

    qreal adc2workV( int adc );
    qreal adc2probeV( int adc );
    qreal adc2workI( int adc );
    qreal adc2probeI( int adc );

    void  workV2Dac( qreal workV, int & dac1, int & dac2 );
    void  probeV2Dac( qreal workV, int & dac1, int & dac2 );

    VoltampIo  * io;
    QMutex       mutex;
    bool         sigs[4];
    QVector<int> data;

    qreal workA, workB, probeA, probeB;
    qreal workGain, probeGain;

    QList<CalibrationDac> clbrDacWork, 
                          clbrDacProbe;
    QList<CalibrationAdc> clbrAdc;

    CoefDac workDac;
    CoefDac probeDac;
    qreal   temperature;

    static const int MEASURES_CNT;
};

const int Bipot::PD::MEASURES_CNT = 128;

qreal Bipot::PD::adc2workV( int adc )
{
    qreal res = static_cast< qreal >( adc );
    return res;
}

qreal Bipot::PD::adc2probeV( int adc )
{
    qreal res = static_cast< qreal >( adc );
    return res;
}

qreal Bipot::PD::adc2workI( int adc )
{
    qreal res = static_cast< qreal >( adc );
    res = res * workA + workB;
    return res;
}

qreal Bipot::PD::adc2probeI( int adc )
{
    qreal res = static_cast< qreal >( adc );
    res = res * probeA + probeB;
    return res;
}

void  Bipot::PD::workV2Dac( qreal workV, int & dac1, int & dac2 )
{
    qreal aDacLow  = workDac.a1;
    qreal aDacHigh = workDac.a2;
    qreal bDac     = workDac.b;
    qreal temp     = temperature; // Also should be included into consideration.

    qreal fLow = 32767.0;
    qreal fHigh = ceil( (workV - bDac - fLow * aDacLow ) / aDacHigh - 0.5 );
    fLow = ceil( (workV - bDac - fHigh*aDacHigh)/aDacLow - 0.5 );
    int dacLow  = static_cast<int>( fLow );
    int dacHigh = static_cast<int>( fHigh );

    dac1 = dacLow;
    dac2 = dacHigh;
}

void  Bipot::PD::probeV2Dac( qreal probeV, int & dac1, int & dac2 )
{
    qreal aDacLow  = probeDac.a1;
    qreal aDacHigh = probeDac.a2;
    qreal bDac     = probeDac.b;
    qreal temp     = temperature; // Also should be included into consideration.

    qreal fLow = 32767.0;
    qreal fHigh = ceil( (probeV - bDac - fLow * aDacLow ) / aDacHigh - 0.5 );
    fLow = ceil( (probeV - bDac - fHigh*aDacHigh)/aDacLow - 0.5 );
    int dacLow  = static_cast<int>( fLow );
    int dacHigh = static_cast<int>( fHigh );

    dac1 = dacLow;
    dac2 = dacHigh;
}


Bipot::Bipot()
{
    pd = new PD();
    VoltampIo * io = new VoltampIo();
    pd->io = io;

    pd->workA  = 1.0;
    pd->workB  = 0.0;
    pd->probeA = 1.0;
    pd->probeB = 0.0;
    pd->workGain  = 1.0;
    pd->probeGain = 1.0;

    pd->workDac.a1 = 0.3741271/8000.0;
    pd->workDac.a2 = 0.3741271;
    pd->workDac.b  = -11980.84;

    pd->probeDac.a1 = 0.3741271/8000.0;
    pd->probeDac.a2 = 0.3741271;
    pd->probeDac.b  = -11980.84;

    pd->temperature = 23.0;

    pd->sigs[0] = pd->sigs[1] = pd->sigs[2] = pd->sigs[3] = true;
}

Bipot::~Bipot()
{
    delete pd->io;
    delete pd;
}

QStringList Bipot::deviceList()
{
    VoltampIo & io = *(pd->io);
    QStringList l = io.enumDevices();
    return l;
}

bool Bipot::open( int index )
{
    VoltampIo & io = *(pd->io);
    bool res = io.open( index );
    return res;
}

bool Bipot::isOpen()
{
    VoltampIo & io = *(pd->io);
    bool res = io.isOpen();
    return res;
}

void Bipot::close()
{
    VoltampIo & io = *(pd->io);
    io.close();
}

QString Bipot::hardwareVersion()
{
    VoltampIo & io = *(pd->io);
    QString stri;
    bool res = io.hardware_version( stri );    
    if ( !res )
        return QString();
    return stri;
}

QString Bipot::firmwareVersion()
{
    VoltampIo & io = *(pd->io);
    QString stri;
    bool res = io.firmware_version( stri );    
    if ( !res )
        return QString();
    return stri;
}

bool Bipot::setWorkRaw( int a, int b )
{
    VoltampIo & io = *(pd->io);
    bool res = io.setDac1( a, b );
    return res;
}

bool Bipot::setWorkMv( qreal mv )
{
    qreal aDacLow  = pd->workDac.a1;
    qreal aDacHigh = pd->workDac.a2;
    qreal bDac     = pd->workDac.b;

    qreal fLow = 32767.0;
    qreal fHigh = ceil( (mv - bDac - fLow * aDacLow ) / aDacHigh - 0.5 );
    fLow = ceil( (mv - bDac - fHigh*aDacHigh)/aDacLow - 0.5 );
    int dacLow  = static_cast<int>( fLow );
    int dacHigh = static_cast<int>( fHigh );

    bool res = setWorkRaw( dacLow, dacHigh );
    return res;
}

bool Bipot::setProbeRaw( int a, int b )
{
    VoltampIo & io = *(pd->io);
    bool res = io.setDac2( a, b );
    return res;
}

bool Bipot::setProbeMv( qreal mv )
{
    qreal aDacLow  = pd->probeDac.a1;
    qreal aDacHigh = pd->probeDac.a2;
    qreal bDac     = pd->probeDac.b;

    qreal fLow = 32767.0;
    qreal fHigh = ceil( (mv - bDac - fLow * aDacLow ) / aDacHigh - 0.5 );
    fLow = ceil( (mv - bDac - fHigh*aDacHigh)/aDacLow - 0.5 );
    int dacLow  = static_cast<int>( fLow );
    int dacHigh = static_cast<int>( fHigh );

    bool res = setProbeRaw( dacLow, dacHigh );
    return res;
}

bool Bipot::setOscPeriod( int ptsCnt, qreal msTotal )
{
    VoltampIo & io = *(pd->io);
    bool res = io.setOscPeriod( ptsCnt, msTotal );
    return res;
}

/*
bool Bipot::setOscSigs( bool workV, bool probeV, bool workI, bool probeI )
{
    VoltampIo & io = *(pd->io);
    pd->mutex.lock();
        bool * en = pd->sigs;
        en[0] = workV;
        en[1] = probeV;
        en[2] = workI;
        en[3] = probeI;
    pd->mutex.unlock();
    bool res = io.setOscSignals( en );
    return res;
}
*/

bool Bipot::oscData( QVector<qreal> & workV, QVector<qreal> & probeV, QVector<qreal> & workI, QVector<qreal> & probeI )
{
    VoltampIo & io = *(pd->io);
    pd->mutex.lock();
        bool en[4];
        en[0] = pd->sigs[0];
        en[1] = pd->sigs[1];
        en[2] = pd->sigs[3];
        en[3] = pd->sigs[2];
    pd->mutex.unlock();
    QVector<int> & dataRaw = pd->data;
    QVector<qreal> * data[4];
    workV.clear();
    probeV.clear();
    workI.clear();
    probeI.clear();
    data[0] = &workV;
    data[1] = &probeV;
    data[3] = &workI;
    data[2] = &probeI;
    bool res = io.oscData( dataRaw );
    if ( !res )
        return false;
    // Unit convertion is to be applied here.
    int ind = 0;
    
    for ( QVector<int>::const_iterator i=dataRaw.begin(); i!= dataRaw.end(); i++ )
    {
        if ( en[ind] )
        {
            int adc = *i;
            qreal v;
            switch ( ind )
            {
            case 0:
                v = pd->adc2workV( adc );
                break;
            case 1:
                v = pd->adc2probeV( adc );
                break;
            case 2:
                v = pd->adc2probeI( adc );
                break;
            default:
                v = pd->adc2workI( adc );
                break;
            }
            data[ind]->append( v );
        }
        ind = (ind + 1) % 4;
    }
    return true;
}

bool Bipot::instantDataRaw( int & workV, int & probeV, int & workI, int & probeI )
{
    VoltampIo & io = *(pd->io);
    int data[4];
    bool res = io.instantAdc( data );
    if ( !res )
        return false;

    workV  = data[0];
    probeV = data[1];
    workI  = data[3];
    probeI = data[2];

    return true;
}

bool Bipot::instantData( qreal & workV, qreal & probeV, qreal & workI, qreal & probeI )
{
    VoltampIo & io = *(pd->io);
    int data[4];
    bool res = io.instantAdc( data );
    if ( !res )
        return false;

    workV  = pd->adc2workV( data[0] );
    probeV = pd->adc2probeV( data[1] );
    workI  = pd->adc2workI( data[3] );
    probeI = pd->adc2probeI( data[2] );

    return true;
}

bool Bipot::temperature( qreal & t )
{
    VoltampIo & io = *(pd->io);

    bool res = io.temperature( t );
    if ( !res )
        return false;
    return true;
}

bool Bipot::setTriggerEn( bool en )
{
    VoltampIo & io = *(pd->io);

    bool res = io.setTriggerEn( en );
    if ( !res )
        return false;
    return true;
}

bool Bipot::setSweepRange( qreal workV, qreal probeV )
{
    VoltampIo & io = *(pd->io);

    int dac[4];
    pd->workV2Dac( workV, dac[0], dac[1] );
    pd->probeV2Dac( probeV, dac[2], dac[3] );

    bool res = io.setSweepRange( dac );
    if ( !res )
        return false;
    return true;
}

bool Bipot::setSweepTime( int ptsCnt, qreal periodMs )
{
    VoltampIo & io = *(pd->io);

    int period = static_cast<int>( periodMs * 4.0 );

    bool res = io.setSweepTime( ptsCnt, period );
    if ( !res )
        return false;
    return true;
}

bool Bipot::setSweepEn( bool en )
{
    VoltampIo & io = *(pd->io);

    bool res = io.setSweepEn( en );
    if ( !res )
        return false;
    return true;
}

bool Bipot::sweepEn( bool & en )
{
    VoltampIo & io = *(pd->io);

    bool res = io.sweepEn( en );
    if ( !res )
        return false;
    return true;
}

bool Bipot::sweepData( QVector<qreal> & workV, QVector<qreal> & probeV, QVector<qreal> & workI, QVector<qreal> & probeI )
{
    VoltampIo & io = *(pd->io);
    pd->mutex.lock();
        bool en[4];
        en[0] = pd->sigs[0];
        en[1] = pd->sigs[1];
        en[2] = pd->sigs[2];
        en[3] = pd->sigs[3];
    pd->mutex.unlock();
    QVector<int> & dataRaw = pd->data;
    QVector<qreal> * data[4];
    workV.clear();
    probeV.clear();
    workI.clear();
    probeI.clear();
    data[0] = &workV;
    data[1] = &probeV;
    data[2] = &workI;
    data[3] = &probeI;
    bool res = io.sweepData( dataRaw );
    if ( !res )
        return false;
    // Unit convertion is to be applied here.
    int ind = 0;
    
    for ( QVector<int>::const_iterator i=dataRaw.begin(); i!= dataRaw.end(); i++ )
    {
        if ( en[ind] )
        {
            int adc = *i;
            qreal v;
            switch ( ind )
            {
            case 0:
                v = pd->adc2workV( adc );
                break;
            case 1:
                v = pd->adc2probeV( adc );
                break;
            case 2:
                v = pd->adc2workI( adc );
                break;
            default:
                v = pd->adc2probeI( adc );
                break;
            }
            data[ind]->append( v );
        }
        ind = (ind + 1) % 4;
    }
    return true;
}

bool Bipot::setOutput( int o )
{
    VoltampIo & io = *(pd->io);

    bool res = io.setOutput( o );
    if ( !res )
        return false;
    return true;
}

bool Bipot::runBootloader()
{
    VoltampIo & io = *(pd->io);

    bool res = io.runBootloader();
    if ( !res )
        return false;
    return true;
}

bool Bipot::bootloaderHardwareVersion( QString & stri )
{
    VoltampIo & io = *(pd->io);

    bool res = io.bootloaderHardwareVersion( stri );
    if ( !res )
        return false;
    return true;
}

bool Bipot::bootloaderFirmwareVersion( QString & stri )
{
    VoltampIo & io = *(pd->io);

    bool res = io.bootloaderFirmwareVersion( stri );
    if ( !res )
        return false;
    return true;
}

bool Bipot::firmwareUpgrade( const QString & fileName )
{
    VoltampIo & io = *(pd->io);

    bool res = io.firmwareUpgrade( fileName );
    if ( !res )
        return false;
    return true;
}


void Bipot::setmV2mA( qreal workA, qreal workB, qreal probeA, qreal probeB )
{
    QMutexLocker lock( &pd->mutex );
        pd->workA = workA;
        pd->workB = workB;
        pd->probeA = probeA;
        pd->probeB = probeB;
}

void Bipot::setVoltScale( qreal scaleWork, qreal scaleProbe )
{
    QMutexLocker lock( &pd->mutex );
        pd->workGain  = scaleWork;
        pd->probeGain = scaleProbe;
}

void Bipot::setTemperature( qreal temp )
{
    QMutexLocker lock( &pd->mutex );
        pd->temperature = temp;
}

void Bipot::clearCalibrationWorkDac()
{
    pd->clbrDacWork.clear();
}

bool Bipot::loadCalibrationWorkDac( const QString & fileName )
{
    QFile file( fileName );
    bool open = file.open( QIODevice::ReadOnly );
    if ( open )
    {
        QString stri; 
        while ( !file.atEnd() )
        {
            stri = file.readLine();
            QRegExp ex( "(\\w+)\\s+(\\w+)\\s+(\\w+)\\s+(\\w+)\\s+(\\w+)\\s+(\\w+)" );
            int index = ex.indexIn( stri );
            if ( index >= 0 )
            {
                CalibrationDac d;
                QString m;
                m = ex.cap( 1 );
                d.dacA = m.toInt();

                m = ex.cap( 2 );
                d.dacB = m.toInt();

                m = ex.cap( 3 );
                d.dacC = m.toInt();

                m = ex.cap( 4 );
                d.dacD = m.toInt();

                m = ex.cap( 5 );
                d.temp = m.toDouble();

                m = ex.cap( 6 );
                d.volt = m.toDouble();

                pd->clbrDacWork.append( d );
            }
        }
        file.close();
        return true;
    }

    return false;
}

bool Bipot::saveCalibrationWorkDac( const QString & fileName )
{
    QFile file( fileName );
    if ( file.open( QIODevice::WriteOnly ) )
    {
        QTextStream out( &file );
        for ( QList<CalibrationDac>::const_iterator i=pd->clbrDacWork.begin(); i!=pd->clbrDacWork.end(); i++ )
        {
            const CalibrationDac d = *i;
            QString stri = QString( "%1 %2 %3 %4 %5 %6\n" ).arg( d.dacA ).arg( d.dacB ).arg( d.dacC ).arg( d.dacD ).arg( d.temp ).arg( d.volt );
            out << stri;
        }
        out.flush();
        file.close();
        return true;
    }
    return false;
}

bool Bipot::addCalibrationWorkDac( int dacA, int dacB, int dacC, int dacD, qreal mV )
{
    CalibrationDac d;
    d.dacA = dacA;
    d.dacB = dacB;
    d.dacC = dacC;
    d.dacD = dacD;
    d.volt = mV;

    qreal tt = 0.0;
    for ( int i=0; i<PD::MEASURES_CNT; i++ )
    {
        qreal t;
        bool res = temperature( t );
        if ( !res )
            return false;
        tt += t;
    }
    tt /= static_cast<qreal>( PD::MEASURES_CNT );
    d.temp = tt;

    pd->clbrDacWork.append( d );

    return true;
}

void Bipot::setCalibrationWorkDac( qreal a1, qreal a2, qreal b )
{
}

void Bipot::clearCalibrationProbeDac()
{
    pd->clbrDacProbe.clear();
}

bool Bipot::loadCalibrationProbeDac( const QString & fileName )
{
    QFile file( fileName );
    bool open = file.open( QIODevice::ReadOnly );
    if ( open )
    {
        QString stri; 
        while ( !file.atEnd() )
        {
            stri = file.readLine();
            QRegExp ex( "(\\w+)\\s+(\\w+)\\s+(\\w+)\\s+(\\w+)\\s+(\\w+)\\s+(\\w+)" );
            int index = ex.indexIn( stri );
            if ( index >= 0 )
            {
                CalibrationDac d;
                QString m;
                m = ex.cap( 1 );
                d.dacA = m.toInt();

                m = ex.cap( 2 );
                d.dacB = m.toInt();

                m = ex.cap( 3 );
                d.dacC = m.toInt();

                m = ex.cap( 4 );
                d.dacD = m.toInt();

                m = ex.cap( 5 );
                d.temp = m.toDouble();

                m = ex.cap( 6 );
                d.volt = m.toDouble();

                pd->clbrDacProbe.append( d );
            }
        }
        file.close();
        return true;
    }

    return false;
}

bool Bipot::saveCalibrationProbeDac( const QString & fileName )
{
    QFile file( fileName );
    if ( file.open( QIODevice::WriteOnly ) )
    {
        QTextStream out( &file );
        for ( QList<CalibrationDac>::const_iterator i=pd->clbrDacProbe.begin(); i!=pd->clbrDacProbe.end(); i++ )
        {
            const CalibrationDac d = *i;
            QString stri = QString( "%1 %2 %3 %4 %5 %6\n" ).arg( d.dacA ).arg( d.dacB ).arg( d.dacC ).arg( d.dacD ).arg( d.temp ).arg( d.volt );
            out << stri;
        }
        out.flush();
        file.close();
        return true;
    }
    return false;
}

bool Bipot::addCalibrationProbeDac( int dacA, int dacB, int dacC, int dacD, qreal mV )
{
    CalibrationDac d;
    d.dacA = dacA;
    d.dacB = dacB;
    d.dacC = dacC;
    d.dacD = dacD;
    d.volt = mV;

    qreal tt = 0.0;
    for ( int i=0; i<PD::MEASURES_CNT; i++ )
    {
        qreal t;
        bool res = temperature( t );
        if ( !res )
            return false;
        tt += t;
    }
    tt /= static_cast<qreal>( PD::MEASURES_CNT );
    d.temp = tt;

    pd->clbrDacProbe.append( d );
    return true;
}

void Bipot::clearCalibrationAdc()
{
    pd->clbrAdc.clear();
}

bool Bipot::loadCalibrationAdc( const QString & fileName )
{
    QFile file( fileName );
    bool open = file.open( QIODevice::ReadOnly );
    if ( open )
    {
        QString stri; 
        while ( !file.atEnd() )
        {
            stri = file.readLine();
            QRegExp ex( "(\\w+)\\s+(\\w+)\\s+(\\w+)\\s+(\\w+)\\s+(\\w+)\\s+(\\w+)\\s+(\\w+)\\s+(\\w+)\\s+(\\w+)" );
            int index = ex.indexIn( stri );
            if ( index >= 0 )
            {
                CalibrationAdc d;
                QString m;
                m = ex.cap( 1 );
                d.adcA = m.toDouble();

                m = ex.cap( 2 );
                d.adcB = m.toDouble();

                m = ex.cap( 3 );
                d.adcC = m.toDouble();

                m = ex.cap( 4 );
                d.adcD = m.toDouble();

                m = ex.cap( 5 );
                d.temp = m.toDouble();

                m = ex.cap( 6 );
                d.voltA = m.toDouble();

                m = ex.cap( 7 );
                d.voltB = m.toDouble();

                m = ex.cap( 8 );
                d.voltC = m.toDouble();

                m = ex.cap( 9 );
                d.voltD = m.toDouble();

                pd->clbrAdc.append( d );
            }
        }
        file.close();
        return true;
    }

    return false;
}

bool Bipot::saveCalibrationAdc( const QString & fileName )
{
    QFile file( fileName );
    if ( file.open( QIODevice::WriteOnly ) )
    {
        QTextStream out( &file );
        for ( QList<CalibrationAdc>::const_iterator i=pd->clbrAdc.begin(); i!=pd->clbrAdc.end(); i++ )
        {
            const CalibrationAdc d = *i;
            QString stri = QString( "%1 %2 %3 %4 %5 %6 %7 %8 %9\n" ).arg( d.adcA )
                                                                    .arg( d.adcB )
                                                                    .arg( d.adcC )
                                                                    .arg( d.adcD )
                                                                    .arg( d.temp )
                                                                    .arg( d.voltA )
                                                                    .arg( d.voltB )
                                                                    .arg( d.voltC )
                                                                    .arg( d.voltD );
            out << stri;
        }
        out.flush();
        file.close();
        return true;
    }
    return false;
}

bool Bipot::addCalibrationAdc( qreal mv0, qreal mv1, qreal mv2, qreal mv3 )
{
    CalibrationAdc d;

    qreal f0 = 0.0;
    qreal f1 = 0.0;
    qreal f2 = 0.0;
    qreal f3 = 0.0;
    for ( int i=0; i<PD::MEASURES_CNT; i++ )
    {
        int i0, i1, i2, i3;
        bool res = instantDataRaw( i0, i1, i2, i3 );
        if ( !res )
            return false;
        f0 += static_cast<qreal>( i0 );
        f1 += static_cast<qreal>( i1 );
        f2 += static_cast<qreal>( i2 );
        f3 += static_cast<qreal>( i3 );
    }

    f0 /= static_cast<qreal>( PD::MEASURES_CNT );
    f1 /= static_cast<qreal>( PD::MEASURES_CNT );
    f2 /= static_cast<qreal>( PD::MEASURES_CNT );
    f3 /= static_cast<qreal>( PD::MEASURES_CNT );
    
    d.adcA = f0;
    d.adcB = f1;
    d.adcC = f2;
    d.adcD = f3;

    d.voltA = mv0;
    d.voltB = mv1;
    d.voltC = mv2;
    d.voltD = mv3;

    qreal tt = 0.0;
    for ( int i=0; i<PD::MEASURES_CNT; i++ )
    {
        qreal t;
        bool res = temperature( t );
        if ( !res )
            return false;
        tt += t;
    }
    tt /= static_cast<qreal>( PD::MEASURES_CNT );
    d.temp = tt;

    pd->clbrAdc.append( d );

    return true;
}

bool Bipot::calibrationCalc()
{
    return true;
}

bool Bipot::calibrationLoad( const QString & fileName )
{
    return true;
}

bool Bipot::calibrationSave( const QString & fileName )
{
    return true;
}






