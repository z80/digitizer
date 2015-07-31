
#ifndef __MAIN_WND_H_
#define __MAIN_WND_H_

#include <QtGui>
#include <QMessageBox>
#include <QtConcurrent/QtConcurrentRun>

#include "ui_main_wnd.h"
#include "bipot.h"
#include "oscilloscope_wnd.h"
#include "calibration_wnd.h"

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
protected:
    void closeEvent( QCloseEvent * e );
private:
    void setTitle( const QString & stri );
    void refreshDevicesList();

    void measure();
    void reopen();

    int devName;

    Ui_MainWnd ui;
    QFuture<void> future;
    QMutex        mutex;
    bool          terminate;
    Bipot         * io;
    OscilloscopeWnd * oscWork;
    OscilloscopeWnd * oscProbe;
    CalibrationWnd  * calibrationWnd;

    QList<QAction *> devicesList;

    // Data to be read from USB directly in a separate thread.
    QVector<qreal> t_workV, t_workI, t_probeV, t_probeI;
    // Data to be painted.
    QQueue<qreal> p_workV, p_workI, p_probeV, p_probeI;

    static const QString SETTINGS_INI;
};

#endif












