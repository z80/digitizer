
#include "osc_main.h"
#include "osc_scaler.h"
#include "qwt_plot.h"
#include "qwt_plot_canvas.h"
#include "qwt_plot_layout.h"
#include "qwt_plot_grid.h"
#include "qwt_plot_curve.h"
#include "usb_io.h"
#include <boost/bind.hpp>
#include <boost/bind/arg.hpp>
#include <boost/bind/placeholders.hpp>


Curve::Curve()
{
    curve = new QwtPlotCurve();
    cnt = 0;
}

Curve::~Curve()
{
    delete curve;
}

Curve::Curve( const Curve & inst )
{
    curve = new QwtPlotCurve();
    *this = inst;
}

const Curve & Curve::operator=( const Curve & inst )
{
    if ( this != &inst )
    {
        x   = inst.x;
        y   = inst.y;
        cnt = inst.cnt;
    }
    return *this;
}

void Curve::resize( int sz )
{
    x.resize( sz );
    y.resize( sz );
}

void Curve::attach( QwtPlot * plot )
{
    curve->attach( plot );
}

void Curve::prepare()
{
    curve->setRawSamples( x.data(), y.data(), cnt );
}



class CDelay: public QThread
{
public:
    static void delay( int ms )
    {
        msleep( ms );
    }
};




COscMain::COscMain()
: QMainWindow()
{
    m_dev = new CUsbIo();
    m_running = false;

    ui.setupUi( this );
    connect( ui.actionQuit, SIGNAL(triggered()), this, SLOT(close()) );
    connect( ui.actionRun, SIGNAL(triggered()), this, SLOT(start()) );
    connect( ui.actionConnect, SIGNAL(triggered()), this, SLOT(open()) );
    COscScaler * scaler = new COscScaler( ui.plot );
    scaler->setWheelZoomX( true );
    scaler->setWheelZoomY( true );
    scaler->setEqualScales( false );
    scaler->setSaveScales( false );

    QwtPlotGrid * g = new QwtPlotGrid();
    g->enableXMin( true );
    g->enableYMin( true );
    g->setPen( QPen( Qt::gray, 0.0, Qt::DotLine ) );
    g->attach( ui.plot );

    g = new QwtPlotGrid();
    g->enableX( true );
    g->enableY( true );
    g->setPen( QPen( Qt::gray, 0.0, Qt::SolidLine ) );
    g->attach( ui.plot );


    connect( this, SIGNAL(sigReplot()), this, SLOT(replot()), Qt::QueuedConnection );

    ui.plot->canvas()->setBorderRadius( 10 );
    ui.plot->plotLayout()->setAlignCanvasToScales( true );

    // Nice background coloration.
    QPalette pal = ui.plot->canvas()->palette();
    QLinearGradient gr( 0.0, 0.0, 1.0, 1.0 );
    gr.setCoordinateMode( QGradient::StretchToDeviceMode );
    gr.setColorAt( 0.0, QColor( 200, 200, 230 ) );
    gr.setColorAt( 1.0, QColor( 230, 230, 250 ) );
    pal.setBrush( QPalette::Window, QBrush( gr ) );
    ui.plot->canvas()->setPalette( pal );

    m_ptsPerSecond = new QSpinBox();
    m_ptsPerSecond->setRange( 1, 128 );
    m_ptsPerSecond->setPrefix( tr( "Pts/sec" ) );
    m_ptsPerSecond->setValue( 50 );
    connect( m_ptsPerSecond, SIGNAL(editingFinished()), this, SLOT(settingsChanged()) );

    m_seconds = new QSpinBox();
    m_seconds->setRange( 1, 600 );
    m_seconds->setPrefix( tr( "Seconds" ) );
    m_seconds->setValue( 3 );
    connect( m_seconds, SIGNAL(editingFinished()), this, SLOT(settingsChanged()) );

    m_curvesCnt = new QSpinBox();
    m_curvesCnt->setRange( 1, 10 );
    m_curvesCnt->setValue( 3 );
    m_curvesCnt->setPrefix( tr( "Curves" ) );
    connect( m_curvesCnt, SIGNAL(editingFinished()), this, SLOT(curvesCntChanged()) );

    ui.sb->addWidget( m_ptsPerSecond );
    ui.sb->addWidget( m_seconds );
    ui.sb->addWidget( m_curvesCnt );

    ui.sb->adjustSize();

    open();
    curvesCntChanged();
    settingsChanged();
    /*if ( m_dev->isOpen() )
    {
        
        m_dev->setTimer( 12000, CUsbIo::T1024 );
        m_dev->setTimer( 6000,  CUsbIo::T1024 );
        m_dev->setTimer( 255,   CUsbIo::T1024 );
    }*/
}

COscMain::~COscMain()
{
    if ( m_dev )
    {
        delete m_dev;
        m_dev = 0;
    }
}

void COscMain::open()
{
    m_mutex.lock();
    m_dev->close();
    bool res = m_dev->open();
    m_mutex.unlock();
    setWindowTitle( res ? "Ready" : "ERROR: No device connected!" );
}

void COscMain::start()
{
    // Сначала остановка.
    m_mutex.lock();
    m_running = false;
    m_mutex.unlock();

    while ( m_future.isRunning() )
        qApp->processEvents();

    if ( ui.actionRun->isChecked() )
    {
        // Если кнопка нажата, старт.
        m_mutex.lock();
        m_running = true;
        m_mutex.unlock();

        m_future = QtConcurrent::run( boost::bind( &COscMain::measure, this ) );
    }
}

