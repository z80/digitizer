
#include "polarization_dlg.h"
#include "main_wnd.h"

PolarizationDlg::PolarizationDlg( MainWnd * mainWnd )
    : QDialog( mainWnd )
{
    ui.setupUi( this );
}

PolarizationDlg::~PolarizationDlg()
{
}

void PolarizationDlg::setPotential( qreal v )
{
    ui.potential->setValue( v );
}

qreal PolarizationDlg::potential() const
{
    qreal res = ui.potential->value();
    return res;
}

void PolarizationDlg::setDuration( qreal ms )
{
    ui.duration->setValue( ms );
}

qreal PolarizationDlg::duration() const
{
    qreal res = ui.duration->value();
    return res;
}

void PolarizationDlg::setMeasureDuraton( qreal t )
{
    ui.recordDuration->setValue( t );
}

qreal PolarizationDlg::measureDuration() const
{
    qreal res = ui.recordDuration->value();
    return res;
}

void PolarizationDlg::setApplyTo( int index )
{
    if ( index > 0 )
    {
        ui.e1->setChecked( false );
        ui.e2->setChecked( true );
    }
    else
    {
        ui.e2->setChecked( false );
        ui.e1->setChecked( true );
    }
}

int PolarizationDlg::applyTo() const
{
    if ( ui.e1->isChecked() )
        return 0;
    return 1;
}

void PolarizationDlg::setPull( bool pull )
{
    ui.pull->setChecked( pull );
}

bool PolarizationDlg::pull() const
{
    return ui.pull->isChecked();
}









