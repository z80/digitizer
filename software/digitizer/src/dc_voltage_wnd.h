
#ifndef __DC_VOLTAGE_WND_H_
#define __DC_VOLTAGE_WND_H_

#include <QtGui>
#include "ui_dc_voltage_wnd.h"
#include "voltamp_io.h"

class MainWnd;

class DcVoltageWnd: public QWidget
{
    Q_OBJECT
public:
    DcVoltageWnd( QWidget * parent = 0 );
    ~DcVoltageWnd();

    void setIo( VoltampIo * io, MainWnd * mainWnd );

private slots:
    void slotChange();
    void slotApply();

private:
    Ui_DcVoltageWnd ui;
    VoltampIo * io;
    MainWnd * mainWnd;
};



#endif



