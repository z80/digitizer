
#ifndef __EXEC_H_
#define __EXEC_H_

#include <boost/function.hpp>

#include <QtGui>

class Exec: public QThread
{
    Q_OBJECT
public:
    typedef boost::function<void ()> Func;

    Exec( QObject * parent = 0 );
    ~Exec();

    void run( Func func );
protected:
    void run();
private:
    class PD;
    PD * pd;
};

#endif

