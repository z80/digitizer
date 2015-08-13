
#include "voltamp_io.h"
#include "io.h"

class VoltampIo::PD
{
public:
    PD()
    {
    }

    ~PD()
    {
    }

    void    encodeData( quint8 * data, int sz );
    quint32 msToTicks( qreal ms );
    QMutex mutex;
    Io * io;
    QByteArray buffer_raw;
    QByteArray buffer_cmd;
    QByteArray buffer;

    static const int CMD_SET_ARGS;
    static const int CMD_EXEC_FUNC;

    static const int TIMEOUT;
    static const int IN_BUFFER_SZ;
};

const int VoltampIo::PD::CMD_SET_ARGS  = 1;
const int VoltampIo::PD::CMD_EXEC_FUNC = 2;

const int VoltampIo::PD::TIMEOUT = 3000;
const int VoltampIo::PD::IN_BUFFER_SZ = (4 * 2048);

void VoltampIo::PD::encodeData( quint8 * data, int sz )
{
    QByteArray & arr = buffer;
    arr.reserve( sz * 2 + 2 );
    arr.clear();
    for ( int i=0; i<sz; i++ )
    {
        if ( ( data[i] == '\\' ) || ( data[i] == '\0' ) )
            arr.append( '\\' );
        arr.append( data[i] );
    }
    arr.append( '\0' );
}

quint32 VoltampIo::PD::msToTicks( qreal ms )
{
    quint32 ticks = static_cast<quint32>( ms * 24000.0 / (3*239.5*16.0) );
    return ticks;
}

VoltampIo::VoltampIo()
{
    pd = new PD;
    pd->io = new Io();
}

VoltampIo::~VoltampIo()
{
    delete pd->io;
    delete pd;
}

QStringList VoltampIo::enumDevices()
{
    QStringList l = pd->io->enumDevices();
    return l;
}

bool VoltampIo::open( int index )
{
    QMutexLocker lock( &pd->mutex );

    bool res = pd->io->open( index );
    return res;
}

bool VoltampIo::isOpen() const
{
    QMutexLocker lock( &pd->mutex );

    bool res = pd->io->isOpen();
    return res;
}

void VoltampIo::close()
{
    QMutexLocker lock( &pd->mutex );

    pd->io->close();
}

bool VoltampIo::hardware_version( QString & stri )
{
    QMutexLocker lock( &pd->mutex );

    quint8 funcInd = 0;
    bool res = execFunc( funcInd );
    //if ( !res )
    //    return false;

    QByteArray & arr = pd->buffer;
    arr.resize( PD::IN_BUFFER_SZ );
    bool eom;
    int cnt = read( reinterpret_cast<quint8 *>( arr.data() ), arr.size(), eom );
    if ( !eom )
        return false;
    stri.clear();
    for ( int i=0; i<cnt; i++ )
        stri.append( QChar( arr.at( i ) ) );
    return true;
}

bool VoltampIo::firmware_version( QString & stri )
{
    QMutexLocker lock( &pd->mutex );

    quint8 funcInd = 1;
    bool res = execFunc( funcInd );
    if ( !res )
        return false;

    QByteArray & arr = pd->buffer;
    arr.resize( PD::IN_BUFFER_SZ );
    bool eom;
    int cnt = read( reinterpret_cast<quint8 *>( arr.data() ), arr.size(), eom );
    if ( !eom )
        return false;
    stri.clear();
    for ( int i=0; i<cnt; i++ )
        stri.append( QChar( arr.at( i ) ) );
    return true;
}

bool VoltampIo::setLed( int leds )
{
    QMutexLocker lock( &pd->mutex );

    quint8 v;
    v = static_cast<quint8>( leds & 0xFF );

    bool res;
    res = setArgs( &v, 1 );
    if ( !res )
        return false;

    quint8 funcInd = 2;
    res = execFunc( funcInd );
    if ( !res )
        return false;

    return true;
}