void COscMain::settingsChanged()
{
    int pts_s   = m_ptsPerSecond->value();
    int seconds = m_seconds->value();
    int cnt = pts_s * seconds;
    int curvesCnt = m_curvesCnt->value();
    qreal scale = static_cast<qreal>( seconds ) / static_cast<qreal>( cnt-1 );
    for ( int i=0; i<curvesCnt; i++ )
    {
        Curve & c = m_curves[i];
        c.resize( cnt );
	c.cnt = 0;
        for ( int i=0; i<cnt; i++ )
        {
            qreal x = static_cast<qreal>(i) * scale;
            c.x[i] = x;
            c.y[i] = 0.0;
        }
    }
    ui.plot->setAxisScale( QwtPlot::xBottom, 0.0, static_cast<qreal>(seconds) );
    ui.plot->axisAutoScale( QwtPlot::yLeft );
    ui.plot->replot();
    setTimer();
}

void COscMain::curvesCntChanged()
{
    const QColor front( Qt::darkGreen );
    QColor back( 50, 50, 50 );
    int r1 = front.red();
    int g1 = front.green();
    int b1 = front.blue();
    int r2 = back.red();
    int g2 = back.green();
    int b2 = back.blue();
    int cnt = m_curvesCnt->value();
    m_curves.resize( cnt );

    for ( int i=0; i<cnt; i++ )
    {
        m_curves[i].attach( ui.plot );
	int r, g, b;
	if ( cnt > 1 )
	{
	    r = ((cnt-i-1) * r1 + i * r2)/(cnt-1);
	    g = ((cnt-i-1) * g1 + i * g2)/(cnt-1);
	    b = ((cnt-i-1) * b1 + i * b2)/(cnt-1);
	}
	else
	{
	    r = r1;
	    g = g1;
	    b = b1;
	}
        QPen pen;
        QColor color( r, g, b );
        pen.setColor( color );
        if ( i == 0 )
	{
            pen.setWidth( 3 );
	    m_curves[i].curve->setZ( 100.0 );
	}
        else
	{
            pen.setWidth( 1 );
	    m_curves[i].curve->setZ( 90.0 );
	}
        m_curves[i].curve->setPen( pen );
    }
    ui.plot->replot();
}

void COscMain::replot()
{
    m_mutex.lock();
    m_paintData = m_data;
    m_data.clear();
    m_mutex.unlock();
    int cnt = m_ptsPerSecond->value() * m_seconds->value();
    Curve & c = m_curves[0];
    int paintSz = m_paintData.size();
    for ( int i=0; i<paintSz; i++ )
    {
        c.y[c.cnt++] = m_paintData[i];
	if ( c.cnt >= cnt )
	{
	    for ( int j=(m_curves.size()-1); j>0; j-- )
	        m_curves[j] = m_curves[j-1];

	    c.cnt = 0;
 	    m_mutex.lock();
	    for ( int j=i+1; j<paintSz; j++ )
                m_data << m_paintData[j];
	    m_mutex.unlock();
	    break;
	}
    }
    for ( int i=0; i<m_curves.size(); i++ )
	m_curves[i].prepare();
    // Update plot.
    ui.plot->replot();
}

void COscMain::setTimer()
{
    const qreal CLK         = 12000000.0;
    const qreal TIMER_RES   = 65535.0;
    const int   DIV_SZ      = 5;
    const qreal DIVF[]      = { 1.0, 8.0, 64.0, 256.0, 1024.0 };
    const CUsbIo::TDiv DIV[]= { CUsbIo::T1, CUsbIo::T8, CUsbIo::T64, CUsbIo::T256, CUsbIo::T1024 };

    // Аппаратная установка таймера.
    qreal pts_s = static_cast<qreal>( m_ptsPerSecond->value() );
    qreal ticksf = CLK / pts_s;
    int divInd = 0;
    while ( ( ticksf/DIVF[divInd] >= TIMER_RES ) && (divInd < DIV_SZ) )
        divInd++;
    ticksf = ticksf/DIVF[divInd];
    ticksf = (ticksf < TIMER_RES) ? ticksf : TIMER_RES;
    int timer = static_cast<int>( ticksf );

    m_mutex.lock();
    if ( m_dev->isOpen() )
        m_dev->setTimer( timer, DIV[ divInd ] );
    m_mutex.unlock();
}

void COscMain::measure()
{
    QTime time;
    time.start();
    const int TIMEOUT = (1000 / 20);
    bool running;
    do {
        std::vector<unsigned short> data;
        m_mutex.lock();
        running = m_running;
	bool open = m_dev->isOpen();
	m_mutex.unlock();
        
	if ( open )
	{
	    m_mutex.lock();
            bool res = m_dev->data( data );
            int sz = data.size();
            for ( int i=0; (i<sz) && res; i++ )
                 m_data.append( static_cast<qreal>( data.at( i ) ) );
            m_mutex.unlock();
            if ( res && (sz > 0) && (time.elapsed() >= TIMEOUT) )
	    {
	        time.restart();
	        emit sigReplot();
	    }
	}
	else
	{
            m_mutex.lock();
	    for ( int i=0; i<4; i++ )
            {
	        m_data.append( sin(static_cast<qreal>(i)/3.1415926535/2/4) );
                CDelay::delay( 20 );
            }
	    m_mutex.unlock();
	    if ( time.elapsed() > TIMEOUT )
	    {
	        time.restart();
                emit sigReplot();
            }
	}
	CDelay::delay( 1 );
    } while ( running );
}


