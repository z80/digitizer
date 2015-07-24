
#include "main_wnd.h"
#include <QFileDialog>
#include "lua.hpp"


const QString MainWnd::SETTINGS_INI = "./settings.ini";

MainWnd::MainWnd( QWidget * parent )
    : QMainWindow( parent )
{
    ui.setupUi( this );
    //ui.osc->show();

    statusLabel = new QLabel( ui.statusbar );
    ui.statusbar->addWidget( statusLabel );

    io = new VoltampIo();

    loadSettings();
    refreshDevicesList();

    QVBoxLayout * bl = new QVBoxLayout( ui.osc );
    osc = new OscilloscopeWnd( this );
    bl->addWidget( osc );
    osc->setIo( io, this );

    dcWnd = new DcVoltageWnd( 0 );
    dcWnd->setIo( io, this );

    spWnd = new SinglePulseWnd( 0 );
    spWnd->setIo( io, this );

    mrWnd = new MeandrWnd( 0 );
    mrWnd->setIo( io, this );

    swWnd = new SweepWnd( 0 );
    swWnd->setIo( io, this );

    calibrationWnd = new CalibrationWnd( 0 );
    calibrationWnd->setIo( io, this, osc );

    connect( ui.action_Quit,  SIGNAL(triggered()), this, SLOT(slotQuit()) );
    //connect( ui.actionOpen,   SIGNAL(triggered()), this, SLOT(slotReopen()) );
    connect( ui.action_About, SIGNAL(triggered()), this, SLOT(slotAbout()) );

    connect( ui.actionDC_Volt,      SIGNAL(triggered()), this, SLOT(slotDc()) );
    connect( ui.actionSingle_pulse, SIGNAL(triggered()), this, SLOT(slotSinglePulse()) );
    connect( ui.actionMeandr,       SIGNAL(triggered()), this, SLOT(slotMeandr()) );
    connect( ui.actionSweep,        SIGNAL(triggered()), this, SLOT(slotSweep()) );

    connect( ui.actionShort_relay, SIGNAL(triggered()), this, SLOT(slotShortRelay()) );
    connect( ui.actionOut_relay,   SIGNAL(triggered()), this, SLOT(slotOutRelay()) );

    connect( ui.actionCalibration,  SIGNAL(triggered()), this, SLOT(slotCalibration()) );

    connect( ui.actionExecLua, SIGNAL(triggered()), this, SLOT(slotLuaOpen()) );
    connect( ui.actionStopLua, SIGNAL(triggered()), this, SLOT(slotLuaStop()) );

    // Lua State creation.
    state = new QtLua::State();

    // Console connection.
    connect( ui.console, SIGNAL(line_validate(const QString&)),
             state,      SLOT(exec(const QString&)));

    connect( ui.console, SIGNAL(get_completion_list(const QString &, QStringList &, int &)),
             state,      SLOT(fill_completion_list(const QString &, QStringList &, int &)));

    connect( state, SIGNAL(output(const QString&)),
             ui.console, SLOT(print(const QString&)));
             
    m_luaDoStop = false;
    lua_State * L = state->get_lua_state();
    lua_pushliteral( L, "MainWnd" );
    MainWnd * * p = reinterpret_cast< MainWnd * * >( lua_newuserdata( L, sizeof( MainWnd * ) ) );
    *p = this;
    lua_settable( L, LUA_REGISTRYINDEX );

    // This should be in separate slot in single shot timer.
    //state->lua_do( MainWnd::lua_init );
    QTimer::singleShot( 100, this, SLOT(slotLuaInit()) );
}

MainWnd::~MainWnd()
{
    //state->deleteLater();
    ui.osc->deleteLater();
    delete io;
}

qreal MainWnd::vAux( quint16 adc )
{
    return static_cast<qreal>( adc ) * aAdcAux + bAdcAux;
}

qreal MainWnd::vRef( quint16 adc )
{
    return static_cast<qreal>( adc ) * aAdcRef + bAdcRef;
}

qreal MainWnd::iAux( quint16 adc )
{
    return static_cast<qreal>( adc ) * aAdcI + bAdcI;
}

void  MainWnd::dac( qreal v, int & dacLow, int & dacHigh )
{
    qreal fLow = 2047.0;
    qreal fHigh = ceil( (v - bDac - fLow * aDacLow ) / aDacHigh - 0.5 );
    fLow = ceil( (v - bDac - fHigh*aDacHigh)/aDacLow - 0.5 );
    dacLow  = static_cast<int>( fLow );
    dacHigh = static_cast<int>( fHigh );
}

