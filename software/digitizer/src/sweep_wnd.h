
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

    void addData( QMutex & m, QQueue<qreal> & workV, QQueue<qreal> & workI, QQueue<qreal> & probeV, QQueue<qreal> & probeI );
    void load( const QString & fileName );

protected:
    void showEvent( QShowEvent * e );
    
public slots:
    void slotSave();
    void slotQuit();
    void tileVertically();
    void tileHorizontally();
private:
    Ui_sweep_wnd ui;
    OscilloscopeWnd * work;
    OscilloscopeWnd * probe;
};




#endif


