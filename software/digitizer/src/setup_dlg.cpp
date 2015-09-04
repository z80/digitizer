
#include "setup_dlg.h"
#include <QtNetwork>

SetupDlg::SetupDlg( QWidget * parent )
: QDialog( parent )
{
    ui.setupUi( this );
    QList<QHostAddress> l = QNetworkInterface::allAddresses();
    QStringList sl;
    sl.reserve( l.size() + 1 );
    sl << "All interfaces";
    for ( QList<QHostAddress>::const_iterator i=l.begin(); i!=l.end(); i++ )
    {
        QString ip = (*i).toString();
        sl << ip;
    }
    ui.address->addItems( sl );
    ui.address->setCurrentIndex( 0 );
}

SetupDlg::~SetupDlg()
{
}

void SetupDlg::setPort( int val )
{
    ui.port->setValue( val );
}

int SetupDlg::port() const
{
    return ui.port->value();
}

QString SetupDlg::host() const
{
    return ui.address->currentText();
}

void SetupDlg::setListen( bool en )
{
    ui.listen->setChecked( en );
}

bool SetupDlg::listen() const
{
    return ui.listen->isChecked();
}

void SetupDlg::closeEvent( QCloseEvent * e )
{
    e->ignore();
}