int MainWnd::timeToTicks( qreal time )
{
    int res = static_cast<int>( time );
    return res;
}

void MainWnd::setStatus( qreal eaux, qreal eref, qreal iaux )
{
    QString stri = QString( "Eref %1mV,     I %2mA,     Eaux %3mV" ).arg( eref, 6, 'g', 1, QChar( '_' ) )
                                                                    .arg( iaux, 6, 'g', 3, QChar( '_' ) )
                                                                    .arg( eaux, 6, 'g', 1, QChar( '_' ) );
    //statusLabel->setText( stri );
    this->setWindowTitle( stri );
}

void MainWnd::setRelays( bool shortRelay, bool outRelay )
{
    ui.actionShort_relay->setChecked( shortRelay );
    ui.actionOut_relay->setChecked( outRelay );
    slotShortRelay();
    slotOutRelay();
}

void MainWnd::setCalibrationDac( qreal aLow, qreal aHigh, qreal b )
{
    aDacLow  = aLow;
    aDacHigh = aHigh;
    bDac     = b;
}

void MainWnd::setCalibrationAdcVolt( qreal aAux, qreal bAux, qreal aRef, qreal bRef )
{
    aAdcAux = aAux;
    bAdcAux = bAux;
    aAdcRef = aRef;
    bAdcRef = bRef;

    saveSettings();
}

void MainWnd::setCalibrationAdcCurr( qreal aI, qreal bI )
{
    aAdcI = aI;
    bAdcI = bI;

    saveSettings();
}

void MainWnd::loadSettings()
{
    QSettings s( SETTINGS_INI, QSettings::IniFormat );

    devName  = s.value( "devName",  "0" ).toInt();

    aDacLow  = s.value( "aDacLow",  0.0001 ).toDouble();
    aDacHigh = s.value( "aDacHigh", 1.0 ).toDouble();
    bDac     = s.value( "bDac",     -2047.0 ).toDouble();
    aAdcAux  = s.value( "aAdcAux",  1.0 ).toDouble();
    bAdcAux  = s.value( "bAdcAux",  -2047.0 ).toDouble();
    aAdcRef  = s.value( "aAdcRef",  1.0 ).toDouble();
    bAdcRef  = s.value( "bAdcRef",  -2047.0 ).toDouble();
    aAdcI    = s.value( "aAdcI",    1.0 ).toDouble();
    bAdcI    = s.value( "bAdcI",    -2047.0 ).toDouble();

    ui.console->load_history( s );

    this->restoreState( s.value( "state", QByteArray() ).toByteArray() );
}

void MainWnd::saveSettings()
{
    QSettings s( SETTINGS_INI, QSettings::IniFormat );

    s.setValue( "devName", devName );

    s.setValue( "aDacLow",  aDacLow );
    s.setValue( "aDacHigh", aDacHigh );
    s.setValue( "bDac",     bDac );
    s.setValue( "aAdcAux",  aAdcAux );
    s.setValue( "bAdcAux",  bAdcAux );
    s.setValue( "aAdcRef",  aAdcRef );
    s.setValue( "bAdcRef",  bAdcRef );
    s.setValue( "aAdcI",    aAdcI );
    s.setValue( "bAdcI",    bAdcI );

    ui.console->save_history( s );

    s.setValue( "state", this->saveState() );
}

int MainWnd::deviceName() const
{
    return devName;
}

void MainWnd::slotQuit()
{
    saveSettings();
    osc->stop();
    slotLuaStop();
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
    QMessageBox::about( this, "About", 
                        "Arbeit march frei!"
                       );
}

void MainWnd::slotDc()
{
    dcWnd->show();
}
void MainWnd::slotSinglePulse()
{
    spWnd->show();
}

void MainWnd::slotMeandr()
{
    mrWnd->show();
}

void MainWnd::slotSweep()
{
    swWnd->show();
}

void MainWnd::slotShortRelay()
{
    bool en = ui.actionShort_relay->isChecked();
    io->set_sc_relay( en );
}

void MainWnd::slotOutRelay()
{
    bool en = ui.actionOut_relay->isChecked();
    io->set_out_relay( en );
}

void MainWnd::slotCalibration()
{
    calibrationWnd->show();
}

void MainWnd::slotLuaOpen()
{
    QString fileName = QFileDialog::getOpenFileName( this, tr("Open File"),
                                                    "./",
                                                    tr("Lua (*.lua)"));
    QFile f( fileName );
    if ( f.open( QIODevice::ReadOnly ) )
    {
        try {
            state->exec_chunk( f );
            f.close();
        } catch ( QtLua::String & e )
        {
            ui.console->print( e );
        }
    }
}

