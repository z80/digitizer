
#include "main_wnd.h"
#include <QFileDialog>
#include "sweep_wnd.h"
#include "setup_dlg.h"
#include "polarization_dlg.h"
#include "host_tray.h"
#include "help_main_wnd.h"
#include "version.h"

#include "qwt_text_label.h"

#include <boost/bind.hpp>
#include <boost/bind/arg.hpp>
#include <boost/bind/placeholders.hpp>

const QString MainWnd::SETTINGS_INI = "./settings.ini";

MainWnd::MainWnd( HostTray * parent )
    : QMainWindow( 0 ), 
      m_helpBrowser( 0 ), 
      sem( 1 )
{
    m_hostTray = parent;

    ui.setupUi( this );
    //ui.osc->show();

    totalPts       = 0;
    temperature = -273.15;
    doMeasureSweep = false;
    doMeasureOsc   = false;
    sweepRemote    = false;

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
    textWork.setText( "Electrode 1, I(t) [Amp of sec]" );

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
    textProbe.setText( "Electrode 2, I(t) [Amp of sec]" );

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
    connect( ui.actionHelp,        SIGNAL(triggered()), this, SLOT(slotHelp()) );
    connect( ui.action_About,      SIGNAL(triggered()), this, SLOT(slotAbout()) );
    connect( ui.actionCalibration, SIGNAL(triggered()), this, SLOT(slotCalibration()) );
    connect( ui.actionRemote_control, SIGNAL(triggered()), this, SLOT(slotRemoteSetup()) );

    connect( ui.sweepPtsCnt, SIGNAL(editingFinished()), this, SLOT(slotSweepPts()) );
    connect( ui.sweepRate,   SIGNAL(editingFinished()), this, SLOT(slotSweepRate()) );

    connect( ui.workVoltGain,   SIGNAL(currentIndexChanged(int)), this, SLOT(slotGain()) );
    connect( ui.workCurrGain,   SIGNAL(currentIndexChanged(int)), this, SLOT(slotGain()) );
    connect( ui.probeVoltGain,  SIGNAL(currentIndexChanged(int)), this, SLOT(slotGain()) );
    connect( ui.probeCurrGain,  SIGNAL(currentIndexChanged(int)), this, SLOT(slotGain()) );

    connect( ui.workVolt, SIGNAL(valueChanged(double)), this, SLOT(slotWorkVoltChange()) );
    connect( ui.workVolt, SIGNAL(editingFinished()),    this, SLOT(slotWorkVolt()) );
    connect( ui.sweepWork, SIGNAL(clicked()),           this, SLOT(slotSweepWork()) );

    connect( ui.probeVolt,  SIGNAL(valueChanged(double)), this, SLOT(slotProbeVoltChange()) );
    connect( ui.probeVolt,  SIGNAL(editingFinished()),    this, SLOT(slotProbeVolt()) );
    connect( ui.sweepProbe, SIGNAL(clicked()),            this, SLOT(slotSweepProbe()) );

    connect( ui.workVolt, SIGNAL(editingFinished()),    this, SLOT(slotWorkVolt()) );

    // Polarization dialog.
    connect( ui.actionPolarization, SIGNAL(triggered()), this, SLOT(slotPolarization()) );

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

    connect( ui.workVertex1En,  SIGNAL(clicked(bool)), this, SLOT(slotVertexEnChanged()) );
    connect( ui.workVertex2En,  SIGNAL(clicked(bool)), this, SLOT(slotVertexEnChanged()) );
    connect( ui.probeVertex1En, SIGNAL(clicked(bool)), this, SLOT(slotVertexEnChanged()) );
    connect( ui.probeVertex2En, SIGNAL(clicked(bool)), this, SLOT(slotVertexEnChanged()) );


    flipPotA = new QAction( "Invert potential 1", this );
    flipCurA = new QAction( "Invert current 1", this ); 
    flipPotB = new QAction( "Invert potential 2", this ); 
    flipCurB = new QAction( "Invert current 2", this );  
    flipOutA = new QAction( "Invert out 1", this );
    flipOutB = new QAction( "Invert out 2", this );
    flipPotA->setCheckable( true );
    flipPotA->setChecked( invSampleV );
    flipCurA->setCheckable( true );
    flipCurA->setChecked( invSampleI );
    flipPotB->setCheckable( true );
    flipPotB->setChecked( invProbeV );
    flipCurB->setCheckable( true );
    flipCurB->setChecked( invProbeI );
    flipOutA->setCheckable( true );
    flipOutA->setChecked( invOutA );
    flipOutB->setCheckable( true );
    flipOutB->setChecked( invOutB );
    connect( flipPotA, SIGNAL(triggered()), this, SLOT(slotGain()) );
    connect( flipCurA, SIGNAL(triggered()), this, SLOT(slotGain()) );
    connect( flipPotB, SIGNAL(triggered()), this, SLOT(slotGain()) );
    connect( flipCurB, SIGNAL(triggered()), this, SLOT(slotGain()) );
    connect( flipOutA, SIGNAL(triggered()), this, SLOT(slotGain()) );
    connect( flipOutB, SIGNAL(triggered()), this, SLOT(slotGain()) );

    slotGain();
    slotVertexEnChanged();

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

    ui.workCurrGain->setCurrentIndex( s.value( "workCurrGain", 0 ).toInt() );
    ui.workVoltGain->setCurrentIndex( s.value( "workVoltGain", 0 ).toInt() );
    ui.workVolt->setValue( s.value( "workVolt", 0.0 ).toDouble() );
    ui.workVertex1->setValue( s.value( "workVertex1", 0.0 ).toDouble() );
    ui.workVertex1En->setChecked( s.value( "workVertex1En", false ).toBool() );
    ui.workVertex2->setValue( s.value( "workVertex2", 0.0 ).toDouble() );
    ui.workVertex2En->setChecked( s.value( "workVertex2En", false ).toBool() );
    ui.workSweepTo->setValue( s.value( "workSweepTo", 0.0 ).toDouble() );
    ui.pullProbe->setChecked( s.value( "pullProbe", false ).toBool() );

    ui.probeCurrGain->setCurrentIndex( s.value( "probeCurrGain", 0 ).toInt() );
    ui.probeVoltGain->setCurrentIndex( s.value( "probeVoltGain", 0 ).toInt() );
    ui.probeVolt->setValue( s.value( "probeVolt", 0.0 ).toDouble() );
    ui.probeVertex1->setValue( s.value( "probeVertex1", 0.0 ).toDouble() );
    ui.probeVertex1En->setChecked( s.value( "probeVertex1En", false ).toBool() );
    ui.probeVertex2->setValue( s.value( "probeVertex2", 0.0 ).toDouble() );
    ui.probeVertex2En->setChecked( s.value( "probeVertex2En", false ).toBool() );
    ui.probeSweepTo->setValue( s.value( "probeSweepTo", 0.0 ).toDouble() );
    ui.pullWork->setChecked( s.value( "pullWork", false ).toBool() );

    ui.sweepPtsCnt->setValue( s.value( "sweepPtsCnt", 100 ).toInt() );
    ui.sweepRate->setValue( s.value( "sweepRate", 0.1 ).toDouble() );
    ui.sweepDacMode->setChecked( s.value( "dacMode", false ).toBool() );

    ui.cyclesCnt->setValue( s.value( "cyclesCnt", 1 ).toInt() );

    polarizationPotential = s.value( "polarizationPotential", 0.0 ).toDouble();
    polarizationDuration  = s.value( "polarizationDuration",  1.0 ).toDouble();
    measureDuration       = s.value( "measureDuration", 5.0 ).toDouble();

    this->restoreState( s.value( "state", QByteArray() ).toByteArray() );

    invSampleV = s.value( "invSampleV", false ).toBool();
    invSampleI = s.value( "invSampleI", false ).toBool(); 
    invProbeV  = s.value( "invProbeV",  false ).toBool();
    invProbeI  = s.value( "invProbeI",  false ).toBool();
    invOutA    = s.value( "invOutA",    false ).toBool();
    invOutB    = s.value( "invOutB",    false ).toBool();

    polarizationIndex = s.value( "polarizationIndex", 0 ).toInt();
    polarizationPull  = s.value( "polarizationPull",  false ).toBool();

    setWorkV  = ui.workVolt->value();
    setProbeV = ui.probeVolt->value();
}

