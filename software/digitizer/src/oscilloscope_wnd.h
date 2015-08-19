
#ifndef __OSCILLOSCOPE_WND_H_
#define __OSCILLOSCOPE_WND_H_

#include <QtGui>
#include <QFuture>
#include "voltamp_io.h"
#include "ui_oscilloscope_wnd.h"
#include "curve.h"

class MainWnd;
class QwtPlot;

class OscilloscopeWnd: public QMainWindow
{
    Q_OBJECT
public:
    OscilloscopeWnd( QWidget * parent = 0 );
    ~OscilloscopeWnd();

    void setPeriod( qreal sec );
    void addData( QQueue<qreal> & y );
    void addData( QQueue<qreal> & x, QQueue<qreal> & y );
    void clear();

    void data( QVector<qreal> & x, QVector<qreal> & y );

    QwtPlot * plot();
public slots:
    //void slotCurveType();
    //void slotPeriod();
    void slotReplot();
private:

    void curveSizeChanged();
    void curvesCntChanged();
    void copyData( QQueue<qreal> & src, QQueue<qreal> & dest, int cnt );

    Ui_OscilloscopeWnd ui;

    enum CurveType { TT, TP };

    CurveType curveType;
    qreal     period;
    qreal     timeScale;
    int       ptIndex;
    QVector<qreal> dataX;

    QVector<Curve>   curves;

    static const int CURVES_CNT;
    static const int PTS_CNT;
};

#endif




