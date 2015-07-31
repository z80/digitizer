
#ifndef __MAIN_WND_H_
#define __MAIN_WND_H_

#include <QtGui>
#include <QMessageBox>
#include "ui_main_wnd.h"
#include "bipot.h"
#include "oscilloscope_wnd.h"
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

    void loadSettings();
    void saveSettings();

    int deviceName() const;

signals:
    void sigInstantValues( qreal wv, qreal pv, qreal wi, qreal pi );
public slots:
    void slotQuit();
    void slotAbout();
    void slotCalibration();
    void slotDevice();
    void slotReopen();

    void slotGain();

    // Work electrode setup.
    void slotWorkVoltChange();
    void slotWorkVolt();
    void slotSweepWork();

    // Probe electrode setup.
    void slotProbeVoltChange();
    void slotProbeVolt();
    void slotSweepProbe();

    void slotInstantValues( qreal wv, qreal pv, qreal wi, qreal pi );
protected:
    void closeEvent( QCloseEvent * e );
private:
    void setTitle( const QString & stri );
    void refreshDevicesList();

    void measure();
    void reopen();

    int devName;

    Ui_MainWnd ui;
    QMutex mutex;
    Bipot  * io;
    OscilloscopeWnd * oscWork;
    OscilloscopeWnd * oscProbe;
    CalibrationWnd * calibrationWnd;

    QList<QAction *> devicesList;

    static const QString SETTINGS_INI;
};

#endif












