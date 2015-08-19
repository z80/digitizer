
#include "sweep_wnd.h"
#include <QMdiArea>
#include <QMdiSubwindow>
#include <QFileDialog>
#include <QMessageBox>
#include "oscilloscope_wnd.h"

SweepWnd::SweepWnd( QWidget * parent )
: QMainWindow( parent )
{
    ui.setupUi( this );

    setAttribute( Qt::WA_DeleteOnClose );
    setWindowTitle( "*Untitled" );

    work  = new OscilloscopeWnd( 0 );
    work->setWindowTitle( "Work electrode I(V)" );
    work->setWindowIcon( QIcon( ":images/icon.png" ) );

    probe = new OscilloscopeWnd( 0 );
    probe->setWindowTitle( "Probe electrode I(V)" );
    probe->setWindowIcon( QIcon( ":images/icon.png" ) );

    ui.mdiArea->addSubWindow( work );
    ui.mdiArea->addSubWindow( probe );

    connect( ui.actionTile_vertically,   SIGNAL(triggered()), this, SLOT(tileVertically()) );
    connect( ui.actionTile_horizontally, SIGNAL(triggered()), this, SLOT(tileHorizontally()) );
}

SweepWnd::~SweepWnd()
{
}

void SweepWnd::addData( QMutex & m, QQueue<qreal> & workV, QQueue<qreal> & workI, QQueue<qreal> & probeV, QQueue<qreal> & probeI )
{
    QMutexLocker lock( &m );
        work->addData(  workV,  workI );
        probe->addData( probeV, probeI );
        work->slotReplot();
        probe->slotReplot();
}

void SweepWnd::load( const QString & fileName )
{
}

void SweepWnd::showEvent( QShowEvent * e )
{
    QMainWindow::showEvent( e );
    tileVertically();
}

void SweepWnd::data( QVector<qreal> & workV, QVector<qreal> & workI, QVector<qreal> & probeV, QVector<qreal> & probeI )
{
    work->data( workV, workI );
    probe->data( probeV, probeI );
}

void SweepWnd::slotSave()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "", tr("Text Files (*.txt)"));
    if ( fileName.length() > 0 )
    {
        if ( !fileName.toLower().endsWith( ".txt" ) )
            fileName.append( ".txt" );

        QFile f( fileName );
        if ( f.exists( fileName ) )
        {
            QMessageBox::StandardButton res = QMessageBox::warning( this, "Warning", QString( "Specified file \"%1\" exists! Overwrite?" ).arg( fileName ) );
            if ( res != QMessageBox::Ok )
                return;
        }

        if ( !f.open( QIODevice::WriteOnly ) )
        {
            return;
        }

        QTextStream out( &f );

        out << "workV[mV]; workI[mV]; probeV[mV]; probeI[mV]\r\n"; // Yes, I in [mV] due to module measures [mV].
        QVector<qreal> workV, workI, probeV, probeI;
        data( workV, workI, probeV, probeI );
        int sz = workV.size();
        for ( int i=0; i<sz; i++ )
        {
            out << QString( "%1; %2; %3; %4\r\n" ).arg( workV.at( i ) )
        }

        out.flush();
        f.close();
    }
}

void SweepWnd::slotQuit()
{
}

void SweepWnd::tileVertically()
{
    QPoint position(0, 0);

    foreach ( QMdiSubWindow * window, ui.mdiArea->subWindowList() )
    {
        QRect rect(0, 0, ui.mdiArea->width(), 
                   ui.mdiArea->height() / ui.mdiArea->subWindowList().count());
        window->setGeometry(rect);
        window->move(position);
        position.setY(position.y() + window->height());
    }
}

void SweepWnd::tileHorizontally()
{
    if (ui.mdiArea->subWindowList().isEmpty())
        return;

    QPoint position(0, 0);

    foreach ( QMdiSubWindow * window, ui.mdiArea->subWindowList() )
    {
        QRect rect(0, 0, ui.mdiArea->width() / ui.mdiArea->subWindowList().count(), 
                   ui.mdiArea->height());
        window->setGeometry(rect);
        window->move(position);
        position.setX(position.x() + window->width());
    }
}









