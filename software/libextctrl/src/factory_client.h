
#ifndef __FACTORY_CLIENT_H_
#define __FACTORY_CLIENT_H_

#include <string>
class DigitizerPrx;

class FactoryClient
{
public:
    static FactoryClient * factory( const std::string & host = "", int port = 21345 );
    ~FactoryClient();

    std::string status();
    static const std::string & version();
    DigitizerPrx * digitizer();

private:
    FactoryClient();
    class PD;
    PD * pd;
};



#endif

