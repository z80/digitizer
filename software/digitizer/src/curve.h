
#ifndef __CURVE_H_
#define __CURVE_H_

#include <QtCore>
class QwtPlot;
class QwtPlotCurve;

class Curve
{
public:
    Curve();
    ~Curve();
    Curve( const Curve & inst );
    const Curve & operator=( const Curve & inst );
    void resize( int sz );
    void attach( QwtPlot * plot );
    void prepare();

    QVector<qreal> x, y;
    int cnt;
    QwtPlotCurve * curve;
};


#endif


