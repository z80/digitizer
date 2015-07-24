
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

    bool isRunning() const;
    void setIo( VoltampIo * io, MainWnd * mainWnd );
    void mostRecentVals( qreal & eaux, qreal & eref, qreal & iaux );
    void mostRecentValsRaw( int & eaux, int & eref, int & iaux );

    void stop();
signals:
    void sigReplot();
private slots:
    void slotTimeout();
    void slotCurveType();
    void slotPeriod();
    void slotReplot();
private:

    void measure();
    void reopen();
    void curveSizeChanged();
    void curvesCntChanged();
    void copyData( QQueue<qreal> & src, QQueue<qreal> & dest, int cnt );

    enum CurveType { EAUX_T=0, EREF_T, IAUX_T, I_EAUX, I_EREF };
    enum Period    { T_1s=0, T_10s, T_1m, T_10m };

    Ui_OscilloscopeWnd ui;
    VoltampIo * io;
    MainWnd   * mainWnd;
    QTimer    * timer;
    QMutex      mutex;
    QFuture<void> future;

    CurveType curveType;
    Period    period;
    qreal     timeScale;
    QVector<quint16>  data;
    QVector<quint16>  eaux_m, eref_m, iaux_m;
    QQueue<qreal>     eaux,    eref,    iaux;
    QVector<qreal>    luaEaux, luaEref, luaIaux;
    QQueue<qreal>     paintDataX, paintDataY;

    QVector<Curve>   curves;

    qreal lastEaux, lastEref, lastIaux;
    int   lastEauxRaw, lastErefRaw, lastIauxRaw;
    bool  terminate;

    static const int CURVES_CNT;
    static const int PTS_CNT;
};

#endif




