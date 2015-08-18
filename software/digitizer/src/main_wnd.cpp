
#include "main_wnd.h"
#include <QFileDialog>
#include "sweep_wnd.h"

#include <boost/bind.hpp>
#include <boost/bind/arg.hpp>
#include <boost/bind/placeholders.hpp>

const QString MainWnd::SETTINGS_INI = "./settings.ini";

MainWnd::MainWnd( QWidget * parent )
    : QMainWindow( parent )
{
    ui.setupUi( this );
    //ui.osc->show();

    temperature = -273.15;

    terminate = false;
    io        = new Bipot();

    loadSettings();
    refreshDevicesList();
    future = QtConcurrent::run( boost::bind( &MainWnd::measure, this ) );


    QVBoxLayout * bl = new QVBoxLayout( ui.osc );
    oscWork = new OscilloscopeWnd( this );
    bl->addWidget( oscWork );

    oscProbe = new OscilloscopeWnd( this );
    bl->addWidget( oscProbe );

    calibrationWnd = new CalibrationWnd( 0 );
    calibrationWnd->setIo( io );

    tempTimer = new QTimer( this );
    tempTimer->setInterval( 1000 );
    connect( tempTimer, SIGNAL(timeout()), this, SLOT(slotTemp()));
    tempTimer->start();

    connect( ui.action_Quit,       SIGNAL(triggered()), this, SLOT(slotQuit()) );
    connect( ui.action_About,      SIGNAL(triggered()), this, SLOT(slotAbout()) );
    connect( ui.actionCalibration, SIGNAL(triggered()), this, SLOT(slotCalibration()) );

    connect( ui.workVoltGain,   SIGNAL(currentIndexChanged(int)), this, SLOT(slotGain()) );
    connect( ui.workCurrGainA,  SIGNAL(currentIndexChanged(int)), this, SLOT(slotGain()) );
    connect( ui.workCurrGainB,  SIGNAL(currentIndexChanged(int)), this, SLOT(slotGain()) );
    connect( ui.probeVoltGain,  SIGNAL(currentIndexChanged(int)), this, SLOT(slotGain()) );
    connect( ui.probeCurrGainA, SIGNAL(currentIndexChanged(int)), this, SLOT(slotGain()) );
    connect( ui.probeCurrGainB, SIGNAL(currentIndexChanged(int)), this, SLOT(slotGain()) );

    connect( ui.workVolt, SIGNAL(valueChanged(double)), this, SLOT(slotWorkVoltChange()) );
    connect( ui.workVolt, SIGNAL(editingFinished()),    this, SLOT(slotWorkVolt()) );
    connect( ui.sweepWork, SIGNAL(clicked()),           this, SLOT(slotSweepWork()) );

    connect( ui.probeVolt,  SIGNAL(valueChanged(double)), this, SLOT(slotProbeVoltChange()) );
    connect( ui.probeVolt,  SIGNAL(editingFinished()),    this, SLOT(slotProbeVolt()) );
    connect( ui.sweepProbe, SIGNAL(clicked()),            this, SLOT(slotSweepProbe()) );

    connect( ui.workVolt, SIGNAL(editingFinished()),    this, SLOT(slotWorkVolt()) );

    connect( this, SIGNAL(sigInstantValues(qreal,qreal,qreal,qreal)), 
             this, SLOT(slotInstantValues(qreal,qreal,qreal,qreal)), 
             Qt::QueuedConnection );
    connect( this, SIGNAL(sigReplot()), 
             this, SLOT(slotReplot()), 
             Qt::QueuedConnection );

    connect( this, SIGNAL(sigSweepReplot()),   this, SLOT(slotSweepReplot()) );
    connect( this, SIGNAL(sigSweepFinished()), this, SLOT(slotSweepFinished()) );
    connect( ui.actionStop_sweep, SIGNAL(triggered()), this, SLOT(slotStopSweep()) );

    connect( ui.actionPeriod1s,  SIGNAL(triggered()), this, SLOT(slotOscPeriod()) );
    connect( ui.actionPeriod10s, SIGNAL(triggered()), this, SLOT(slotOscPeriod()) );
    connect( ui.actionPeriod1m,  SIGNAL(triggered()), this, SLOT(slotOscPeriod()) );
    
}

