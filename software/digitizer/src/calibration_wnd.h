
#ifndef __CALIBRATION_WND_H_
#define __CALIBRATION_WND_H_

#include <QtGui>
#include "ui_calibration_wnd.h"
#include "bipot.h"

class MainWnd;
class OscilloscopeWnd;

class CalibrationWnd: public QWidget
{
    Q_OBJECT
public:
    CalibrationWnd( QWidget * parent = 0 );
    ~CalibrationWnd();

    void setIo( Bipot * io );

protected:
    void showEvent( QShowEvent & e );
    void closeEvent( QCloseEvent & e );

private slots:
    void slotEnable();
    void slotAddWorkVolt();
    void slotAddProbeVolt();
    void slotAddAdc();
    void slotClearFiles();
private:
    void setRandomVolt();

    void openCalibrationFiles();
    void saveCalibrationFiles();
    void clearCalibrationFiles();

    Ui_CalibrationWnd ui;
    MainWnd * mainWnd;
    Bipot * io;

    int dacA,
        dacB, 
        dacC, 
        dacD;
};


#endif





