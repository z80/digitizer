
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

class QwtTextLabel;

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
    void sigReplot();

    void sigSweepReplot();
    void sigSweepFinished();
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
    void measureSweep();
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
};

#endif












