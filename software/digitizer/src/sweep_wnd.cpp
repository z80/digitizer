
#include "sweep_wnd.h"
#include "main_wnd.h"

SweepWnd::SweepWnd( QWidget * parent )
: QWidget( parent )
{
    ui.setupUi( this );
    connect( ui.apply, SIGNAL(clicked()), this, SLOT(slotApply()) );
}

SweepWnd::~SweepWnd()
{

}

void SweepWnd::setIo( VoltampIo * io, MainWnd * mainWnd )
{
    this->io      = io;
    this->mainWnd = mainWnd;
}

void SweepWnd::slotApply()
{

    int dacLow1,  dacHigh1;
    qreal volt     = ui.low->value();
    mainWnd->dac( volt, dacLow1, dacHigh1 );

    int dacLow2, dacHigh2;
    volt     = ui.high->value();
    mainWnd->dac( volt, dacLow2, dacHigh2 );

    qreal duration = ui.period->value();
    int time = mainWnd->timeToTicks( duration );

    bool res;
    res = io->set_sweep_raw( dacLow1, dacHigh1, dacLow2, dacHigh2, time );
}


