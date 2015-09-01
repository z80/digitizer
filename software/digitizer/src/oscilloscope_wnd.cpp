
#include <QtConcurrent/QtConcurrentRun>
#include "oscilloscope_wnd.h"
#include "main_wnd.h"

#include "osc_scaler.h"
#include "qwt_plot.h"
#include "qwt_plot_canvas.h"
#include "qwt_plot_layout.h"
#include "qwt_plot_grid.h"
#include "qwt_plot_curve.h"

#include <boost/bind.hpp>
#include <boost/bind/arg.hpp>
#include <boost/bind/placeholders.hpp>

const int OscilloscopeWnd::CURVES_CNT = 3;
const int OscilloscopeWnd::PTS_CNT = 1024;

OscilloscopeWnd::OscilloscopeWnd( QWidget * parent )
    : QMainWindow( parent )
{

    ui.setupUi( this );

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


    qobject_cast<QwtPlotCanvas *>(ui.plot->canvas())->setBorderRadius( 10 );
    ui.plot->plotLayout()->setAlignCanvasToScales( true );

    // Nice background coloration.
    QPalette pal = ui.plot->canvas()->palette();
    QLinearGradient gr( 0.0, 0.0, 1.0, 1.0 );
    gr.setCoordinateMode( QGradient::StretchToDeviceMode );
    gr.setColorAt( 0.0, QColor( 200, 200, 230 ) );
    gr.setColorAt( 1.0, QColor( 230, 230, 250 ) );
    pal.setBrush( QPalette::Window, QBrush( gr ) );
    ui.plot->canvas()->setPalette( pal );

    connect( this,  SIGNAL(sigReplot()), this, SLOT(slotReplot()) );

    curvesCntChanged();
    curveSizeChanged();
    setPeriod( 10.0 );
}

OscilloscopeWnd::~OscilloscopeWnd()
{
}

void OscilloscopeWnd::setPeriod( qreal sec )
{
    period = sec;
    curveSizeChanged();
}

void OscilloscopeWnd::addData( QQueue<qreal> & y )
{
    int sz = y.size();
    Curve & c = curves[0];

    while ( y.size() > 0 )
    {
        qreal v = y.dequeue();
        c.y[c.cnt] = v;
        c.x[c.cnt] = period * static_cast<qreal>( c.cnt ) / static_cast<qreal>( PTS_CNT );
        c.cnt++;
        if ( c.cnt >= PTS_CNT )
        {
            for ( int j=(curves.size()-1); j>0; j-- )
                curves[j] = curves[j-1];

            c.cnt = 0;
        }
    }
}

void OscilloscopeWnd::addData( QQueue<qreal> & x, QQueue<qreal> & y )
{
    int sz = ( y.size() < x.size() ) ? y.size() : x.size();
    Curve & c = curves[0];

    int newSz = c.cnt + sz;
    if ( c.x.size() < newSz )
        c.x.resize( newSz );
    if ( c.y.size() < newSz )
        c.y.resize( newSz );

    for ( int i=0; i<sz; i++ )
    {
        qreal vx = x.dequeue();
        qreal vy = y.dequeue();
        c.y[c.cnt] = vy;
        c.x[c.cnt] = vx;
        c.cnt++;
    }
}

void OscilloscopeWnd::clear()
{
    for ( int j=0; j<0; j++ )
        curves[j].cnt = 0;
}

void OscilloscopeWnd::data( QVector<qreal> & x, QVector<qreal> & y )
{
    Curve & c = curves[0];
    x = c.x;
    y = c.y;
}

QwtPlot * OscilloscopeWnd::plot()
{
    return ui.plot;
}

void OscilloscopeWnd::slotReplot()
{
    // Plot curves.
    for ( int i=0; i<curves.size(); i++ )
        curves[i].prepare();
    // Update plot.
    ui.plot->replot();
}

void OscilloscopeWnd::curveSizeChanged()
{
    //int curvesCnt = CURVES_CNT;
    int cnt       = PTS_CNT;

    int curvesCnt = curves.size();
    for ( int i=0; i<curvesCnt; i++ )
    {
        Curve & c = curves[i];
        c.x.resize( cnt );
        c.y.resize( cnt );
        c.cnt = 0;
    }
}

void OscilloscopeWnd::curvesCntChanged()
{
    const QColor front( Qt::darkRed );
    QColor back( 20, 0, 0 );
    int r1 = front.red();
    int g1 = front.green();
    int b1 = front.blue();
    int r2 = back.red();
    int g2 = back.green();
    int b2 = back.blue();
    int cnt = CURVES_CNT;
    curves.resize( cnt );

    for ( int i=0; i<cnt; i++ )
    {
        curves[i].attach( ui.plot );
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
            pen.setWidth( cnt );
            pen.setStyle( Qt::SolidLine );
            curves[i].curve->setZ( 100.0 );
        }
        else
        {
            pen.setWidth( cnt - i );
            pen.setStyle( Qt::DotLine );
            curves[i].curve->setZ( 90.0 );
        }
        curves[i].curve->setPen( pen );
    }
    ui.plot->replot();
}

void OscilloscopeWnd::copyData( QQueue<qreal> & src, QQueue<qreal> & dest, int cnt )
{
    for ( int i=0; i<cnt; i++ )
        dest.enqueue( src.dequeue() );
}