void MainWnd::saveSettings()
{
    QSettings s( SETTINGS_INI, QSettings::IniFormat );

    s.setValue( "devName",   devName );
    s.setValue( "host",      m_host );
    s.setValue( "port",      m_port );
    s.setValue( "listen",    m_doListen );

    s.setValue( "workCurrGain", ui.workCurrGain->currentIndex() );
    s.setValue( "workVoltGain", ui.workVoltGain->currentIndex() );
    s.setValue( "workVolt", ui.workVolt->value() );
    s.setValue( "workVertex1", ui.workVertex1->value() );
    s.setValue( "workVertex1En", ui.workVertex1En->isChecked() );
    s.setValue( "workVertex2", ui.workVertex2->value() );
    s.setValue( "workVertex2En", ui.workVertex2En->isChecked() );
    s.setValue( "workSweepTo", ui.workSweepTo->value() );
    s.setValue( "pullProbe", ui.pullProbe->isChecked() );

    s.setValue( "probeCurrGain", ui.probeCurrGain->currentIndex() );
    s.setValue( "probeVoltGain", ui.probeVoltGain->currentIndex() );
    s.setValue( "probeVolt", ui.probeVolt->value() );
    s.setValue( "probeVertex1", ui.probeVertex1->value() );
    s.setValue( "probeVertex1En", ui.probeVertex1En->isChecked() );
    s.setValue( "probeVertex2", ui.probeVertex2->value() );
    s.setValue( "probeVertex2En", ui.probeVertex2En->isChecked() );
    s.setValue( "probeSweepTo", ui.probeSweepTo->value() );
    s.setValue( "pullWork", ui.pullWork->isChecked() );

    s.setValue( "sweepPtsCnt", ui.sweepPtsCnt->value() );
    s.setValue( "sweepRate", ui.sweepRate->value() );
    s.setValue( "dacMode", ui.sweepDacMode->isChecked() );

    s.setValue( "cyclesCnt", ui.cyclesCnt->value() );

    s.setValue( "polarizationPotential", polarizationPotential );
    s.setValue( "polarizationDuration",  polarizationDuration );
    s.setValue( "measureDuration",       measureDuration );

    s.setValue( "state", this->saveState() );

    s.setValue( "invSampleV", invSampleV );
    s.setValue( "invSampleI", invSampleI ); 
    s.setValue( "invProbeV",  invProbeV );
    s.setValue( "invProbeI",  invProbeI );
    s.setValue( "invOutA",    invOutA );
    s.setValue( "invOutB",    invOutB );

    s.setValue( "polarizationIndex", polarizationIndex );
    s.setValue( "polarizationPull",  polarizationPull );
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

void MainWnd::slotHelp()
{
    if ( !m_helpBrowser )
        m_helpBrowser = new HelpMainWnd( "./help.qhc" );
    m_helpBrowser->show();
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

    QString stri = QString( "Bipotentiostat control module: fw ver: \"<b>%1</b>\", sw ver: \"<b>%2</b>\"" ).arg( fmwVer ).arg( SOFTWARE_VERSION );
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

void MainWnd::contextMenuEvent( QContextMenuEvent * e )
{
    QMenu menu( this );
    menu.addAction( flipPotA );
    menu.addAction( flipCurA );
    menu.addAction( flipPotB );
    menu.addAction( flipCurB );

    menu.addAction( flipOutA );
    menu.addAction( flipOutB );

    menu.exec( e->globalPos() );
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
                if ( !io->isOpen() )
                {
                    Msleep::msleep( 1000 );
                    continue;
                }
            }

            bool res;

            measureSweep();

            // Read oscilloscope data.
            res = io->oscData( t_workV, t_probeV, t_workI, t_probeI );
            if ( !res )
            {
                Msleep::msleep( 10 );
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
                emit sigReplot();

            // Sweep measure routine in the same thread.
            bool measured = measureSweep();

            if ( ( szMeasured < 24 ) && ( !measured ) )
                Msleep::msleep( 50 );
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
        int szMeasured;
        int szCollected;
        do {
            // Measure data.
            res = io->sweepData( t_swWorkV, t_swProbeV, t_swWorkI, t_swProbeI, sweepDacMode );
            if ( !res )
            {
                Msleep::msleep( 1000 );
                reopen();
                return false;
            }

            // Check total data cnt in all arrays.
            szMeasured = t_swWorkV.size() + t_swWorkI.size() + t_swProbeV.size() + t_swProbeI.size();
            mutex.lock();
                totalPts += szMeasured;
            mutex.unlock();
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
                szCollected = p_swWorkV.size() + p_swWorkI.size() + p_swProbeV.size() + p_swProbeI.size();
            mutexSw.unlock();
        } while ( szMeasured > 32 );

        bool sweep;
        // Replot if necessary.
        if ( szCollected > 12 )
        {
            emit sigSweepReplot();
            sweep = true;
        }
        else
        {
            res = io->sweepEn( sweep );
            if ( !res )
            {
                Msleep::msleep( 1000 );
                reopen();
                return false;
            }
        }

        mutex.lock();
            bool sweepRem = sweepRemote;
        mutex.unlock();
        if ( ( !sweep ) && ( !sweepRem ) )
        {
            int szCollected = 0;
            int cnt = 0;
            do {
                bool res = io->sweepData( t_swWorkV, t_swProbeV, t_swWorkI, t_swProbeI );
                Msleep::msleep( 10 );
                cnt = t_swWorkV.size() + t_swWorkI.size() + t_swProbeV.size() + t_swProbeI.size();

                mutex.lock();
                    totalPts += cnt;
                mutex.unlock();

                mutexSw.lock();
                    for ( int i=0; i<t_swWorkV.size(); i++ )
                        p_swWorkV.enqueue( t_swWorkV.at( i ) );
                    for ( int i=0; i<t_swWorkI.size(); i++ )
                        p_swWorkI.enqueue( t_swWorkI.at( i ) );
                    for ( int i=0; i<t_swProbeV.size(); i++ )
                        p_swProbeV.enqueue( t_swProbeV.at( i ) );
                    for ( int i=0; i<t_swProbeI.size(); i++ )
                        p_swProbeI.enqueue( t_swProbeI.at( i ) );
                    szCollected = p_swWorkV.size() + p_swWorkI.size() + p_swProbeV.size() + p_swProbeI.size();
                mutexSw.unlock();
            } while ( cnt > 0 );
            if ( szCollected > 0 )
                emit sigSweepReplot();

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
        // If opened turn LEDs off.
        if ( io->isOpen() )
            io->setLed( 0 );
}

void MainWnd::clearSweepConfig()
{
    ptsCntQueue.clear();
    periodQueue.clear();
    workVQueue.clear();
    probeVQueue.clear();
}

void MainWnd::pushSweepConfig( qreal workTo, qreal probeTo )
{
    int ptsPerVolt = ui.sweepPtsCnt->value();
    qreal rate = ui.sweepRate->value();

    qreal workFrom  = (workVQueue.size() > 0)  ? workVQueue.last()  : ui.workVolt->value();
    qreal probeFrom = (probeVQueue.size() > 0) ? probeVQueue.last() : ui.probeVolt->value();
    qreal workDiff  = (workTo - workFrom);
    qreal probeDiff = (probeTo - probeFrom);
    workDiff = (workDiff >= 0.0) ? workDiff : (-workDiff);
    probeDiff = (probeDiff >= 0.0) ? probeDiff : (-probeDiff);
    qreal diff = ( workDiff > probeDiff ) ? workDiff : probeDiff;
    qreal timeMs = diff / rate * 1000.0;
    int ptsCnt = static_cast<int>( diff * static_cast<qreal>( ptsPerVolt ) / 1000.0 );
    if ( ptsCnt < 2 )
        return;

    ptsCntQueue.enqueue( ptsCnt );
    periodQueue.enqueue( timeMs );
    workVQueue.enqueue( workTo );
    probeVQueue.enqueue( probeTo );
}

bool MainWnd::runSweep()
{
    p_swWorkV.clear();
    p_swWorkI.clear();
    p_swProbeV.clear();
    p_swProbeI.clear();

    sweepDacMode = ui.sweepDacMode->isChecked();
    bool res = io->setSweepDacMode( sweepDacMode );
    if ( !res )
    {
        QString stri = QString( "Failed to set potential transition options!" );
        QMessageBox::critical( this, "Error", stri );
        return false;
    }

    bool firstTransition = true;
    qDebug() << "Queue size: " << ptsCntQueue.size();
    while ( ptsCntQueue.size() > 0 )
    {
        qDebug() << "A) Queue size: " << ptsCntQueue.size();
        int ptsCnt = ptsCntQueue.dequeue();
        qreal timeMs = periodQueue.dequeue();
        qreal workTo = workVQueue.dequeue();
        qreal probeTo = probeVQueue.dequeue();
        qDebug() << "B) Queue size: " << ptsCntQueue.size();
        res = io->sweepPush( ptsCnt, timeMs, outA( workTo ), outB( probeTo ) );
        qDebug() << "C) Queue size: " << ptsCntQueue.size();
        if ( !res )
        {
            QString stri = QString( "Failed to set potential transition!" );
            QMessageBox::critical( this, "Error", stri );
            return false;
        }
        qDebug() << "Pushed transition";

        if ( firstTransition )
        {
            firstTransition = false;
            res = io->setSweepEn( true );
            if ( !res )
            {
                QString stri = QString( "Failed to start potential transition!" );
                QMessageBox::critical( this, "Error", stri );
                return false;
            }

            ui.dockWidget_2->setEnabled( false );
            // Open sweep window display.
            sweepWnd = new SweepWnd( 0 );
            sweepWnd->show();
            // Run sweep data readout.
            QMutexLocker lock( &mutex );
                doMeasureSweep = true;
        }

        // Wait if queue is too long.
        qDebug() << "Wait for transition";
        sweepDelay( static_cast<int>( timeMs ) );
        qDebug() << "Done waiting";
    }

    qDebug() << "Queue size: " << ptsCntQueue.size();
    clearSweepConfig();

    return true;
}

void MainWnd::sweepDelay( int ms )
{
    const int MIN_QTY = 16;
    const int DELAY = ms;
    int sz = 0;
    do {
        bool res = io->sweepQueueSize( sz );
        if ( sz <= MIN_QTY )
            break;
        QTime time;
        time.start();
        while ( time.elapsed() < DELAY )
            qApp->processEvents();
    } while ( sz > MIN_QTY );
}

void MainWnd::polarizationDelay( int ms )
{
    const int MIN_QTY = 300;
    const int DELAY = ms;
    int sz = 0;
    QTime time;
    time.start();
    while ( time.elapsed() < DELAY )
        qApp->processEvents();
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
    invSampleV = flipPotA->isChecked();
    invSampleI = flipCurA->isChecked();
    invProbeV  = flipPotB->isChecked();
    invProbeI  = flipCurB->isChecked();
    invOutA    = flipOutA->isChecked();
    invOutB    = flipOutB->isChecked();

    int indV  = ui.workVoltGain->currentIndex();
    qreal gainWorkV = pow( 10.0, -static_cast<qreal>( indV ) );
    if ( invSampleV )
        gainWorkV = -gainWorkV;

    int indIA = ui.workCurrGain->currentIndex();
    qreal gainI = pow( 10.0, static_cast<qreal>( indIA + 3 ) );
    qreal gainI1 = 0.001 / gainI;
    if ( invSampleI )
        gainI1 = -gainI1;

    indV  = ui.probeVoltGain->currentIndex();
    qreal gainProbeV = pow( 10.0, -static_cast<qreal>( indV ) );
    if ( invProbeV )
        gainProbeV = -gainProbeV;

    indIA = ui.probeCurrGain->currentIndex();
    gainI = pow( 10.0, static_cast<qreal>( indIA + 3 ) );
    qreal gainI2 = 0.001 / gainI;
    if ( invProbeI )
        gainI2 = -gainI2;

    io->setmV2mA( 0.0, gainI1, 0.0, gainI2 );
    io->setVoltScale( gainWorkV, gainProbeV );
}

// Points per second limit.
static const qreal PTS_LIMIT = 500.0;

void MainWnd::slotSweepPts()
{
    int pts = ui.sweepPtsCnt->value();
    qreal speed = ui.sweepRate->value();
    qreal pts_per_sec = static_cast<int>( static_cast<qreal>( pts ) * speed / 1000.0 );
    if ( pts_per_sec > PTS_LIMIT )
        ui.sweepRate->setValue( PTS_LIMIT / static_cast<qreal>( pts ) * 1000.0 );
}

void MainWnd::slotSweepRate()
{
    int pts = ui.sweepPtsCnt->value();
    qreal speed = ui.sweepRate->value();
    qreal pts_per_sec = static_cast<int>( static_cast<qreal>( pts ) * speed / 1000.0 );
    if ( pts_per_sec > PTS_LIMIT )
        ui.sweepPtsCnt->setValue( static_cast<int>( PTS_LIMIT / speed  * 1000.0 ) );
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
        io->setWorkMv( outA( val ) );

        setWorkV = val;

        QString ss = "background-color: rgb(100, 135, 100);\nborder-color: rgb(100, 135, 100);";
        ui.workVolt->setStyleSheet( ss );
    }
}

void MainWnd::slotSweepWork()
{
    if ( io->isOpen() )
    {
        clearSweepConfig();

        bool pull = ui.pullProbe->isChecked();

        qreal workAt  = ui.workVolt->value();
        qreal probeAt = ui.probeVolt->value();
        int   qty     = ui.cyclesCnt->value();
        if ( ( !ui.workVertex1En->isChecked() ) || ( !ui.workVertex2En->isChecked() ) )
            qty = 1;
        for ( int i=0; i<qty; i++ )
        {
            if ( ui.workVertex1En->isChecked() )
            {
                qreal wv = ui.workVertex1->value();
                qreal pv = ( pull ) ? (probeAt+wv-workAt) : probeAt;
                pushSweepConfig( wv, pv );
            }
            if ( ui.workVertex2En->isChecked() )
            {
                qreal wv = ui.workVertex2->value();
                qreal pv = ( pull ) ? (probeAt+wv-workAt) : probeAt;
                pushSweepConfig( wv, pv );
            }
        }
        qreal wv = ui.workSweepTo->value();
        qreal pv = ( pull ) ? (probeAt+wv-workAt) : probeAt;
        pushSweepConfig( wv, pv );

        bool res = runSweep();
        if ( !res )
            return;

        // Flip transition boundaries.
        ui.workVolt->setValue( wv );
        ui.workSweepTo->setValue( workAt );
        setWorkV  = wv;
        if ( pull )
        {
            ui.probeVolt->setValue( pv );
            ui.probeSweepTo->setValue( probeAt );
            setProbeV = pv;
        }

        QString ss = "background-color: rgb(100, 135, 100);\nborder-color: rgb(100, 135, 100);";
        ui.workVolt->setStyleSheet( ss );
        ui.probeVolt->setStyleSheet( ss );
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
        io->setProbeMv( outA( val ) );

        setProbeV = val;

        QString ss = "background-color: rgb(100, 135, 100);\nborder-color: rgb(100, 135, 100);";
        ui.probeVolt->setStyleSheet( ss );
    }
}

void MainWnd::slotSweepProbe()
{
    if ( io->isOpen() )
    {
        clearSweepConfig();

        bool pull = ui.pullWork->isChecked();

        qreal workAt  = ui.workVolt->value();
        qreal probeAt = ui.probeVolt->value();
        int   qty     = ui.cyclesCnt->value();
        if ( ( !ui.workVertex1En->isChecked() ) || ( !ui.workVertex2En->isChecked() ) )
            qty = 1;
        for ( int i=0; i<qty; i++ )
        {
            if ( ui.probeVertex1En->isChecked() )
            {
                qreal pv = ui.probeVertex1->value();
                qreal wv = ( pull ) ? (workAt+pv-probeAt) : workAt;
                pushSweepConfig( wv, pv );
            }
            if ( ui.probeVertex2En->isChecked() )
            {
                qreal pv = ui.probeVertex2->value();
                qreal wv = ( pull ) ? (workAt+pv-probeAt) : workAt;
                pushSweepConfig( wv, pv );
            }
        }
        qreal pv = ui.probeSweepTo->value();
        qreal wv = ( pull ) ? (workAt+pv-probeAt) : workAt;
        pushSweepConfig( wv, pv );

        bool res = runSweep();
        if ( !res )
            return;

        // Flip transition boundaries.
        if ( pull )
        {
            ui.workVolt->setValue( wv );
            ui.workSweepTo->setValue( workAt );
            setWorkV = wv;
        }
        ui.probeVolt->setValue( pv );
        ui.probeSweepTo->setValue( probeAt );
        setProbeV = pv;

        QString ss = "background-color: rgb(100, 135, 100);\nborder-color: rgb(100, 135, 100);";
        ui.workVolt->setStyleSheet( ss );
        ui.probeVolt->setStyleSheet( ss );
    }
}

void MainWnd::slotPolarization()
{
    PolarizationDlg pDlg( this );
    pDlg.setPotential( polarizationPotential );
    pDlg.setDuration( polarizationDuration );
    pDlg.setMeasureDuraton( measureDuration );
    pDlg.setApplyTo( polarizationIndex );
    pDlg.setPull( polarizationPull );
    if ( pDlg.exec() != QDialog::Accepted )
        return;

    tempTimer->stop();

    polarizationPotential = pDlg.potential();
    polarizationDuration  = pDlg.duration();
    measureDuration       = pDlg.measureDuration();
    polarizationIndex     = pDlg.applyTo();
    polarizationPull      = pDlg.pull();

    qreal workAt  = ui.workVolt->value();
    qreal probeAt = ui.probeVolt->value();

    // Block GUI.
    qreal workTo; 
    qreal probeTo;
    if ( polarizationIndex == 0 )
    {
        workTo  = polarizationPotential;
        probeTo = (polarizationPull) ? (probeAt + polarizationPotential - workAt) : probeAt;
    }
    else
    {
        probeTo = polarizationPotential;
        workTo  = (polarizationPull) ? (workAt + polarizationPotential - probeAt) : workAt;
    }

    bool res = io->sweepPush( 0, 0, outA( workTo ), outB( probeTo ) );
    if ( !res )
    {
        QString stri = QString( "Failed to set potential transition!" );
        QMessageBox::critical( this, "Error", stri );
        return;
    }

    res = io->sweepPush( 0, polarizationDuration, outA( workTo ), outB( probeTo ) );
    if ( !res )
    {
        QString stri = QString( "Failed to set potential transition!" );
        QMessageBox::critical( this, "Error", stri );
        return;
    }
    res = io->sweepPush( 0, 0, outA( workAt ), outB( probeAt ) );
    if ( !res )
    {
        QString stri = QString( "Failed to set potential transition!" );
        QMessageBox::critical( this, "Error", stri );
        return;
    }

    sweepWnd = new SweepWnd( this );
    sweepWnd->setTitles( "Electrode 1 I(t)", "Electrode 2 I(t)" );
    sweepWnd->show();
    mutex.lock();
        doMeasureOsc = true;
    mutex.unlock();
    res = io->setSweepEn( true );
    ui.dockWidget_2->setEnabled( false );

    QTime time;
    time.start();
    // To prevent overwriting potential by timer.
    setWorkV = polarizationPotential;
    bool sweepEn;
    do {
        polarizationDelay();
        res = io->sweepEn( sweepEn );
        if ( !res )
            break;
        qApp->processEvents();
    } while ( sweepEn );

    do {
        qApp->processEvents();
        mutex.lock();
            bool measureEn = doMeasureOsc;
        mutex.unlock();
        if ( !measureEn )
            break;
    } while ( ( time.elapsed() / 1000 ) < measureDuration );

    // Sto measures.
    mutex.lock();
        doMeasureOsc = false;
    mutex.unlock();


    // Restore overwriting potential by timer.
    setWorkV  = ui.workVolt->value();
    setProbeV = ui.probeVolt->value();

    ui.dockWidget_2->setEnabled( true );

    tempTimer->start();
}

void MainWnd::slotInstantValues( qreal wv, qreal pv, qreal wi, qreal pi )
{
    // Debugging;
    mutex.lock();
        int cnt = totalPts;
    mutex.unlock();

    //setWindowTitle( QString( "%1" ).arg( cnt ) );
    QString stri= QString( "T: %1[C], workV = %2[mV], workI = %3[uA], probeV = %4[mV], probeI = %5[uA], triggers: %6" ).arg( temperature, 5, 'f', 2, QChar( '0' ) ) 
                                                                                         .arg( wv, 8, 'f', 1, QChar( '0' ) ) 
                                                                                         .arg( wi * 1000000.0, 8, 'f', 7, QChar( '0' ) )
                                                                                         .arg( pv, 8, 'f', 1, QChar( '0' ) )
                                                                                         .arg( pi * 1000000.0, 8, 'f', 7, QChar( '0' ) )
                                                                                         .arg( cnt, 6, 10, QChar( ' ' ) );

    setTitle( stri );

}

void MainWnd::slotReplot()
{
    // Add data to oscilloscope windows.
    QQueue<qreal> workV, workI, probeV, probeI;
    mutex.lock();
        bool measureOsc = doMeasureOsc;
        if ( sweepWnd && measureOsc )
        {
            workV = p_workV;
            workI = p_workI;
            probeV = p_probeV;
            probeI = p_probeI;
        }
        oscWork->addData( p_workI );
        oscProbe->addData( p_probeI );
        p_workV.clear();
        p_probeV.clear();
    mutex.unlock();
    // Replot oscilloscope windows.
    oscWork->slotReplot();
    oscProbe->slotReplot();

    // Replot sweepWnd if there is something to replot.
    if ( sweepWnd && measureOsc )
        sweepWnd->addData( workV, workI, probeV, probeI, oscWork->scale() );
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
                io->setWorkMv( outA( setWorkV ) );
                io->setProbeMv( outB( setProbeV ) );
            }
            temperature = t;

            qreal workV, probeV, workI, probeI;
            // Probably instant data shouldn't be read here.
            res = io->instantData( workV, probeV, workI, probeI );
            if ( res )
                emit sigInstantValues( workV, probeV, workI, probeI );
        }


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
    clearSweepConfig();
    {
        QMutexLocker lock( &mutex );
            doMeasureSweep = false;
            doMeasureOsc   = false;
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
    mutex.lock();
        totalPts = 0;
    mutex.unlock();
    bool en = ui.actionExternal_trigger->isChecked();
    bool dacMode = ui.sweepDacMode->isChecked();
    bool res;
    {
        QMutexLocker lock( &mutex );
            res = io->isOpen();
            if ( res )
            {
                res = io->setSweepDacMode( dacMode );
                if ( res )
                    res = io->setTriggerEn( en );
            }
    }
    if ( !res )
    {
        QMessageBox::critical( this, "Error", "Failed to setup external triggering!" );
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

void MainWnd::slotVertexEnChanged()
{
    ui.workVertex1->setEnabled( ui.workVertex1En->isChecked() );
    ui.workVertex2->setEnabled( ui.workVertex2En->isChecked() );
    ui.probeVertex1->setEnabled( ui.probeVertex1En->isChecked() );
    ui.probeVertex2->setEnabled( ui.probeVertex2En->isChecked() );
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

qreal MainWnd::outA( qreal value ) const
{
    return invOutA ? (-value) : value;
}

qreal MainWnd::outB( qreal value ) const
{
    return invOutB ? (-value) : value;
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
    mutex.lock();
        sweepRemote    = this->trigEn;
        doMeasureSweep = this->trigEn;
    mutex.unlock();
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






