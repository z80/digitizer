
#include "factory_prx.h"
#include "digitizer_prx.h"
#include <sstream>

class FactoryPrx::PD
{
public:
    Ice::CommunicatorPtr        comm;
    //Ice::ObjectAdapterPtr       adapter;
    
    ::Factory::DeviceFactoryPrx factory;
    ::Device::DigitizerPrx      digitizer;
    std::string                 status;

    IceUtil::Mutex  mutex;
};

FactoryPrx::Factory FactoryPrx::factory( const std::string & host, int port )
{
    FactoryPrx::Factory result;
    try {
        Ice::InitializationData initData;
        Ice::PropertiesPtr      props;
        props = Ice::createProperties();
        props->setProperty( "client_adapter.Endpoints", "tcp" );
        initData.properties = props;
        Ice::CommunicatorPtr comm = Ice::initialize( initData );
        //Ice::CommunicatorPtr comm = Ice::initialize();
        //Ice::ObjectAdapterPtr adapter = comm->createObjectAdapter( "adapter" );
        std::ostringstream os;
        if ( host.length() > 0 )
            os << "factory: tcp -h " << host << " -p " << port;
        else
            os << "factory: tcp -p " << port;
        ::Factory::DeviceFactoryPrx factory = ::Factory::DeviceFactoryPrx::uncheckedCast( comm->stringToProxy( os.str() ) );
        if ( !factory )
            return 0;
        ::Factory::DeviceAbsPrx dev;

        dev = factory->query( "digitizer" );
        if ( !dev )
            return 0;
        ::Device::DigitizerPrx digi = ::Device::DigitizerPrx::uncheckedCast( dev );
        if ( !digi )
            return 0;


        FactoryPrx * f = new FactoryPrx();
        f->pd->comm    = comm;
        //f->pd->adapter = adapter;
        f->pd->factory = factory;

        f->pd->digitizer = digi;

        f->pd->status = "connected";

        result = f;
    } catch ( const Ice::Exception & e )
    {
        qDebug() << e.ice_name().c_str();
        result = 0;
    }
    return result;
}

FactoryPrx::FactoryPrx()
: ::Ice::Object()
{
    pd = new PD();
}

FactoryPrx::~FactoryPrx()
{
    delete pd;
}

DigitizerPrx * FactoryPrx::digitizer()
{
    DigitizerPrx * d = new DigitizerPrx( this );
    return d;
}

std::string FactoryPrx::status()
{
    IceUtil::Mutex::Lock lock( pd->mutex );
    return pd->status;
}

void FactoryPrx::setStatus( const std::string & stri )
{
    IceUtil::Mutex::Lock lock( pd->mutex );
    pd->status = stri;
}

::IceUtil::Mutex & FactoryPrx::mutex()
{
    return pd->mutex;
}

::Device::DigitizerPrx FactoryPrx::digitizerPrx()
{
    return pd->digitizer;
}





