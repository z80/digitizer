
#include "host_tray.h"
#include "main_wnd.h"
#include <sstream>

HostTray::HostTray( QObject * parent )
: QSystemTrayIcon( parent )
{
    setIcon( QIcon( ":images/icon.png" ) );
    setToolTip( "Potentiostat" );

    m_menu = new QMenu();
    m_menu->setTitle( "Menu" );
    m_menu->setTearOffEnabled( true );

    m_actionShow = new QAction( "Show window", m_menu );
    m_actionShow->setCheckable( false );
    connect( m_actionShow, SIGNAL(triggered()), this, SLOT(slotShowMainWnd()) );
    m_menu->addAction( m_actionShow );

    m_actionQuit = new QAction( "Quit", m_menu );
    connect( m_actionQuit, SIGNAL(triggered()), this, SLOT(slotQuit()) );
    m_menu->addAction( m_actionQuit );

    setContextMenu( m_menu );

    m_mainWnd = new MainWnd( this );

    connect( this, SIGNAL(activated( QSystemTrayIcon::ActivationReason )), this, SLOT( slotActivted( QSystemTrayIcon::ActivationReason ) ) );

    show();
}

HostTray::~HostTray()
{
}

void HostTray::slotActivted( QSystemTrayIcon::ActivationReason reason )
{
    if ( ( reason == QSystemTrayIcon::DoubleClick ) || ( reason == QSystemTrayIcon::Trigger ) || ( reason == QSystemTrayIcon::MiddleClick ) )
    {
        if ( m_mainWnd->isVisible() )
            m_mainWnd->hide();
        else
            m_mainWnd->show();
    }
}

void HostTray::slotShowMainWnd()
{
    m_mainWnd->show();
}


void HostTray::slotQuit()
{
    hide();
    m_mainWnd->slotQuit();
    this->deleteLater();
    QApplication::quit();
}


