
#ifndef __MEANDR_WND_H_
#define __MEANDR_WND_H_

#include <QtGui>
#include "ui_meandr_wnd.h"
#include "voltamp_io.h"

class MainWnd;

class MeandrWnd: public QWidget
{
    Q_OBJECT
public:
    MeandrWnd( QWidget * parent = 0 );
    ~MeandrWnd();

    void setIo( VoltampIo * io, MainWnd * mainWnd );

private slots:
    void slotApply();
private:
    Ui_MeandrWnd ui;
    VoltampIo * io;
    MainWnd * mainWnd;
};

#endif


