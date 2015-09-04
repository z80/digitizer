
#include "factory_ice.h"
#include "thread_ice.h"
#include "digitizer_ice.h"

FactoryIce::FactoryIce( ThreadIce * parent, MainWnd * mw  )
: ::Factory::DeviceFactory()
{
    m_digit    = new DigitizerIce( mw );
    m_digitPrx = ::Factory::DeviceAbsPrx::uncheckedCast( parent->adapter()->addWithUUID( m_digit ) );
}

FactoryIce::FactoryIce()
{
}

::Factory::TStringArray FactoryIce::interfaces(const ::Ice::Current & c )
{
    ::Factory::TStringArray types;
    types.push_back( "digitizer" );
    return types;
}

::Factory::DeviceAbsPrx FactoryIce::query(const ::std::string & interfaceName, const ::Ice::Current & c )
{
    if ( interfaceName == "digitizer" )
        return m_digitPrx;
    return 0;
}