MainWnd::~MainWnd()
{
    tempTimer->deleteLater();

    mutex.lock();
        terminate = true;
    mutex.unlock();
    future.waitForFinished();

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
    QString fmwVer;
    if ( io->isOpen() )
        fmwVer = io->firmwareVersion();
    else
        fmwVer = "undefined";
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
                mutex.lock();
                    term = terminate;
                mutex.unlock();
                if ( term )
                    break;
                Msleep::msleep( 1000 );
                continue;
            }

            bool res;

            // Read oscilloscope data.
            res = io->oscData( t_workV, t_workI, t_probeV, t_probeI );
            if ( !res )
            {
                io->close();
                Msleep::msleep( 1000 );
                continue;
            }

            // Check total data cnt in all arrays.
            int szMeasured = t_workV.size() + t_workI.size() + t_probeV.size() + t_probeI.size();
            // Now move data to paint data.
            mutex.lock();
                for ( int i=0; i<t_workV.size(); i++ )
                    p_workV.enqueue( t_workV.at( i ) );
                for ( int i=0; i<t_workI.size(); i++ )
                    p_workI.enqueue( t_workI.at( i ) );
                for ( int i=0; i<t_probeV.size(); i++ )
                    p_probeV.enqueue( t_probeV.at( i ) );
                for ( int i=0; i<t_probeI.size(); i++ )
                    p_probeI.enqueue( t_probeI.at( i ) );
                int szCollected = p_workV.size() + p_workI.size() + p_probeV.size() + p_probeI.size();
            mutex.unlock();
            // Replot if necessary.
            if ( szCollected > 12 )
            {
                qreal workV, probeV, workI, probeI;
                // Probably instant data shouldn't be read here.
                res = io->instantData( workV, probeV, workI, probeI );
                if ( !res )
                {
                    io->close();
                    Msleep::msleep( 1000 );
                    continue;
                }
                emit sigInstantValues( workV, probeV, workI, probeI );
                emit sigReplot();
            }
            if ( szMeasured < 24 )
                Msleep::msleep( 10 );
        }
        else
            Msleep::msleep( 100 );
        mutex.lock();
            term = terminate;
        mutex.unlock();
    } while ( !term );
}

void MainWnd::measureSweep()
{
    bool sweep = false;
    mutexSw.lock();
        swTerminate = false;
    mutexSw.unlock();
    bool term = false;

    bool res;
    do {
        Bipot * io;
        mutex.lock();
            io = this->io;
        mutex.unlock();

        mutexSw.lock();
            term = swTerminate;
        mutexSw.unlock();
        if ( term )
            break;

        if ( !io->isOpen() )
        {
            reopen();
            Msleep::msleep( 1000 );
            continue;
        }

        // Measure data.
        res = io->sweepData( t_swWorkV, t_swWorkI, t_swProbeV, t_swProbeI );
        if ( !res )
        {
            io->close();
            Msleep::msleep( 1000 );
            continue;
        }

        // Check total data cnt in all arrays.
        int szMeasured = t_swWorkV.size() + t_swWorkI.size() + t_swProbeV.size() + t_swProbeI.size();
        // Now move data to paint data.
        mutex.lock();
            for ( int i=0; i<t_swWorkV.size(); i++ )
                p_swWorkV.enqueue( t_swWorkV.at( i ) );
            for ( int i=0; i<t_swWorkI.size(); i++ )
                p_swWorkI.enqueue( t_swWorkI.at( i ) );
            for ( int i=0; i<t_swProbeV.size(); i++ )
                p_swProbeV.enqueue( t_swProbeV.at( i ) );
            for ( int i=0; i<t_swProbeI.size(); i++ )
                p_swProbeI.enqueue( t_swProbeI.at( i ) );
            int szCollected = p_swWorkV.size() + p_swWorkI.size() + p_swProbeV.size() + p_swProbeI.size();
        mutex.unlock();
        // Replot if necessary.
        if ( szCollected > 12 )
        {
            emit sigSweepReplot();
        }
        else
        {
            res = io->sweepEn( sweep );
            if ( !res )
            {
                io->close();
                Msleep::msleep( 1000 );
                continue;
            }
        }
        if ( szMeasured < 24 )
            Msleep::msleep( 10 );
    } while ( ( sweep ) && (!term) );

    if ( term )
    {
        // If terminated, sweep should be stopped.
        io->setSweepEn( false );
        // Read while something is read.
        int cnt = 0;
        do {
            res = io->sweepData( t_swWorkV, t_swWorkI, t_swProbeV, t_swProbeI );
            Msleep::msleep( 10 );
            cnt = t_swWorkV.size() + t_swWorkI.size() + t_swProbeV.size() + t_swProbeI.size();
        } while ( cnt > 0 );
    }

    emit sigSweepFinished();
}

void MainWnd::reopen()
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
    if ( io->isOpen() )
    {
        qreal val = ui.workVolt->value();
        io->setWorkMv( val );

        QString ss = "background-color: rgb(100, 135, 100);\nborder-color: rgb(100, 135, 100);";
        ui.workVolt->setStyleSheet( ss );
    }
}

void MainWnd::slotSweepWork()
{
    if ( io->isOpen() )
    {
        qreal workFrom   = ui.workVolt->value();
        qreal workTo     = ui.workSweepTo->value();
        qreal probeFrom  = ui.probeVolt->value();
        qreal delta = workTo - workFrom;
        qreal timeMs = 1000.0;

        bool pull = ui.pullProbe->isChecked();
        qreal probeTo = ( pull ) ? (probeFrom + delta) : probeFrom;

        bool res = io->setSweepRange( workTo, probeTo );
        if ( !res )
        {
            QString stri = QString( "Failed to set sweep range!" );
            QMessageBox::critical( this, "Error", stri );
            return;
        }

        res = io->setSweepTime( 1024, timeMs );
        if ( !res )
        {
            QString stri = QString( "Failed to set sweep time!" );
            QMessageBox::critical( this, "Error", stri );
            return;
        }

        res = io->setSweepEn( true );
        if ( !res )
        {
            QString stri = QString( "Failed to start sweep!" );
            QMessageBox::critical( this, "Error", stri );
            return;
        }

        ui.dockWidget_2->setEnabled( false );
        // Open sweep window display.
        sweepWnd = new SweepWnd( 0 );
        sweepWnd->show();
        // Run sweep data readout.
        sweepExec.run( boost::bind( &MainWnd::measureSweep, this ) );
        // While sweep runs continue adding points.
        while ( sweepExec.isRunning() )
            qApp->processEvents();
    }
}