void MainWnd::slotLuaStop()
{
    setLuaDoStop( true );
    /*
    try {
        state->exec_statements( "error( \'Execution was stopped\', 1 )" );
    } catch ( QtLua::String & e )
    {
        ui.console->print( e );
    }
    */
}

void MainWnd::slotLuaInit()
{
    state->lua_do( MainWnd::lua_init );
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

void MainWnd::refreshDevicesList()
{
    foreach( QAction * a, devicesList )
        a->deleteLater();
    devicesList.clear();
    QStringList l = io->enumDevices();
    foreach( QString stri, l )
    {
        QAction * a = new QAction( stri, ui.menuDevice );
        ui.menuDevice->addAction( a );
        a->setCheckable( true );
        connect( a, SIGNAL(triggered()), this, SLOT(slotDevice()) );
        devicesList.append( a );
    }
}




static const struct luaL_reg FUNCTIONS[] = {
    { "msleep",                   MainWnd::lua_msleep },
    { "setDc",                    MainWnd::lua_setDc },
    { "setMeandr",                MainWnd::lua_setMeandr },
    { "setSweep",                 MainWnd::lua_setSweep },
    { "setScRelay",               MainWnd::lua_setScRelay },
    { "setOutRelay",              MainWnd::lua_setOutRelay },
    { "dataCallbackRegister",     MainWnd::lua_dataCallbackRegister },
    { "dataCallbackRegisterFull", MainWnd::lua_dataCallbackRegisterFull },

    { NULL,            NULL },
};

static MainWnd * mainWnd( lua_State * L )
{
    lua_pushliteral( L, "MainWnd" );
    lua_gettable( L, LUA_REGISTRYINDEX );
    MainWnd * mw = *reinterpret_cast<MainWnd * *>( lua_touserdata( L, -1 ) );
    lua_pop( L, 1 );
    return mw;
}

void MainWnd::lua_init( lua_State * L )
{
    int top = lua_gettop( L );

    luaL_register( L, "voltamp", FUNCTIONS );

    // Install hook function to prevent software hanging up.
    lua_sethook( L, MainWnd::lua_hook, LUA_MASKLINE, 0 );

    MainWnd * mw = mainWnd( L );

    // Open all Lua standard libraries.
    mw->state->openlib( QtLua::AllLibs );

    // Execute initialization file.
    QFile f( "./init.lua" );
    if ( f.open( QIODevice::ReadOnly) )
    {
        try {
            mw->state->exec_chunk( f );
            f.close();
        } catch ( QtLua::String & e )
        {
            mw->ui.console->print( e );
        }
    }
}

void MainWnd::lua_setHook( bool set )
{
    lua_State * L = state->get_lua_state();
    if ( set )
        lua_sethook( L, MainWnd::lua_hook, LUA_MASKLINE, 0 );
    else
        lua_sethook( L, 0, LUA_MASKLINE, 0 );
}

void MainWnd::lua_invokeCallback( qreal eref, qreal iaux, qreal eaux )
{
    lua_State * L = state->get_lua_state();
    int top = lua_gettop( L );

    lua_pushliteral( L, "dc" );
    //int top2 = lua_gettop( L );
    lua_gettable( L, LUA_REGISTRYINDEX );
    //int top3 = lua_gettop( L );
    int tp = lua_type( L, -1 );
    if ( tp == LUA_TFUNCTION )
    {
        lua_pushnumber( L, eref );
        lua_pushnumber( L, iaux );
        lua_pushnumber( L, eaux );
        int res = lua_pcall( L, 3, 0, 0 );
        if ( res != 0 )
        {
            QString stri = lua_tostring( L, -1 );
            ui.console->print( stri );
        }
    }
    else
        lua_pop( L, 1 );
    lua_settop( L, top );
}

static void pushTableData( lua_State * L, const QVector<qreal> & data )
{
    int sz = data.size();
    lua_createtable( L, sz, 0 );
    for ( int i=0; i<sz; i++ )
    {
        lua_pushinteger( L, i+1 );
        lua_pushnumber( L, static_cast<lua_Number>( data.at( i ) ) );
        lua_settable( L, -3 );
    }
}

void MainWnd::lua_invokeCallbackFull( const QVector<qreal> & eref, const QVector<qreal> & iref, const QVector<qreal> & eaux )
{
    lua_State * L = state->get_lua_state();
    int top = lua_gettop( L );

    lua_pushliteral( L, "dcfull" );
    //int top2 = lua_gettop( L );
    lua_gettable( L, LUA_REGISTRYINDEX );
    //int top3 = lua_gettop( L );
    int tp = lua_type( L, -1 );
    int top2 = lua_gettop( L );
    if ( tp == LUA_TFUNCTION )
    {
        pushTableData( L, eref );
        pushTableData( L, iref );
        pushTableData( L, eaux );
        int top3 = lua_gettop( L );
        int res = lua_pcall( L, 3, 0, 0 );
        if ( res != 0 )
        {
            QString stri = lua_tostring( L, -1 );
            ui.console->print( stri );
        }
    }
    lua_settop( L, top );
}

void MainWnd::lua_hook( lua_State * L, lua_Debug * Ld )
{
    MainWnd * mw = mainWnd( L );
    qApp->processEvents();
    if ( mw->luaDoStop() )
    {
        mw->setLuaDoStop( false );
        lua_error( L );
    }
}

int MainWnd::lua_msleep( lua_State * L )
{
    int ms = static_cast<int>( lua_tonumber( L, 1 ) );
    QTime t0 = QTime::currentTime();
    t0.start();
    while ( t0.elapsed() < ms )
        qApp->processEvents();
    return 0;
}

int MainWnd::lua_setDc( lua_State * L )
{
    MainWnd * mw = mainWnd( L );
    qreal volt = static_cast<qreal>( lua_tonumber( L, 1 ) );
    int low, high;
    mw->dac( volt, low, high );
    bool res = mw->io->set_dac_raw( low, high );
    lua_pushboolean( L, res ? 1 : 0 );
    return 1;
}

int MainWnd::lua_setMeandr( lua_State * L )
{
    MainWnd * mw = mainWnd( L );
    qreal volt1 = static_cast<qreal>( lua_tonumber( L, 1 ) );
    qreal timeMs1 = static_cast<qreal>( lua_tonumber( L, 2 ) );
    qreal volt2 = static_cast<qreal>( lua_tonumber( L, 3 ) );
    qreal timeMs2 = static_cast<qreal>( lua_tonumber( L, 4 ) );
    int low1, high1;
    mw->dac( volt1, low1, high1 );
    int low2, high2;
    mw->dac( volt2, low2, high2 );

    bool res = mw->io->set_meandr_raw( low1, high1, timeMs1, low2, high2, timeMs2 );
    lua_pushboolean( L, res ? 1 : 0 );
    return 1;
}

int MainWnd::lua_setSweep( lua_State * L )
{
    MainWnd * mw = mainWnd( L );
    qreal volt1 = static_cast<qreal>( lua_tonumber( L, 1 ) );
    qreal volt2 = static_cast<qreal>( lua_tonumber( L, 2 ) );
    qreal timeMs = static_cast<qreal>( lua_tonumber( L, 3 ) );
    int low1, high1;
    mw->dac( volt1, low1, high1 );
    int low2, high2;
    mw->dac( volt2, low2, high2 );

    bool res = mw->io->set_sweep_raw( low1, high1, low2, high2, timeMs );
    lua_pushboolean( L, res ? 1 : 0 );
    return 1;
}

int MainWnd::lua_setScRelay( lua_State * L )
{
    MainWnd * mw = mainWnd( L );
    bool en = ( lua_toboolean( L, 1 ) > 0 );

    bool res = mw->io->set_sc_relay( en );
    lua_pushboolean( L, res ? 1 : 0 );
    return 1;
}

int MainWnd::lua_setOutRelay( lua_State * L )
{
    MainWnd * mw = mainWnd( L );
    bool en = ( lua_toboolean( L, 1 ) > 0 );

    bool res = mw->io->set_out_relay( en );
    lua_pushboolean( L, res ? 1 : 0 );
    return 1;
}

int MainWnd::lua_dataCallbackRegister( lua_State * L )
{
    lua_pushliteral( L, "dc" );
    lua_pushvalue( L, -2 );
    lua_settable( L, LUA_REGISTRYINDEX );
    return 0;
}

int MainWnd::lua_dataCallbackRegisterFull( lua_State * L )
{
    lua_pushliteral( L, "dcfull" );
    lua_pushvalue( L, -2 );
    lua_settable( L, LUA_REGISTRYINDEX );
    return 0;
}

bool MainWnd::luaDoStop()
{
    return m_luaDoStop;
}

void MainWnd::setLuaDoStop( bool stop )
{
    m_luaDoStop = stop;
}









