
#ifndef __MAIN_WND_H_
#define __MAIN_WND_H_

#include <QtGui>
#include <QMessageBox>
#include "ui_main_wnd.h"
#include "voltamp_io.h"
#include "oscilloscope_wnd.h"
#include "dc_voltage_wnd.h"
#include "single_pulse_wnd.h"
#include "meandr_wnd.h"
#include "sweep_wnd.h"
#include "calibration_wnd.h"

#include <QtLua/State>
#include <QtLua/Console>
struct lua_Debug;

class MainWnd: public QMainWindow
{
    Q_OBJECT
public:
    MainWnd( QWidget * parent = 0 );
    ~MainWnd();

    qreal vAux( quint16 adc );
    qreal vRef( quint16 adc );
    qreal iAux( quint16 adc );
    void  dac( qreal v, int & dacLow, int & dacHigh );
    int   timeToTicks( qreal time );

    void setStatus( qreal eaux, qreal eref, qreal iaux );

    void setRelays( bool shortRelay, bool outRelay );

    void setCalibrationDac( qreal aLow, qreal aHigh, qreal b );
    void setCalibrationAdcVolt( qreal aAux, qreal bAux, qreal aRef, qreal bRef );
    void setCalibrationAdcCurr( qreal aI, qreal bI );

    void loadSettings();
    void saveSettings();

    int deviceName() const;
public slots:
    void slotQuit();
    void slotReopen();
    void slotAbout();

    void slotDc();
    void slotSinglePulse();
    void slotMeandr();
    void slotSweep();

    void slotShortRelay();
    void slotOutRelay();

    void slotCalibration();

    void slotDevice();

    void slotLuaOpen();
    void slotLuaStop();

    void slotLuaInit();
protected:
    void closeEvent( QCloseEvent * e );
private:
    void setTitle( const QString & stri );
    void refreshDevicesList();

    // Lua state initialization and handling.
    static void lua_init( lua_State * L );

    int devName;

    Ui_MainWnd ui;
    VoltampIo  * io;
    OscilloscopeWnd * osc;
    DcVoltageWnd * dcWnd;
    SinglePulseWnd * spWnd;
    MeandrWnd * mrWnd;
    SweepWnd * swWnd;
    CalibrationWnd * calibrationWnd;

    QLabel * statusLabel;

    qreal aDacLow, aDacHigh, bDac;
    qreal aAdcAux, bAdcAux, aAdcRef, bAdcRef;
    qreal aAdcI, bAdcI;

    QList<QAction *> devicesList;

    QtLua::State * state;
    bool m_luaDoStop;

    static const QString SETTINGS_INI;

public:
    void lua_setHook( bool set );
    static void lua_hook( lua_State * L, lua_Debug * Ld );

    static int lua_msleep( lua_State * L );
    static int lua_setDc( lua_State * L );
    static int lua_setMeandr( lua_State * L );
    static int lua_setSweep( lua_State * L );
    static int lua_setScRelay( lua_State * L );
    static int lua_setOutRelay( lua_State * L );
    static int lua_dataCallbackRegister( lua_State * L );
    static int lua_dataCallbackRegisterFull( lua_State * L );

    void lua_invokeCallback( qreal eaux, qreal eref, qreal iaux );
    void lua_invokeCallbackFull( const QVector<qreal> & eref, const QVector<qreal> & iref, const QVector<qreal> & eaux );

    bool luaDoStop();
    void setLuaDoStop( bool stop );
};

#endif




