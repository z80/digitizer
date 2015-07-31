
#include "main_wnd.h"
#include <QFileDialog>
#include "lua.hpp"


const QString MainWnd::SETTINGS_INI = "./settings.ini";

MainWnd::MainWnd( QWidget * parent )
    : QMainWindow( parent )
{
    ui.setupUi( this );
    //ui.osc->show();

    io = new Bipot();

    loadSettings();
    refreshDevicesList();

    QVBoxLayout * bl = new QVBoxLayout( ui.osc );
    oscWork = new OscilloscopeWnd( this );
    bl->addWidget( oscWork );

    oscProbe = new OscilloscopeWnd( this );
    bl->addWidget( oscProbe );

    calibrationWnd = new CalibrationWnd( 0 );
    calibrationWnd->setIo( io );

    connect( ui.action_Quit,  SIGNAL(triggered()), this, SLOT(slotQuit()) );
    connect( ui.action_About, SIGNAL(triggered()), this, SLOT(slotAbout()) );
    connect( ui.actionCalibration,  SIGNAL(triggered()), this, SLOT(slotCalibration()) );

    connect( this, SIGNAL(sigInstantValues(qreal,qreal,qreal,qreal)), 
             this, SLOT(slotInstantValues(qreal,qreal,qreal,qreal)), 
             Qt::QueuedConnection );
    

}

MainWnd::~MainWnd()
{
    //state->deleteLater();
    ui.osc->deleteLater();
    delete io;
}

void MainWnd::loadSettings()
{
    QSettings s( SETTINGS_INI, QSettings::IniFormat );

    devName  = s.value( "devName",  "0" ).toInt();

    this->restoreState( s.value( "state", QByteArray() ).toByteArray() );
}

void MainWnd::saveSettings()
{
    QSettings s( SETTINGS_INI, QSettings::IniFormat );

    s.setValue( "devName", devName );

    s.setValue( "state", this->saveState() );
}

int MainWnd::deviceName() const
{
    return devName;
}

void MainWnd::slotQuit()
{
    saveSettings();
    this->deleteLater();
    qApp->quit();
}

void MainWnd::slotReopen()
{
    io->close();
    bool res = io->open( devName );
    if ( res )
        setTitle( "device connected" );
    else
        setTitle( "no device" );
}

void MainWnd::slotAbout()
{
    QString fmwVer = io->firmwareVersion();
    //QString hdwVer = io->hardwareVersion();
    QString stri = QString( "Bipotentiostat control module\nfirmware version: %1" ).arg( fmwVer );
    QMessageBox::about( this, "About", stri );
}

void MainWnd::slotCalibration()
{
    calibrationWnd->show();
}


void MainWnd::slotDevice()
{
    foreach( QAction * a, devicesList )
        a->setChecked( false );
    QAction * a = qobject_cast<QAction *>( sender() );
    int index = devicesList.indexOf( a );
    bool res = io->open( index );
    a->setChecked( res );
    devName = index;
}

void MainWnd::closeEvent( QCloseEvent * e )
{
    slotQuit();
}

void MainWnd::setTitle( const QString & stri )
{
    setWindowTitle( QString( "Potentiostat: %1" ).arg( stri ) );
}

class Msleep: public QThread
{
public:
    static void msleep( int ms )
    {
        QThread::msleep( ms );
    }
};

void MainWnd::measure()
{
    bool term = false;
    do {
        Bipot * io;
        mutex.lock();
            io = this->io;
        mutex.unlock();
        if ( io )
        {
            if ( !io->isOpen() )
            {
                reopen();
                Msleep::msleep( 1000 );
                continue;
            }

            bool res;
            qreal workV, probeV, workI, probeI;
            res = io->instantData( workV, probeV, workI, probeI );
            if ( !res )
            {
                io->close();
                Msleep::msleep( 1000 );
                continue;
            }

            

            if ( paintSz > 12 )
                emit sigReplot();
            int sleepSz = szEaux + szEref + szIaux;
            if ( sleepSz < 30 )
                Msleep::msleep( 10 );
        }
        else
            Msleep::msleep( 100 );
        mutex.lock();
            term = terminate;
        mutex.unlock();
    } while ( !term );
}

