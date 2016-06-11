
#ifndef __POLARIZATION_DLG_H_
#define __POLARIZATION_DLG_H_

#include "ui_polarization_dlg.h"

class MainWnd;

class PolarizationDlg: public QDialog
{
    Q_OBJECT
public:
    PolarizationDlg( MainWnd * mainWnd );
    ~PolarizationDlg();

    void setPotential( qreal v );
    qreal potential() const;

    void setDuration( qreal ms );
    qreal duration() const;

    void setMeasureDuraton( qreal t );
    qreal measureDuration() const;

private:
    Ui_PolarizationDlg ui;
};



#endif