bool VoltampIo::setDac1( int dacA, int dacB )
{
    QMutexLocker lock( &pd->mutex );

    QByteArray & b = pd->buffer_raw;
    b.clear();
    b.reserve( 4 );

    quint8 v;
    v = static_cast<quint8>( dacA & 0xFF );
    b.append( *reinterpret_cast<char *>(&v) );
    v = static_cast<quint8>( (dacA >> 8) & 0xFF );
    b.append( *reinterpret_cast<char *>(&v) );
    v = static_cast<quint8>( dacB & 0xFF );
    b.append( *reinterpret_cast<char *>(&v) );
    v = static_cast<quint8>( (dacB >> 8) & 0xFF );
    b.append( *reinterpret_cast<char *>(&v) );

    bool res;
    res = setArgs( reinterpret_cast<quint8 *>( b.data() ), b.size() );
    if ( !res )
        return false;

    quint8 funcInd = 3;
    res = execFunc( funcInd );
    if ( !res )
        return false;

    return true;
}

bool VoltampIo::setDac2( int dacA, int dacB )
{
    QMutexLocker lock( &pd->mutex );

    QByteArray & b = pd->buffer_raw;
    b.clear();
    b.reserve( 4 );

    quint8 v;
    v = static_cast<quint8>( dacA & 0xFF );
    b.append( *reinterpret_cast<char *>(&v) );
    v = static_cast<quint8>( (dacA >> 8) & 0xFF );
    b.append( *reinterpret_cast<char *>(&v) );
    v = static_cast<quint8>( dacB & 0xFF );
    b.append( *reinterpret_cast<char *>(&v) );
    v = static_cast<quint8>( (dacB >> 8) & 0xFF );
    b.append( *reinterpret_cast<char *>(&v) );

    bool res;
    res = setArgs( reinterpret_cast<quint8 *>( b.data() ), b.size() );
    if ( !res )
        return false;

    quint8 funcInd = 4;
    res = execFunc( funcInd );
    if ( !res )
        return false;

    return true;
}

bool VoltampIo::instantAdc( int * data )
{
    QMutexLocker lock( &pd->mutex );

    quint8 funcInd = 5;
    bool res = execFunc( funcInd );
    if ( !res )
        return false;

    QByteArray & arr = pd->buffer;
    arr.resize( PD::IN_BUFFER_SZ );
    bool eom;
    int cnt = read( reinterpret_cast<quint8 *>( arr.data() ), arr.size(), eom );
    if ( ( !eom ) || ( cnt < 16 ) )
        return false;

    for ( int i=0; i<4; i++ )
    {
        quint8 * d = reinterpret_cast<quint8 *>( arr.data() );
        d += i*4;
        int val = static_cast<int>( d[0] );
        val    += (static_cast<int>( d[1] ) << 8);
        val    += (static_cast<int>( d[2] ) << 16);
        val    += (static_cast<int>( d[3] ) << 24);
        data[i] = val;
    }
    
    return true;
}

bool VoltampIo::setOscSignals( bool * en )
{
    QMutexLocker lock( &pd->mutex );

    quint8 funcInd = 6;
    bool res = execFunc( funcInd );
    if ( !res )
        return false;
    quint8 v = ( (en[0] ? 1 : 0) | (en[1] ? 2 : 0) | (en[2] ? 4 : 0) | (en[3] ? 8 : 0) );
    res = setArgs( &v, 1 );
    if ( !res )
        return false;
    return true;
}

bool VoltampIo::setOscPeriod( int ptsCnt, qreal periodMs )
{
    // Adc timer frequency is 100kHz. So each signal is suppsed to 
    // be measured with frequency 25kHz.
    // One time tick is 1e6[us/s]/25e3[tick/s] = 40[us/tick] = 0.04[ms/tick].
    qreal period = ( periodMs / 0.04 )/ static_cast<qreal>( ptsCnt );
    quint32 periodI = static_cast<quint32>( period );
    quint8  v[4];
    v[0] = static_cast<quint8>( periodI & 0xFF );
    v[1] = static_cast<quint8>( (periodI >> 8) & 0xFF );
    v[2] = static_cast<quint8>( (periodI >> 16) & 0xFF );
    v[3] = static_cast<quint8>( (periodI >> 24) & 0xFF );

    bool res;
    res = setArgs( reinterpret_cast<quint8 *>( v ), 4 );
    if ( !res )
        return false;

    quint8 funcInd = 7;
    res = execFunc( funcInd );
    if ( !res )
        return false;

    return true;
}

