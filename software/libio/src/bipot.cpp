
#include "bipot.h"
#include "voltamp_io.h"


class Bipot::PD
{
public:
    PD() {}
    ~PD() {}

    VoltampIo * io;
    QMutex       mutex;
    bool         sigs[4];
    QVector<int> data;
};

Bipot::Bipot()
{
    pd = new PD();
    VoltampIo * io = new VoltampIo();
    pd->io = io;
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
            qreal v = static_cast<qreal>( *i );
            data[ind]->append( v );
        }
        ind ++;
    }
    return true;
}

bool Bipot::instantDataRaw( int & workV, int & probeV, int & workI, int & probeI )
{
    VoltampIo & io = *(pd->io);
    int data[4];
    bool res = io.instantAdc( data );
    workV  = data[0];
    probeV = data[1];
    workI  = data[2];
    probeI = data[3];
    return true;
}

bool Bipot::instantData( qreal & workV, qreal & probeV, qreal & workI, qreal & probeI )
{
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
}

void Bipot::clearCalibrationWorkDac()
{
    
}

bool Bipot::loadCalibrationWorkDac( const QString & fileName )
{

    return true;
}

bool Bipot::saveCalibrationWorkDac( const QString & fileName )
{

    return true;
}

void Bipot::addCalibrationWorkDac( int dacA, int DacB, qreal mV )
{
}

void Bipot::clearCalibrationProbeDac()
{
}

bool Bipot::loadCalibrationProbeDac( const QString & fileName )
{

    return true;
}

bool Bipot::saveCalibrationProbeDac( const QString & fileName )
{
    
    return true;
}

void Bipot::addCalibrationProbeDac( int dacA, int DacB, qreal mV )
{
}

void Bipot::clearCalibrationAdc()
{
}

bool Bipot::loadCalibrationAdc( const QString & fileName )
{
    return true;
}

bool Bipot::saveCalibrationAdc( const QString & fileName )
{
    return true;
}

void Bipot::addCalibrationAdc( qreal mv0, qreal mv1, qreal mv2, qreal mv3 )
{
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






