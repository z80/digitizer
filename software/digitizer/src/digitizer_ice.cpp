
#include "digitizer_ice.h"
#include "main_wnd.h"


DigitizerIce::DigitizerIce( MainWnd * mw )
: Device::Digitizer()
{
    m_mainWnd = mw;
}

DigitizerIce::~DigitizerIce()
{
}

::std::string DigitizerIce::status( const ::Ice::Current& c )
{
    return "ok";
}

void DigitizerIce::release(const ::Ice::Current& c )
{
}

bool DigitizerIce::instantValues(::Ice::Double&, ::Ice::Double&, ::Ice::Double&, ::Ice::Double&, const ::Ice::Current& )
{

    return true;
}

bool DigitizerIce::setTrigEn(bool, const ::Ice::Current& )
{
    
    return true;
}

bool DigitizerIce::values(::Factory::TDoubleArray&, ::Factory::TDoubleArray&, ::Factory::TDoubleArray&, ::Factory::TDoubleArray&, const ::Ice::Current& )
{
    
    return true;
}


/*
int HumidityIce::enumDevices( const ::Ice::Current& c )
{
    IceUtil::Mutex::Lock lock( m_mutex );
    int cnt = m_hum->enumDevices();
    return cnt;
}
*/




