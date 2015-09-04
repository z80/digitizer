
#include "main_wnd.h"
#include <QFileDialog>
#include "sweep_wnd.h"
#include "setup_dlg.h"
#include "host_tray.h"

#include "qwt_text_label.h"

#include <boost/bind.hpp>
#include <boost/bind/arg.hpp>
#include <boost/bind/placeholders.hpp>

const QString MainWnd::SETTINGS_INI = "./settings.ini";

MainWnd::MainWnd( HostTray * parent )
    : QMainWindow( 0 ), 
      sem( 1 )
{
    m_hostTray = parent;

    ui.setupUi( this );
    //ui.osc->show();

    temperature = -273.15;
    doMeasureSweep = false;

    terminate = false;
    io        = new Bipot();
    io->calibrationLoad( "./calibration.dat" );
    setWorkV = 0.0;
    setProbeV = 0.0;

    loadSettings();
    refreshDevicesList();
    future = QtConcurrent::run( boost::bind( &MainWnd::measure, this ) );


    QVBoxLayout * bl = new QVBoxLayout( ui.osc );
    oscWork = new OscilloscopeWnd( this );
    bl->addWidget( oscWork );


    labelWork = new QwtTextLabel( oscWork->plot()->canvas() );

    textWork.setColor( Qt::darkMagenta );
    QFont f = textWork.font();
    f.setPointSizeF( 15 );
    textWork.setFont( f );
    textWork.setText( "Work I(t) [nA(s)]" );

    labelWork->setText( textWork );
    labelWork->setIndent( 0 );
    labelWork->setMargin( 10 );

    oscProbe = new OscilloscopeWnd( this );
    bl->addWidget( oscProbe );


    labelProbe = new QwtTextLabel( oscProbe->plot()->canvas() );

    textProbe.setColor( Qt::darkMagenta );
    f = textProbe.font();
    f.setPointSizeF( 15 );
    textProbe.setFont( f );
    textProbe.setText( "Probe I(t) [nA(s)]" );

    labelProbe->setText( textProbe );
    labelProbe->setIndent( 0 );
    labelProbe->setMargin( 10 );

    statusLabel = new QLabel( 0 );
    ui.statusBar->addPermanentWidget( statusLabel );

    calibrationWnd = new CalibrationWnd( 0 );
    calibrationWnd->setIo( io );

    tempTimer = new QTimer( this );
    tempTimer->setInterval( 1000 );
    connect( tempTimer, SIGNAL(timeout()), this, SLOT(slotTemp()));
    tempTimer->start();

    connect( ui.action_Quit,       SIGNAL(triggered()), m_hostTray, SLOT(slotQuit()) );
    connect( ui.actionOpen_file,   SIGNAL(triggered()), this, SLOT(slotOpen()) );
    connect( ui.action_About,      SIGNAL(triggered()), this, SLOT(slotAbout()) );
    connect( ui.actionCalibration, SIGNAL(triggered()), this, SLOT(slotCalibration()) );
    connect( ui.actionRemote_control, SIGNAL(triggered()), this, SLOT(slotRemoteSetup()) );

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
    
    connect( ui.actionExternal_trigger, SIGNAL(triggered()), this, SLOT(slotExternalTrigger()) );

    connect( ui.actionAFM_output_None,  SIGNAL(triggered()), this, SLOT(slotAfmOutput()) );
    connect( ui.actionAFM_output_work,  SIGNAL(triggered()), this, SLOT(slotAfmOutput()) );
    connect( ui.actionAFM_output_Probe, SIGNAL(triggered()), this, SLOT(slotAfmOutput()) );

    connect( ui.actionFirmware_upgrade, SIGNAL(triggered()), this, SLOT(slotFirmwareUpgrade()) );

    slotGain();

    // **********************************************
    // Remote functions.
    // **********************************************
    connect( this, SIGNAL(sigInstantValues()), this, SLOT(slotInstantValues()), Qt::QueuedConnection );
    connect( this, SIGNAL(sigSetTrigEn()),     this, SLOT(slotSetTrigEn()), Qt::QueuedConnection );
    connect( this, SIGNAL(sigValues()),        this, SLOT(slotValues()), Qt::QueuedConnection );

    listen();
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
    m_host   = s.value( "host", "" ).toString();
    m_port   = s.value( "port", 21345 ).toInt();
    m_doListen = s.value( "listen", false ).toBool();

    this->restoreState( s.value( "state", QByteArray() ).toByteArray() );
}

