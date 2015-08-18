
#include "exec.h"

class Exec::PD
{
public:
    Exec::Func func;
};

Exec::Exec( QObject * parent )
: QThread( parent )
{
    pd = new PD();
}

Exec::~Exec()
{
    delete pd;
}

void Exec::run( Func func )
{
    pd->func = func;
    start();
}

void Exec::run()
{
    if ( !pd->func.empty() )
        pd->func();
}



