
#include "factory_client.h"
#include "factory_version.h"
#include "factory_prx.h"

class FactoryClient::PD
{
public:
    PD() {}
    ~PD() {}
    FactoryPrx::Factory factory;
    static const std::string version;
};

const std::string FactoryClient::PD::version = FACTORY_VERSION;

FactoryClient * FactoryClient::factory( const std::string & host, int port )
{
    FactoryPrx::Factory factory = FactoryPrx::factory( host, port );
    if ( !factory )
        return 0;
    FactoryClient * res = new FactoryClient();
    res->pd->factory = factory;
    return res;
}

FactoryClient::~FactoryClient()
{
    delete pd;
}

std::string FactoryClient::status()
{
    return pd->factory->status();
}

const std::string & FactoryClient::version()
{
    return PD::version;
}

DigitizerPrx * FactoryClient::digitizer()
{
    DigitizerPrx * res = pd->factory->digitizer();
    return res;
}


FactoryClient::FactoryClient()
{
    pd = new PD();
}




