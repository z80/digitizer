
#include "io.h"
#include <windows.h>
#include "SiUSBXp.h"

class Io::PD
{
public:
    PD()
    {
    }

    ~PD()
    {
    }

    HANDLE ftdi;
    QStringList portsList;

    static const int TIMEOUT;
};

const int Io::PD::TIMEOUT = 3000;

Io::Io()
{
    pd = new PD();
    pd->ftdi = 0;
}

Io::~Io()
{
    delete pd;
}

QStringList Io::enumDevices() const
{
    pd->portsList.clear();
    QStringList l;

    DWORD cnt;
    SI_STATUS st = SI_GetNumDevices( &cnt );
    if ( st != SI_SUCCESS )
        return l;
    for ( DWORD i=0; i<cnt; i++ )
    {
    	SI_DEVICE_STRING devString;
        st = SI_GetProductString( i, devString, SI_RETURN_DESCRIPTION );
        if ( st != SI_SUCCESS )
            return QStringList();
        QString stri = QString( devString );
        pd->portsList << stri;
    }
    return l;
}

bool Io::open( int index )
{
    if ( index >= pd->portsList.size() )
    {
        enumDevices();
        if ( index >= pd->portsList.size() )
            return false;
    }
    if ( pd->ftdi )
    {
        SI_Close( pd->ftdi );
        pd->ftdi = 0;
    }

    SI_STATUS st;

    st = SI_Open( index, &(pd->ftdi) );
    if ( st != SI_SUCCESS )
        return false;
    st = SI_SetBaudRate( pd->ftdi, 115200 );
    if ( st != SI_SUCCESS )
        return false;
    st = SI_SetLineControl( pd->ftdi, 0 ); // 1 stop bit no parity control.
    if ( st != SI_SUCCESS )
        return false;
    st = SI_SetFlowControl( pd->ftdi, 
                       SI_STATUS_INPUT, 
                       SI_HELD_ACTIVE, 
                       SI_HELD_INACTIVE, 
                       SI_STATUS_INPUT, 
                       SI_STATUS_INPUT, 
                       FALSE );
    if ( st != SI_SUCCESS )
        return false;
    st = SI_SetTimeouts( PD::TIMEOUT, PD::TIMEOUT );
    if ( st != SI_SUCCESS )
        return false;

    return true;
}

bool Io::isOpen() const
{
    if ( !pd->ftdi )
        return false;
    return true;

}

void Io::close()
{
    if ( pd->ftdi )
        SI_Close( pd->ftdi );
    pd->ftdi = 0;
}

int Io::write( quint8 * data, int dataSz )
{
    if ( !pd->ftdi )
        return false;

    //int cnt = pd->ftdi->write( reinterpret_cast<char *>( data ), dataSz );
    DWORD qty;
    SI_STATUS st = SI_Write( pd->ftdi, reinterpret_cast<void *>( data ), dataSz, &qty, 0 );
    if ( st != SI_SUCCESS )
        return -1;
    return static_cast<int>( qty );
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

int Io::read( quint8 * data, int dataSz, bool tillTimeout )
{
    if ( !pd->ftdi )
        return false;

    QTime t0 = QTime::currentTime();
    t0.start();
    int cnt;
    int totalCnt = 0;
    DWORD qty = 0;
    do {
        SI_STATUS st = SI_Read( pd->ftdi, reinterpret_cast<void *>( &(data[totalCnt]) ), 1, &qty, 0 );
        if ( st != SI_SUCCESS )
            return -1;
        //cnt = pd->ftdi->read( reinterpret_cast<char *>( &(data[totalCnt]) ), 1 );
        totalCnt += static_cast<int>( qty );
        if ( ( tillTimeout ) && (qty < 1) )
            Sleep::msleep( 1 );
    } while ( (totalCnt < dataSz) && 
              ( (qty > 0) || 
              ( (tillTimeout) && (t0.elapsed() < PD::TIMEOUT) ) ) );
    return cnt;
}


