
#ifndef __SWEEP_WND_H_
#define __SWEEP_WND_H_

#include <QtGui>
#include "ui_sweep_wnd.h"

class OscilloscopeWnd;

class SweepWnd: public QMainWindow
{
    Q_OBJECT
public:
    SweepWnd( QWidget * parent = 0 );
    ~SweepWnd();

    void setTitles( const QString & striA, const QString & striB );

    void addData( QMutex & m, QQueue<qreal> & workV, QQueue<qreal> & workI, QQueue<qreal> & probeV, QQueue<qreal> & probeI, qreal timeScale = -1.0 );
    void addData( QQueue<qreal> & workV, QQueue<qreal> & workI, QQueue<qreal> & probeV, QQueue<qreal> & probeI, qreal timeScale = -1.0 );

    static SweepWnd * loadFile( QWidget * parent = 0 );
    void load( const QString & fileName );

    bool eventFilter( QObject * obj, QEvent * e );

protected:
    void showEvent( QShowEvent * e );
    void closeEvent( QCloseEvent * e );
    void data( QVector<qreal> & workV, QVector<qreal> & workI, QVector<qreal> & probeV, QVector<qreal> & probeI );
    
public slots:
    void slotSave();
    void slotQuit();
    void tileVertically();
    void tileHorizontally();
private:
    Ui_sweep_wnd ui;
    OscilloscopeWnd * work;
    OscilloscopeWnd * probe;
    bool shouldBeSaved;
    int ptIndex;
};




#endif


