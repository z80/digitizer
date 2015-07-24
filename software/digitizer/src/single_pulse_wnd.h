
#ifndef __SINGLE_PULSE_WND_H_
#define __SINGLE_PULSE_WND_H_

#include <QtGui>
#include "ui_single_pulse_wnd.h"
#include "voltamp_io.h"

class MainWnd;

class SinglePulseWnd: public QWidget
{
    Q_OBJECT
public:
    SinglePulseWnd( QWidget * parent = 0 );
    ~SinglePulseWnd();

    void setIo( VoltampIo * io, MainWnd * mainWnd );

private slots:
    void slotApply();
private:
    Ui_SinglePulseWnd ui;
    VoltampIo * io;
    MainWnd   * mainWnd;
};


#endif



