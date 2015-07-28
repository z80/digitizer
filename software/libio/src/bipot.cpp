
#include "bipot.h"
#include "voltamp_io.h"


struct CalibrationDac
{
    int   dacA;
    int   dacB;
    qreal volt;
};

struct CalibrationAdc
{
    int adcA;
    int adcB;
    int adcC;
    int adcD;

    qreal voltA;
    qreal voltB;
    qreal voltC;
    qreal voltD;
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

    VoltampIo * io;
    QMutex       mutex;
    bool         sigs[4];
    QVector<int> data;

    qreal workA, workB, probeA, probeB;

    QList<CalibrationDac> clbrDacWork, 
                          clbrDacProbe;
    QList<CalibrationAdc> clbrAdc;
};

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


Bipot::Bipot()
{
    pd = new PD();
    VoltampIo * io = new VoltampIo();
    pd->io = io;

    pd->workA = 1.0;
    pd->workB = 0.0;
    pd->probeA = 1.0;
    pd->probeB = 0.0;
}

Bipot::~Bipot()
{
    delete pd->io;
    delete pd;
}

bool Bipot::open()
{
    VoltampIo & io = *(pd->io);
    bool res = io.open( 0 );
    return res;
}

void Bipot::close()
{
    VoltampIo & io = *(pd->io);
    io.close();
}

bool Bipot::setWorkRaw( int a, int b )
{
    VoltampIo & io = *(pd->io);
    bool res = io.setDac1( a, b );
    return res;
}

bool Bipot::setWorkMv( qreal mv )
{
    return true;
}

bool Bipot::setProbeRaw( int a, int b )
{
    VoltampIo & io = *(pd->io);
    bool res = io.setDac2( a, b );
    return res;
}

bool Bipot::setProbeMv( qreal mv )
{
    return true;
}

bool Bipot::setOscPeriod( int ptsCnt, qreal msTotal )
{
    VoltampIo & io = *(pd->io);
    bool res = io.setOscPeriod( ptsCnt, msTotal );
    return res;
}

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

bool Bipot::oscData( QVector<qreal> & workV, QVector<qreal> & probeV, QVector<qreal> & workI, QVector<qreal> & probeI )
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
    data[4] = &probeI;
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
                pd->adc2workV( adc );
                break;
            case 1:
                pd->adc2probeV( adc );
                break;
            case 2:
                pd->adc2workI( adc );
                break;
            default:
                pd->adc2probeI( adc );
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
    workI  = data[2];
    probeI = data[3];

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
    workI  = pd->adc2workI( data[2] );
    probeI = pd->adc2probeI( data[3] );

    return true;
}

bool Bipot::sweepWork( qreal from, qreal to, qreal ms )
{
    return true;
}

bool Bipot::triggerSweepWork( qreal from, qreal to, int ptsCnt )
{
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
            QRegExp ex( "(\\w+)\\s+(\\w+)\\s+(\\w+)" );
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
    return false;
}

void Bipot::addCalibrationWorkDac( int dacA, int DacB, qreal mV )
{
    CalibrationDac d;
    d.dacA = dacA;
    d.dacB = dacB;
    d.volt = mV;
    pd->clbrDacWork.append( d );
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
            QRegExp ex( "(\\w+)\\s+(\\w+)\\s+(\\w+)" );
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
    
    return true;
}

void Bipot::addCalibrationProbeDac( int dacA, int DacB, qreal mV )
{
    CalibrationDac d;
    d.dacA = dacA;
    d.dacB = dacB;
    d.volt = mV;
    pd->clbrDacProbe.append( d );
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
            QRegExp ex( "(\\w+)\\s+(\\w+)\\s+(\\w+)\\s+(\\w+)\\s+(\\w+)\\s+(\\w+)\\s+(\\w+)\\s+(\\w+)" );
            int index = ex.indexIn( stri );
            if ( index >= 0 )
            {
                CalibrationAdc d;
                QString m;
                m = ex.cap( 1 );
                d.adcA = m.toInt();

                m = ex.cap( 2 );
                d.adcB = m.toInt();

                m = ex.cap( 3 );
                d.adcC = m.toInt();

                m = ex.cap( 4 );
                d.adcD = m.toInt();

                m = ex.cap( 5 );
                d.voltA = m.toDouble();

                m = ex.cap( 6 );
                d.voltB = m.toDouble();

                m = ex.cap( 7 );
                d.voltC = m.toDouble();

                m = ex.cap( 8 );
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
    return true;
}

void Bipot::addCalibrationAdc( qreal mv0, qreal mv1, qreal mv2, qreal mv3 )
{
    CalibrationDac d;
    d.dacA = dacA;
    d.dacB = dacB;
    d.volt = mV;
    pd->clbrDacProbe.append( d );
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






