
#ifndef __FACTORY_PRX_H_
#define __FACTORY_PRX_H_

#include <string>
#include <vector>
#include "Ice/Ice.h"
#include "factory.h"

#include "digitizer.h"
class DigitizerPrx;

class FactoryPrx: public ::Ice::Object
{
public:
    typedef IceUtil::Handle<FactoryPrx> Factory;

    ~FactoryPrx();

    static Factory factory( const std::string & host = "", int port = 31415 );
    DigitizerPrx * digitizer();

    std::string status();
protected:
    void setStatus( const std::string & stri );
    ::IceUtil::Mutex &    mutex();
    ::Device::DigitizerPrx digitizerPrx();

private:
    FactoryPrx();
private:
    class PD;
    PD * pd;

    friend class DigitizerPrx;
};



#endif