void MainWnd::slotProbeVoltChange()
{
    QString ss = "background-color: rgb(135, 100, 100);\nborder-color: rgb(135, 100, 100);";
    ui.probeVolt->setStyleSheet( ss );
}

void MainWnd::slotProbeVolt()
{
    if ( io->isOpen() )
    {
        qreal val = ui.probeVolt->value();
        io->setProbeMv( val );

        QString ss = "background-color: rgb(100, 135, 100);\nborder-color: rgb(100, 135, 100);";
        ui.probeVolt->setStyleSheet( ss );
    }
}

void MainWnd::slotSweepProbe()
{
    if ( io->isOpen() )
    {
        qreal workFrom   = ui.workVolt->value();
        qreal probeFrom  = ui.probeVolt->value();
        qreal probeTo    = ui.probeSweepTo->value();
        qreal delta = probeTo - probeFrom;
        qreal timeMs = 1000.0;

        bool pull = ui.pullWork->isChecked();
        qreal workTo = ( pull ) ? (workFrom + delta) : workFrom;

        bool res = io->setSweepRange( workTo, probeTo );
        if ( !res )
        {
            QString stri = QString( "Failed to set sweep range!" );
            QMessageBox::critical( this, "Error", stri );
            return;
        }

        res = io->setSweepTime( 1024, timeMs );
        if ( !res )
        {
            QString stri = QString( "Failed to set sweep time!" );
            QMessageBox::critical( this, "Error", stri );
            return;
        }

        res = io->setSweepEn( true );
        if ( !res )
        {
            QString stri = QString( "Failed to start sweep!" );
            QMessageBox::critical( this, "Error", stri );
            return;
        }

        ui.dockWidget_2->setEnabled( false );
        // Open sweep window display.
        sweepWnd = new SweepWnd( 0 );
        sweepWnd->show();
        // Run sweep data readout.
        sweepExec.run( boost::bind( &MainWnd::measureSweep, this ) );
        // While sweep runs continue adding points.
        while ( sweepExec.isRunning() )
            qApp->processEvents();
    }
}

void MainWnd::slotInstantValues( qreal wv, qreal pv, qreal wi, qreal pi )
{
    QString stri= QString( "t: %1[C], workV = %2, workI = %3, probeV = %4, probeI = %5" ).arg( temperature, 5, 'f', 2, QChar( '0' ) ) 
                                                                                         .arg( wv, 8, 'f', 1, QChar( '0' ) ) 
                                                                                         .arg( wi, 8, 'f', 1, QChar( '0' ) )
                                                                                         .arg( pv, 8, 'f', 1, QChar( '0' ) )
                                                                                         .arg( pi, 8, 'f', 1, QChar( '0' ) );

    setTitle( stri );
}

void MainWnd::slotReplot()
{
    // Add data to oscilloscope windows.
    mutex.lock();
        oscWork->addData( p_workV );
        oscProbe->addData( p_probeV );
        p_workI.clear();
        p_probeI.clear();
    mutex.unlock();
    // Replot oscilloscope windows.
    oscWork->slotReplot();
    oscProbe->slotReplot();
}

void MainWnd::slotTemp()
{
    bool res = io->isOpen();
    if ( res )
    {
        qreal t;
        res = io->temperature( t );
        if ( res )
            io->setTemperature( t );
        temperature = t;
    }
}

void MainWnd::slotOscPeriod()
{
    QAction * a = qobject_cast<QAction *>( sender() );
    qreal t = 10.0;
    if ( a == ui.actionPeriod1s )
    {
        t = 1.0;
    }
    else if ( a == ui.actionPeriod10s )
    {
        t = 10.0;
    }
    else if ( a == ui.actionPeriod1m )
    {
        t = 60.0;
    }
    oscWork->setPeriod( t );
    oscProbe->setPeriod( t );
    bool res = io->setOscPeriod( 1024, t * 1000.0 );
    if ( !res )
    {
        QString stri = QString( "Failed to set oscilloscope period!" );
        QMessageBox::critical( this, "Error", stri );
        return;
    }
}

void MainWnd::slotSweepReplot()
{
    if ( sweepWnd )
        sweepWnd->addData( mutexSw, p_swWorkV, p_swWorkI, p_swProbeV, p_swProbeI );
}

void MainWnd::slotSweepFinished()
{
    ui.dockWidget_2->setEnabled( true );
}

void MainWnd::slotStopSweep()
{
    mutexSw.lock();
        swTerminate = true;
    mutexSw.unlock();
}






