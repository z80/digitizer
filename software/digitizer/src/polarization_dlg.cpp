
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






