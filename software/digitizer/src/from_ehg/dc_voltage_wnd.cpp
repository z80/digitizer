
#include "dc_voltage_wnd.h"
#include "main_wnd.h"

DcVoltageWnd::DcVoltageWnd( QWidget * parent )
    : QWidget( parent )
{
    ui.setupUi( this );
    io = 0;

    connect( ui.value, SIGNAL(valueChanged(double)), this, SLOT(slotChange()) );
    connect( ui.value, SIGNAL(editingFinished()),    this, SLOT(slotApply()) );
}

DcVoltageWnd::~DcVoltageWnd()
{

}

void DcVoltageWnd::setIo( VoltampIo * io, MainWnd * mainWnd )
{
    this->io = io;
    this->mainWnd = mainWnd;
}

void DcVoltageWnd::slotChange()
{
    QString ss = "background-color: rgb(185, 10, 10);\nborder-color: rgb(185, 10, 10);";
    ui.value->setStyleSheet( ss );
}

void DcVoltageWnd::slotApply()
{
    qreal volt = ui.value->value();
    int dacLow, dacHigh;
    mainWnd->dac( volt, dacLow, dacHigh );
    bool res = io->set_dac_raw( dacLow, dacHigh );

    QString ss = "background-color: rgb(10, 185, 10);\nborder-color: rgb(10, 185, 10);";
    ui.value->setStyleSheet( ss );
}