void MainWnd::saveSettings()
{
    QSettings s( SETTINGS_INI, QSettings::IniFormat );

    s.setValue( "devName",   devName );
    s.setValue( "host",      m_host );
    s.setValue( "port",      m_port );
    s.setValue( "listen",    m_doListen );

    s.setValue( "state", this->saveState() );
}

int MainWnd::deviceName() const
{
    return devName;
}

void MainWnd::slotQuit()
{
    saveSettings();
    m_doListen = false;
    listen();

    mutex.lock();
        terminate = true;
    mutex.unlock();
    future.waitForFinished();
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

void MainWnd::slotRemoteSetup()
{
    SetupDlg sd;
    sd.setPort( m_port );
    sd.setListen( m_doListen );
    if ( sd.exec() )
    {
        m_host = sd.host();
        if ( m_host == "All interfaces" )
            m_host = "";
        m_port = sd.port();
        m_doListen = sd.listen();
        
        if ( m_doListen )
            this->listen();
        else
        {
            if ( m_thread )
                m_thread->shutdown();
        }
    }    
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
    hide();
    e->ignore();
}

void MainWnd::setTitle( const QString & stri )
{
    statusLabel->setText( QString( "Potentiostat: %1" ).arg( stri ) );
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
            res = io->oscData( t_workV, t_probeV, t_workI, t_probeI );
            if ( !res )
            {
                Msleep::msleep( 1000 );
                reopen();
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
                    Msleep::msleep( 1000 );
                    reopen();
                    continue;
                }
                emit sigInstantValues( workV, probeV, workI, probeI );
                emit sigReplot();
            }

            // Sweep measure routine in the same thread.
            bool measured = measureSweep();

            if ( ( szMeasured < 24 ) && ( !measured ) )
                Msleep::msleep( 10 );
        }
        else
            Msleep::msleep( 100 );
        mutex.lock();
            term = terminate;
        mutex.unlock();
    } while ( !term );
}

bool MainWnd::measureSweep()
{
    mutex.lock();
        bool measure = doMeasureSweep;
    mutex.unlock();

    if ( measure )
    {
        Bipot * io;
        mutex.lock();
            io = this->io;
        mutex.unlock();

        if ( !io->isOpen() )
        {
            return false;
        }

        bool res;
        bool sweep;
        res = io->sweepEn( sweep );
        if ( !res )
        {
            Msleep::msleep( 1000 );
            reopen();
            return false;
        }


        // Measure data.
        res = io->sweepData( t_swWorkV, t_swProbeV, t_swWorkI, t_swProbeI );
        if ( !res )
        {
            Msleep::msleep( 1000 );
            reopen();
            return false;
        }

        // Check total data cnt in all arrays.
        int szMeasured = t_swWorkV.size() + t_swWorkI.size() + t_swProbeV.size() + t_swProbeI.size();
        // Now move data to paint data.
        mutexSw.lock();
            for ( int i=0; i<t_swWorkV.size(); i++ )
                p_swWorkV.enqueue( t_swWorkV.at( i ) );
            for ( int i=0; i<t_swWorkI.size(); i++ )
                p_swWorkI.enqueue( t_swWorkI.at( i ) );
            for ( int i=0; i<t_swProbeV.size(); i++ )
                p_swProbeV.enqueue( t_swProbeV.at( i ) );
            for ( int i=0; i<t_swProbeI.size(); i++ )
                p_swProbeI.enqueue( t_swProbeI.at( i ) );
            int szCollected = p_swWorkV.size() + p_swWorkI.size() + p_swProbeV.size() + p_swProbeI.size();
        mutexSw.unlock();
        // Replot if necessary.
        if ( szCollected > 12 )
        {
            emit sigSweepReplot();
        }

        if ( !sweep )
        {
            mutex.lock();
                doMeasureSweep = false;
            mutex.unlock();
            emit sigSweepFinished();
        }
        else
            Msleep::msleep( 10 );

        return true;
    }
    return false;
}

