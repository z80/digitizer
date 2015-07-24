
#ifndef __SWEEP_WND_H_
#define __SWEEP_WND_H_

#include <QtGui>
#include "ui_sweep_wnd.h"
#include "voltamp_io.h"

class MainWnd;

class SweepWnd: public QWidget
{
    Q_OBJECT
public:
    SweepWnd( QWidget * parent = 0 );
    ~SweepWnd();

    void setIo( VoltampIo * io, MainWnd * mainWnd );

private slots:
    void slotApply();
private:
    Ui_SweepWnd ui;
    VoltampIo * io;
    MainWnd * mainWnd;
};


#endif