void MainWnd::measure()
{
    io->close();
    io->open( devName );
}

void MainWnd::refreshDevicesList()
{
    foreach( QAction * a, devicesList )
        a->deleteLater();
    devicesList.clear();
    QStringList l = io->deviceList();
    foreach( QString stri, l )
    {
        QAction * a = new QAction( stri, ui.menuDevice );
        ui.menuDevice->addAction( a );
        a->setCheckable( true );
        connect( a, SIGNAL(triggered()), this, SLOT(slotDevice()) );
        devicesList.append( a );
    }
}







void MainWnd::slotGain()
{
    int indV  = ui.workVoltGain->currentIndex();
    qreal gainV = pow( 10.0, static_cast<qreal>( indV ) );
    int indIA = ui.workCurrGainA->currentIndex();
    qreal gainIA = pow( 10.0, static_cast<qreal>( indIA ) );
    int indIB = ui.workCurrGainB->currentIndex();
    qreal gainIB = pow( 10.0, static_cast<qreal>( indIB + 1 ) );
    qreal gainI1 = gainIA * gainIB;

    indV  = ui.workVoltGain->currentIndex();
    gainV = pow( 10.0, static_cast<qreal>( indV ) );
    indIA = ui.workCurrGainA->currentIndex();
    gainIA = pow( 10.0, static_cast<qreal>( indIA ) );
    indIB = ui.workCurrGainB->currentIndex();
    gainIB = pow( 10.0, static_cast<qreal>( indIB + 1 ) );
    qreal gainI2 = gainIA * gainIB;

    io->setmV2mA( gainI1, 0.0, gainI2, 0.0 ); 
}

void MainWnd::slotWorkVoltChange()
{
    QString ss = "background-color: rgb(135, 100, 100);\nborder-color: rgb(135, 100, 100);";
    ui.workVolt->setStyleSheet( ss );
}

void MainWnd::slotWorkVolt()
{
    qreal val = ui.workVolt->value();
    io->setWorkMv( val );

    QString ss = "background-color: rgb(100, 135, 100);\nborder-color: rgb(100, 135, 100);";
    ui.workVolt->setStyleSheet( ss );
}

void MainWnd::slotSweepWork()
{
    qreal from   = ui.workVolt->value();
    qreal to     = ui.workSweepTo->value();
    qreal timeMs = 1000.0;

    if ( !ui.pullProbe->isChecked() )
        io->sweepWork( from, to, timeMs );
    else
    {
        qreal from2 = ui.probeVolt->value();
        qreal to2   = from2 + to - from;
        //io->sweepBoth( from, to, from2, to2, timeMs );
    }
}

void MainWnd::slotProbeVoltChange()
{
    QString ss = "background-color: rgb(135, 100, 100);\nborder-color: rgb(135, 100, 100);";
    ui.probeVolt->setStyleSheet( ss );
}

void MainWnd::slotProbeVolt()
{
    qreal val = ui.probeVolt->value();
    io->setProbeMv( val );

    QString ss = "background-color: rgb(100, 135, 100);\nborder-color: rgb(100, 135, 100);";
    ui.probeVolt->setStyleSheet( ss );
}

void MainWnd::slotSweepProbe()
{
    qreal from   = ui.probeVolt->value();
    qreal to     = ui.probeSweepTo->value();
    qreal timeMs = 1000.0;

    if ( !ui.pullWork->isChecked() )
        io->sweepProbe( from, to, timeMs );
    else
    {
        qreal from2 = ui.workVolt->value();
        qreal to2   = from2 + to - from;
        //io->sweepBoth( from2, to2, from, to, timeMs );
    }
}

void MainWnd::slotInstantValues( qreal wv, qreal pv, qreal wi, qreal pi )
{
}





