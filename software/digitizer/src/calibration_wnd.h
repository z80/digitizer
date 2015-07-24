
#ifndef __CALIBRATION_WND_H_
#define __CALIBRATION_WND_H_

#include <QtGui>
#include "ui_calibration_wnd.h"
#include "voltamp_io.h"

class MainWnd;
class OscilloscopeWnd;

class CalibrationWnd: public QWidget
{
    Q_OBJECT
public:
    CalibrationWnd( QWidget * parent = 0 );
    ~CalibrationWnd();

    void setIo( VoltampIo * io, MainWnd * mainWnd, OscilloscopeWnd * osc );

protected:
    void showEvent( QShowEvent & e );
    void closeEvent( QCloseEvent & e );

private slots:
    void slotEnable();
    void slotAddVolt();
    void slotAddCurr();
    void slotClearFiles();
private:
    void setRandomVolt();
    void calcDac2Volt();
    void calcAdcAux2Volt();
    void calcAdcRef2Volt();
    void calcAdcI2Curr();

    void openVoltCalibrationFile();
    void openCurrCalibrationFile();
    void closeCalibrationFiles();

    Ui_CalibrationWnd ui;
    MainWnd * mainWnd;
    OscilloscopeWnd * osc;
    VoltampIo * io;

    int dacLow,
        dacHigh;

    QVector<int> dacLowV,
                 dacHighV,
                 adcAux,
                 adcRef,
                 adcI;

    QVector<qreal> volt,
                   curr;

    // Calibration coefs.
    qreal aDacLow, aDacHigh, bDac;
    qreal aAdcAux, bAdcAux;
    qreal aAdcRef, bAdcRef;
    qreal aAdcI,   bAdcI;

    QFile       voltFile,   currFile;
    QTextStream voltStream, currStream;

    static const QString VOLT_FILE, CURR_FILE;
};


#endif





