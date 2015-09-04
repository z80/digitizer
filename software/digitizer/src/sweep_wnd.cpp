
#include "sweep_wnd.h"
#include <QMdiArea>
#include <QMdiSubWindow>
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
    work->setWindowTitle( "Work electrode I(V) [nA(mV)]" );
    work->setWindowIcon( QIcon( ":images/icon.png" ) );
    work->installEventFilter( this );

    probe = new OscilloscopeWnd( 0 );
    probe->setWindowTitle( "Probe electrode I(V) [nA(mV)]" );
    probe->setWindowIcon( QIcon( ":images/icon.png" ) );
    probe->installEventFilter( this );

    ui.mdiArea->addSubWindow( work );
    ui.mdiArea->addSubWindow( probe );

    connect( ui.actionTile_vertically,   SIGNAL(triggered()), this, SLOT(tileVertically()) );
    connect( ui.actionTile_horizontally, SIGNAL(triggered()), this, SLOT(tileHorizontally()) );

    connect( ui.actionClose, SIGNAL(triggered()),   this, SLOT(slotQuit()) );
    connect( ui.actionSave_as, SIGNAL(triggered()), this, SLOT(slotSave()) );

    shouldBeSaved = false;
}

SweepWnd::~SweepWnd()
{
}

void SweepWnd::addData( QMutex & m, QQueue<qreal> & workV, QQueue<qreal> & workI, QQueue<qreal> & probeV, QQueue<qreal> & probeI )
{
    QMutexLocker lock( &m );
        addData( workV, workI, probeV, probeI );
}

void SweepWnd::addData( QQueue<qreal> & workV, QQueue<qreal> & workI, QQueue<qreal> & probeV, QQueue<qreal> & probeI )
{
    work->addData(  workV,  workI );
    probe->addData( probeV, probeI );
    work->slotReplot();
    probe->slotReplot();
    shouldBeSaved = true;
}

SweepWnd * SweepWnd::loadFile( QWidget * parent )
{
    QString fileName = QFileDialog::getOpenFileName( parent,
        tr("Open file"), "", tr("Text Files (*.txt)"));
    if ( fileName.length() > 0 )
    {
        if ( !fileName.toLower().endsWith( ".txt" ) )
            fileName = QString( "%1%2" ).arg( fileName ).arg( ".txt" );

        QFile f( fileName );
        if ( !f.exists( fileName ) )
        {
            QMessageBox::StandardButton res = QMessageBox::critical( parent, "Error", QString( "Specified file \"%1\" does not exist!" ).arg( fileName ), QMessageBox::Ok );
            return 0;
        }
        SweepWnd * wnd = new SweepWnd( 0 );
        wnd->load( fileName );

        return wnd;
    }
    return 0;
}

void SweepWnd::load(const QString & fileName )
{
    if ( shouldBeSaved )
    {
        QMessageBox::StandardButton res = QMessageBox::warning( this, "Error", QString( "Current data is not saved! Load file anyway?" ), QMessageBox::Ok | QMessageBox::Cancel );
        if ( res != QMessageBox::Ok )
            return;
    }


    QFile f( fileName );
    if ( !f.open( QIODevice::ReadOnly ) )
    {
        QMessageBox::StandardButton res = QMessageBox::critical( this, "Error", QString( "Failed to open \"%1\" for writing!" ).arg( fileName ), QMessageBox::Ok );
        return;
    }

    QQueue<qreal> workV, workI, probeV, probeI;
    QByteArray arr = f.readLine(); // Skip title.
    while ( true )
    {
        arr = f.readLine();
        QString stri = QString::fromUtf8( arr );
        QRegExp ex( "(\\w+)[;\\s]+(\\w+)[;\\s]+(\\w+)[;\\s]+(\\w+)" );
        int index = ex.indexIn( stri );
        if ( index >= 0 )
        {
            QString m;
            m = ex.cap( 1 );
            qreal v = m.toDouble();
            workV.enqueue( v );

            m = ex.cap( 2 );
            v = m.toDouble();
            workI.enqueue( v );

            m = ex.cap( 3 );
            v = m.toDouble();
            probeV.enqueue( v );

            m = ex.cap( 4 );
            v = m.toDouble();
            probeI.enqueue( v );
        }
        else
            break;
    }

    f.close();

    addData( workV, workI, probeV, probeI );

    shouldBeSaved = false;
    setWindowTitle( fileName );
}

bool SweepWnd::eventFilter( QObject * obj, QEvent * e )
{
    if ( e->type() == QEvent::Close )
    {
        QMessageBox::information( this, "Information", QString( "Please, don\'t close subwindows. Close entire data window instead." ), QMessageBox::Ok );
        e->setAccepted( false );
        return true;
    }
    return false;
}

void SweepWnd::showEvent( QShowEvent * e )
{
    QMainWindow::showEvent( e );
    tileVertically();
}

void SweepWnd::closeEvent( QCloseEvent * e )
{
    if ( shouldBeSaved )
    {
        QMessageBox::StandardButton res = QMessageBox::warning( this, "Error", QString( "Current data is not saved! Close anyway?" ), QMessageBox::Ok | QMessageBox::Cancel );
        if ( res != QMessageBox::Ok )
        {
            e->setAccepted( false );
            return;
        }
    }
    QMainWindow::closeEvent( e );
}

void SweepWnd::data( QVector<qreal> & workV, QVector<qreal> & workI, QVector<qreal> & probeV, QVector<qreal> & probeI )
{
    work->data( workV, workI );
    probe->data( probeV, probeI );
}

void SweepWnd::slotSave()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save file"), "", tr("Text Files (*.txt)"));
    if ( fileName.length() > 0 )
    {
        if ( !fileName.toLower().endsWith( ".txt" ) )
            fileName.append( ".txt" );

        QFile f( fileName );
        if ( f.exists( fileName ) )
        {
            QMessageBox::StandardButton res = QMessageBox::warning( this, "Warning", QString( "Specified file \"%1\" exists! Overwrite?" ).arg( fileName ), QMessageBox::Ok | QMessageBox::Cancel );
            if ( res != QMessageBox::Ok )
                return;
        }

        if ( !f.open( QIODevice::WriteOnly ) )
        {
            QMessageBox::StandardButton res = QMessageBox::critical( this, "Error", QString( "Failed to open \"%1\" for writing!" ).arg( fileName ), QMessageBox::Ok );
            return;
        }

        QTextStream out( &f );

        out << "workV[mV]; workI[mV]; probeV[mV]; probeI[mV]\r\n"; // Yes, I in [mV] due to module measures [mV].
        QVector<qreal> workV, workI, probeV, probeI;
        data( workV, workI, probeV, probeI );
        int sz = workV.size();
        for ( int i=0; i<sz; i++ )
            out << QString( "%1; %2; %3; %4\r\n" ).arg( workV.at( i ) ).arg( workI.at( i ) ).arg( probeV.at( i ) ).arg( probeI.at( i ) );

        out.flush();
        f.close();

        shouldBeSaved = false;

        setWindowTitle( fileName );
    }
}

void SweepWnd::slotQuit()
{
    close();
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