void MainWnd::reopen()
{
    QMutexLocker lock( &mutexReopen );
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
    qreal gainWorkV = 1000.0 * pow( 10.0, static_cast<qreal>( indV ) );
    int indIA = ui.workCurrGainA->currentIndex();
    qreal gainIA = pow( 10.0, static_cast<qreal>( indIA + 3 ) );
    int indIB = ui.workCurrGainB->currentIndex();
    qreal gainIB = pow( 10.0, static_cast<qreal>( indIB + 1 ) );
    qreal gainI1 = 1000000000.0 / (gainIA * gainIB);

    indV  = ui.probeVoltGain->currentIndex();
    qreal gainProbeV = 1000.0 * pow( 10.0, static_cast<qreal>( indV ) );
    indIA = ui.probeCurrGainA->currentIndex();
    gainIA = pow( 10.0, static_cast<qreal>( indIA + 3 ) );
    indIB = ui.probeCurrGainB->currentIndex();
    gainIB = pow( 10.0, static_cast<qreal>( indIB + 1 ) );
    qreal gainI2 = 1000000000.0 / (gainIA * gainIB);

    io->setmV2mA( 0.0, gainI1, 0.0, gainI2 ); 
    io->setVoltScale( gainWorkV, gainProbeV );
    //io->setVoltScale( 1.0, 1.0 ); // Voltage is measured as is without amplification.
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

        setWorkV = val;

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
        qreal timeMs = delta / ui.sweepRate->value();
        timeMs = (timeMs >= 0.0) ? timeMs : (-timeMs);
        int   ptsCnt = ui.sweepPtsCnt->value();

        bool pull = ui.pullProbe->isChecked();
        qreal probeTo = ( pull ) ? (probeFrom + delta) : probeFrom;

        bool res = io->setSweepRange( workTo, probeTo );
        if ( !res )
        {
            QString stri = QString( "Failed to set sweep range!" );
            QMessageBox::critical( this, "Error", stri );
            return;
        }

        res = io->setSweepTime( ptsCnt, timeMs );
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
        QMutexLocker lock( &mutex );
            doMeasureSweep = true;
        
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

        setProbeV = val;

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
        qreal timeMs = delta / ui.sweepRate->value();
        timeMs = (timeMs >= 0.0) ? timeMs : (-timeMs);
        int   ptsCnt = ui.sweepPtsCnt->value();

        bool pull = ui.pullWork->isChecked();
        qreal workTo = ( pull ) ? (workFrom + delta) : workFrom;

        bool res = io->setSweepRange( workTo, probeTo );
        if ( !res )
        {
            QString stri = QString( "Failed to set sweep range!" );
            QMessageBox::critical( this, "Error", stri );
            return;
        }

        res = io->setSweepTime( ptsCnt, timeMs );
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
        QMutexLocker lock( &mutex );
            doMeasureSweep = true;
    }
}

void MainWnd::slotInstantValues( qreal wv, qreal pv, qreal wi, qreal pi )
{
    QString stri= QString( "T: %1[C], workV = %2, workI = %3, probeV = %4, probeI = %5" ).arg( temperature, 5, 'f', 2, QChar( '0' ) ) 
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
        oscWork->addData( p_workI );
        oscProbe->addData( p_probeI );
        p_workV.clear();
        p_probeV.clear();
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
        {
            io->setTemperature( t );
            mutex.lock();
                bool measure = doMeasureSweep;
            mutex.unlock();
            // If it isn't sweeping at the moment adjust steady voltages.
            if ( ( !measure ) && ( !calibrationWnd->isVisible() ) )
            {
                io->setWorkMv( setWorkV );
                io->setProbeMv( setProbeV );
            }
        }
        temperature = t;
    }
}

