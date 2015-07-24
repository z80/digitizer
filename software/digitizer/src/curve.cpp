
#include "curve.h"
#include "qwt_plot.h"
#include "qwt_plot_curve.h"

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



