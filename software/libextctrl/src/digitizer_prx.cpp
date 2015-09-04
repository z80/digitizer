
#include "digitizer_prx.h"
#include "Ice/Ice.h"
#include "digitizer.h"
#include "factory_prx.h"

class DigitizerPrx::PD
{
public:
    ::Device::DigitizerPrx      prx;
    IceUtil::Handle<FactoryPrx> factory;
};

DigitizerPrx::DigitizerPrx( FactoryPrx * owner )
{
    pd = new PD();
    pd->factory = owner;
    pd->prx     = owner->digitizerPrx();
}

DigitizerPrx::~DigitizerPrx()
{
    delete pd;
}

bool DigitizerPrx::instantValues( qreal & workV, qreal & workI, qreal & probeV, qreal & probeI )
{
    int res;
    try {
        res = pd->prx->instantValues( workV, workI, probeV, probeI );
    } catch ( const Ice::Exception & e )
    {
        res = false;
        pd->factory->setStatus( std::string( "error: " ) + e.ice_name() );
    }
    return res;
}

bool DigitizerPrx::setTrigEn( bool en )
{
    bool res;
    try {
        res = pd->prx->setTrigEn( en );
    } catch ( const Ice::Exception & e )
    {
        res = false;
        pd->factory->setStatus( std::string( "error: " ) + e.ice_name() );
    }
    return res;
}

bool DigitizerPrx::values( std::vector<qreal> & workV, std::vector<qreal> & workI, std::vector<qreal> & probeV, std::vector<qreal> & probeI )
{
    bool res;
    try {
        res = pd->prx->values( workV, workI, probeV, probeI );
    } catch ( const Ice::Exception & e )
    {
        res = false;
        pd->factory->setStatus( std::string( "error: " ) + e.ice_name() );
    }
    return res;
}










