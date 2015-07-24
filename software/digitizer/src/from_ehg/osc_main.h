
#ifndef __OSC_MAIN_H_
#define __OSC_MAIN_H_

#include "ui_osc_main.h"
#include <QtGui>

class CUsbIo;
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

class COscMain: public QMainWindow
{
    Q_OBJECT
public:
    COscMain();
    ~COscMain();
signals:
    void sigReplot();
private slots:
    void open();
    void start();
    void settingsChanged();
    void curvesCntChanged();
    void replot();
private:
    void setTimer();
    void measure();
    Ui_OscMain ui;
    QMutex     m_mutex;
    CUsbIo   * m_dev;
    QSpinBox * m_ptsPerSecond;
    QSpinBox * m_seconds;
    QSpinBox * m_curvesCnt;
    QVector<Curve> m_curves;
    QVector<qreal> m_data;
    QVector<qreal> m_paintData;
    bool m_running;
    QFuture<void> m_future;
};


#endif