bool VoltampIo::oscData( QVector<int> & data )
{
    QMutexLocker lock( &pd->mutex );

    quint8 funcInd = 8;
    bool res = execFunc( funcInd );
    if ( !res )
        return false;
    bool eom;
    QByteArray & arr = pd->buffer;
    arr.resize( PD::IN_BUFFER_SZ );
    int cnt = read( reinterpret_cast<quint8 *>( arr.data() ), arr.size(), eom );
    if ( !eom )
        return false;

    int sz = cnt / 3;
    data.resize( sz );
    quint8 * d = reinterpret_cast<quint8 *>( arr.data() );
    for ( int i=0; i<sz; i++ )
    {
        int v;
        v  = static_cast<int>( d[i] );
        v += static_cast<int>( d[i+1] << 8 );
        v += static_cast<int>( d[i+2] << 16 );
        data[i] = v;
    }

    return true;
}
















bool VoltampIo::setArgs( quint8 * data, int dataSz )
{
    QByteArray & arr = pd->buffer_cmd;
    arr.reserve( dataSz + 1 );
    arr.clear();
    quint8 cmd = static_cast<quint8>( PD::CMD_SET_ARGS );
    arr.append( *reinterpret_cast<char *>( &cmd ) );
    arr.append( reinterpret_cast<char *>(data), dataSz );
    pd->encodeData( reinterpret_cast<quint8 *>(arr.data()), arr.size() );
    int cnt = write( reinterpret_cast<quint8 *>( pd->buffer.data() ), pd->buffer.size() );
    return ( cnt == pd->buffer.size() );
}

bool VoltampIo::execFunc( quint16 funcId )
{
    QByteArray & arr = pd->buffer_cmd;
    arr.reserve( 2 + 1 );
    arr.clear();
    quint8 cmd = static_cast<quint8>( PD::CMD_EXEC_FUNC );
    arr.append( *reinterpret_cast<char *>( &cmd ) );
    cmd = static_cast<quint8>( funcId & 0xFF );
    arr.append( cmd );
    cmd = static_cast<quint8>( (funcId >> 8) & 0xFF );
    arr.append( cmd );
    pd->encodeData( reinterpret_cast<quint8 *>(arr.data()), arr.size() );
    int cnt = write( reinterpret_cast<quint8 *>( pd->buffer.data() ), pd->buffer.size() );
    return ( cnt == pd->buffer.size() );
}

int VoltampIo::readData( quint8 * data, int dataSz )
{
    bool eom;
    int cnt = read( data, dataSz, eom );
    if ( !eom )
        return -1;
    return cnt;
}

int VoltampIo::write( quint8 * data, int dataSz )
{
    //pd->encodeData( data, dataSz );
    int cnt = pd->io->write( data, dataSz );
    return cnt;
}

class Sleep: public QThread
{
public:
    Sleep()
        : QThread()
    {
    }

    ~Sleep()
    {
    }

    static void msleep( int ms )
    {
        QThread::msleep( ms );
    }
};

int VoltampIo::read( quint8 * data, int dataSz, bool & eom )
{
    quint8 one;
    QTime t0 = QTime::currentTime();
    t0.start();
    bool slash = false;
    int ind = 0;
    do
    {
        int cnt = pd->io->read( &one, 1 );
        if ( cnt < 1 )
        {
            Sleep::msleep( 1 );
        }
        else
        {
            if ( !slash )
            {
                if ( one == '\\' )
                    slash = true;
                else if ( one == '\0' )
                {
                    eom = true;
                    return ind;
                }
                else
                    data[ind++] = one;
            }
            else
            {
                data[ind++] = one;
                slash = false;
            }
        }
    } while ( ( ind < dataSz ) && ( t0.elapsed() < PD::TIMEOUT ) );
    eom = false;
    return ind;
}

