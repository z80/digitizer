
#ifndef __SETUP_DLG_H_
#define __SETUP_DLG_H_

#include <QtGui>
#include "ui_setup_dlg.h"

class SetupDlg: public QDialog
{
    Q_OBJECT
public:
    SetupDlg( QWidget * parent = 0 );
    ~SetupDlg();

    void setPort( int val );
    int port() const;
    QString host() const;

    void setListen( bool en );
    bool listen() const;
protected:
    void closeEvent( QCloseEvent * e );
private:
    Ui_SetupDlg ui;
};

#endif



