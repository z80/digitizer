
#ifndef __OSCILLOSCOPE_WND_H_
#define __OSCILLOSCOPE_WND_H_

#include <QtGui>
#include <QFuture>
#include "voltamp_io.h"
#include "ui_oscilloscope_wnd.h"
#include "curve.h"

class MainWnd;

class OscilloscopeWnd: public QMainWindow
{
    Q_OBJECT
public:
    OscilloscopeWnd( QWidget * parent = 0 );
    ~OscilloscopeWnd();

    void setPeriod( qreal sec );
    void addData( const QVector<qreal> & y );
    void addData( const QVector<qreal> & x, const QVector<qreal> & y );
    void clear();
private slots:
    void slotCurveType();
    void slotPeriod();
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




