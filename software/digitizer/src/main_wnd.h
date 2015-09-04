
#ifndef __MAIN_WND_H_
#define __MAIN_WND_H_

#include <QtGui>
#include <QMessageBox>
#include <QtConcurrent/QtConcurrentRun>

#include "ui_main_wnd.h"
#include "bipot.h"
#include "oscilloscope_wnd.h"
#include "calibration_wnd.h"
#include "sweep_wnd.h"
#include "exec.h"

#include "thread_ice.h"

class QwtTextLabel;
class HostTray;

class MainWnd: public QMainWindow
{
    Q_OBJECT
public:
    MainWnd( HostTray * parent = 0 );
    ~MainWnd();

    void loadSettings();
    void saveSettings();

    int deviceName() const;

signals:
    void sigInstantValues( qreal wv, qreal pv, qreal wi, qreal pi );
    void sigReplot();

    void sigSweepReplot();
    void sigSweepFinished();
public slots:
    void slotQuit();
    void slotAbout();
    void slotCalibration();
    void slotRemoteSetup();
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
    void slotReplot();

    void slotTemp();

    void slotOscPeriod();

    void slotSweepReplot();
    void slotSweepFinished();
    void slotStopSweep();

    void slotOpen();

    void slotExternalTrigger();
    void slotAfmOutput();

    void slotFirmwareUpgrade();
protected:
    void closeEvent( QCloseEvent * e );
private:
    void setTitle( const QString & stri );
    void refreshDevicesList();

    void measure();
    bool measureSweep();
    void reopen();

    int devName;

    Ui_MainWnd ui;
    QFuture<void> future;
    QMutex        mutex, 
                  mutexSw, 
                  mutexReopen; // Mutex for oscilloscope and for sweep.
    bool          terminate;
    bool          doMeasureSweep;
    Bipot         * io;
    OscilloscopeWnd * oscWork;
    OscilloscopeWnd * oscProbe;
    CalibrationWnd  * calibrationWnd;
    QTimer          * tempTimer;
    qreal           temperature;

    QPointer<SweepWnd> sweepWnd;
    Exec               sweepExec;

    QList<QAction *> devicesList;

    // Data to be read from USB directly in a separate thread.
    QVector<qreal> t_workV, t_workI, t_probeV, t_probeI;
    // Data to be painted.
    QQueue<qreal> p_workV, p_workI, p_probeV, p_probeI;

    // Sweep data to be read and to be painted.
    QVector<qreal> t_swWorkV, t_swWorkI, t_swProbeV, t_swProbeI;
    // Data to be painted.
    QQueue<qreal> p_swWorkV, p_swWorkI, p_swProbeV, p_swProbeI;

    QwtTextLabel * labelWork;
    QwtTextLabel * labelProbe;
    QwtText        textWork;
    QwtText        textProbe;

    qreal setWorkV, 
          setProbeV;

    QLabel * statusLabel;

    static const QString SETTINGS_INI;


    // ******************************************
    // External IO part.
    // ******************************************
public:
    bool iceInstantValues( qreal & workV, qreal & workI, qreal & probeV, qreal & probeI );
    bool iceSetTrigEn( bool en );
    bool iceValues( std::vector<qreal> & workV, std::vector<qreal> & workI, std::vector<qreal> & probeV, std::vector<qreal> & probeI );
signals:
    void sigInstantValues();
    void sigSetTrigEn();
    void sigValues();
private slots:
    void slotInstantValues();
    void slotSetTrigEn();
    void slotValues();
private:

    void listen();
    void setTrayToolTip( const QString & stri );

    HostTray * m_hostTray;

    ::IceUtil::Handle<ThreadIce> m_thread;

    QSemaphore sem;
    qreal workV, workI, probeV, probeI;
    std::vector<qreal> vWorkV, vWorkI, vProbeV, vProbeI;
    bool trigEn, result;
    bool sweepRemote;

    QString m_host;
    int     m_port;
    bool    m_doListen;
};

#endif












