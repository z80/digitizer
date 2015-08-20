
#include "io.h"
#include "qextserialport.h"
#include "qextserialenumerator.h"

class Io::PD
{
public:
    PD()
    {
    }

    ~PD()
    {
    }

    QextSerialPort * ftdi;
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
    QList<QextPortInfo> pl = QextSerialEnumerator::getPorts();
    foreach( QextPortInfo i, pl )
    {
        qDebug() << i.portName;   ///< Port name.
        qDebug() << i.physName;   ///< Physical name.
        qDebug() << i.friendName; ///< Friendly name.
        qDebug() << i.enumName;   ///< Enumerator name.

        l << i.friendName;
        pd->portsList << i.portName;
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
        pd->ftdi->deleteLater();
    }
    QextSerialPort * port;
    QString portName = pd->portsList.at( index );
    port = new QextSerialPort( portName );
    port->setQueryMode( QextSerialPort::Polling );
    port->setBaudRate( BAUD115200 );
    //port->setBaudRate( BAUD9600 );
    //port->setFlowControl( FLOW_HARDWARE );
    port->setFlowControl( FLOW_OFF );
    port->setParity( PAR_NONE );
    port->setDataBits( DATA_8 );
    port->setStopBits( STOP_1 );
    pd->ftdi = port;
    bool res = port->open( QIODevice::ReadWrite );
    if ( !res )
    {
        qDebug() << port->errorString();
        port->deleteLater();
        pd->ftdi = 0;
    }
    return res;
}

bool Io::isOpen() const
{
    if ( !pd->ftdi )
        return false;
    bool res = pd->ftdi->isOpen();
    return res;

}

void Io::close()
{
    if ( pd->ftdi )
    {
        pd->ftdi->close();
        pd->ftdi->deleteLater();
    }
    pd->ftdi = 0;
}

int Io::write( quint8 * data, int dataSz )
{
    if ( !pd->ftdi )
        return false;

    int cnt = pd->ftdi->write( reinterpret_cast<char *>( data ), dataSz );
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

int Io::read( quint8 * data, int dataSz, bool tillTimeout )
{
    if ( !pd->ftdi )
        return false;

    QTime t0 = QTime::currentTime();
    t0.start();
    int cnt;
    int totalCnt = 0;
    do {
        cnt = pd->ftdi->read( reinterpret_cast<char *>( &(data[totalCnt]) ), 1 );
        totalCnt += cnt;
        if ( ( tillTimeout ) && (cnt < 1) )
            Sleep::msleep( 1 );
    } while ( (totalCnt < dataSz) && 
              ( (cnt > 0) || 
              ( (tillTimeout) && (t0.elapsed() < PD::TIMEOUT) ) ) );
    return cnt;
}