void MainWnd::slotOscPeriod()
{
    QAction * a = qobject_cast<QAction *>( sender() );
    qreal t = 10.0;
    if ( a == ui.actionPeriod1s )
    {
        t = 3.0;
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
    {
        QMutexLocker lock( &mutex );
            doMeasureSweep = false;
            // If terminated, sweep should be stopped.
            io->setSweepEn( false );
            // Read while something is read.
            int cnt = 0;
            do {
                bool res = io->sweepData( t_swWorkV, t_swWorkI, t_swProbeV, t_swProbeI );
                Msleep::msleep( 10 );
                cnt = t_swWorkV.size() + t_swWorkI.size() + t_swProbeV.size() + t_swProbeI.size();
            } while ( cnt > 0 );
    }
    ui.dockWidget_2->setEnabled( true );
}

void MainWnd::slotOpen()
{
    sweepWnd = SweepWnd::loadFile();
    if ( sweepWnd )
        sweepWnd->show();
}

void MainWnd::slotExternalTrigger()
{
    bool en = ui.actionExternal_trigger->isChecked();
    bool res;
    {
        QMutexLocker lock( &mutex );
            res = io->isOpen();
            if ( res )
                res = io->setTriggerEn( en );
    }
    if ( !res )
    {
        QMessageBox::critical( this, "Error", "Failed to change external triggering option!" );
    }

    // If trigger is enabled turn all user controlls off.
    ui.dockWidget_2->setEnabled( !en );
}

void MainWnd::slotAfmOutput()
{
    QAction * a = qobject_cast<QAction *>( sender() );
    ui.actionAFM_output_None->setChecked( false );
    ui.actionAFM_output_work->setChecked( false );
    ui.actionAFM_output_Probe->setChecked( false );

    bool res;
    {
        QMutexLocker lock( &mutex );
            res = io->isOpen();
    }
    if ( !res )
    {
        QMessageBox::critical( this, "Error", "No hardware available!" );
        return;
    }

    if ( a == ui.actionAFM_output_None )
    {
        res = io->setOutput( 0 );
    }
    else if ( a == ui.actionAFM_output_work )
    {
        res = io->setOutput( 1 );
    }
    else
    {
        res = io->setOutput( 2 );
    }

    if ( !res )
    {
        QMessageBox::critical( this, "Error", "Failed to change AFM input signal!" );
        return;
    }

    a->setChecked( true );
}

void MainWnd::slotFirmwareUpgrade()
{
    disconnect( this,      SIGNAL(sigReplot()), this, SLOT(slotReplot()) );
    disconnect( tempTimer, SIGNAL(timeout()),   this, SLOT(slotTemp()) );
    qApp->processEvents();

    QMutexLocker lock( &mutex );
    do {
        if ( !io->isOpen() )
        {
            reopen();
            if ( !io->isOpen() )
            {
                QMessageBox::critical( this, "Error", "Falied to get access to hardware! It might be it isn't even connected." );
                return;
            }
        }

        QString stri;

        QString fileName = QFileDialog::getOpenFileName( this,
                        tr("Open firmware file"), "", tr("Text Files (*.bin)") );
        if ( fileName.length() > 0 )
        {
            stri = QString( "Press \'Ok\' button to upgrade firmware." );
            QMessageBox::StandardButton choise =QMessageBox::warning( this, "Directions", stri, QMessageBox::Ok | QMessageBox::Cancel );
            if ( choise != QMessageBox::Ok )
                break;

            bool res = io->runBootloader();
            if ( !res )
            {
                QMessageBox::critical( this, "Error", "Falied to run bootloader!" );
                break;
            }

            QString version;
            res = io->bootloaderFirmwareVersion( version );
            if ( !res )
            {
                QMessageBox::critical( this, "Error", "Falied to retrive bootloader version!" );
                break;
            }

            res = io->firmwareUpgrade( fileName );
            if ( !res )
            {
                QMessageBox::critical( this, "Error", "Firmware upgrade has failed! But don't worry, hardware is not bricked after that. Bootloader fires up each time hardware is powered on and waits for instructions for a few seconds. In order to complete firmware upgrade one should fire up firmware upgrade just after powering haddware up." );
                break;
            }

            QString stri = io->firmwareVersion();
            if ( stri.length() < 1 )
            {
                stri = QString( "Bootloader version: %1. Firmware upgrade most probably succeded. But can\'t retrieve new firmware version." ).arg( version );
                QMessageBox::warning( this, "Warning", stri );
                break;
            }
            stri = QString( "Bootloader version: %1. Firmware was successfully upgraded. New firmware version is: %2. Please, take into account that due to upgrade all values are reset to their defaults. So user interface might not reflect hardware state properly. In order to make things match reality please manually set all potentials again." ).arg( version ).arg( stri );
            QMessageBox::information( this, "Success", stri );
        }
    } while ( false );

    connect( this,      SIGNAL(sigReplot()), this, SLOT(slotReplot()) );
    connect( tempTimer, SIGNAL(timeout()),   this, SLOT(slotTemp()) );
}









bool MainWnd::iceInstantValues( qreal & workV, qreal & workI, qreal & probeV, qreal & probeI )
{
    sem.acquire();
    emit sigInstantValues();
    sem.acquire();
    sem.release();

    workV  = this->workV;
    workI  = this->workI;
    probeV = this->probeV;
    probeI = this->probeI;

    return this->result;
}

bool MainWnd::iceSetTrigEn( bool en )
{
    sem.acquire();
    this->trigEn = en;
    emit sigSetTrigEn();
    sem.acquire();
    sem.release();

    return this->result;
}

bool MainWnd::iceValues( std::vector<qreal> & workV, std::vector<qreal> & workI, std::vector<qreal> & probeV, std::vector<qreal> & probeI )
{
    sem.acquire();
    emit sigValues();
    sem.acquire();
    sem.release();

    workV  = this->vWorkV;
    workI  = this->vWorkI;
    probeV = this->vProbeV;
    probeI = this->vProbeI;

    return this->result;
}























void MainWnd::listen()
{
    if ( m_thread )
    {
        bool started = m_thread->isAlive();
        if ( started )
            m_thread->shutdown();
    }
    if ( m_doListen )
    {
        std::ostringstream os;
        if ( m_host.length() > 0 )
            os << "tcp -h " << m_host.toStdString() << " -p " << m_port;
        else
            os << "tcp" << " -p " << m_port;
        m_thread = new ThreadIce( os.str() );
        if ( !m_thread->listen( this ) )
            setTrayToolTip( "Not in service" );
        else
        {
            QString stri;
            if ( m_host.length() > 0 )
                stri = QString( "Listening interface %1, port %2" ).arg( m_host ).arg( m_port );
            else
                stri = QString( "Listening port %1" ).arg( m_port );
            setTrayToolTip( stri );
        }
    }
}

void MainWnd::setTrayToolTip( const QString & stri )
{
    m_hostTray->setToolTip( stri );
}

void MainWnd::slotInstantValues()
{
    if ( io->isOpen() )
    {
        bool res = io->instantData( workV, probeV, workI, probeI );
        result = res;
    }
    else
        result = false;

    sem.release();
}

void MainWnd::slotSetTrigEn()
{
    sweepRemote    = this->trigEn;
    doMeasureSweep = this->trigEn;
    ui.actionExternal_trigger->setChecked( this->trigEn );
    slotExternalTrigger();

    sem.release();
}

void MainWnd::slotValues()
{
    QMutexLocker lock( &mutexSw );
        int sz = p_swWorkV.size();
        int szN = p_swWorkI.size();
        if ( szN < sz )
            sz = szN;
        szN = p_swProbeV.size();
        if ( szN < sz )
            sz = szN;
        szN = p_swProbeI.size();
        if ( szN < sz )
            sz = szN;
        
        vWorkV.clear();
        vWorkI.clear();
        vProbeV.clear();
        vProbeI.clear();

        vWorkV.resize( sz );
        vWorkI.resize( sz );
        vProbeV.resize( sz );
        vProbeI.resize( sz );

        for ( int i=0; i<sz; i++ )
        {
            qreal v = p_swWorkV.dequeue();
            vWorkV[i] = v;

            v = p_swWorkI.dequeue();
            vWorkI[i] = v;

            v = p_swProbeV.dequeue();
            vProbeV[i] = v;

            v = p_swProbeI.dequeue();
            vProbeI[i] = v;
        }

    sem.release();
}






