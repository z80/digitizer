
#include "single_pulse_wnd.h"
#include "main_wnd.h"

SinglePulseWnd::SinglePulseWnd( QWidget * parent )
    : QWidget( parent )
{
    ui.setupUi( this );
    io = 0;
    connect( ui.apply, SIGNAL(clicked()), this, SLOT(slotApply()) );
}

SinglePulseWnd::~SinglePulseWnd()
{

}

void SinglePulseWnd::setIo( VoltampIo * io, MainWnd * mainWnd )
{
    this->io = io;
    this->mainWnd = mainWnd;
}

void SinglePulseWnd::slotApply()
{
    qreal volt     = ui.volt->value();
    qreal duration = ui.duration->value();

    int dacLow;
    int dacHigh;
    mainWnd->dac( volt, dacLow, dacHigh );
    int time = mainWnd->timeToTicks( duration );

    bool res;
    res = io->set_one_pulse_raw( dacLow, dacHigh, time );
}



