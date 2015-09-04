
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

bool DigitizerIce::instantValues(::Ice::Double & workV, ::Ice::Double & workI, ::Ice::Double & probeV, ::Ice::Double & probeI, const ::Ice::Current& )
{
    bool res = m_mainWnd->iceInstantValues( workV, workI, probeV, probeI );
    return res;
}

bool DigitizerIce::setTrigEn( bool en, const ::Ice::Current& )
{
    bool res = m_mainWnd->iceSetTrigEn( en );
    return res;
}

bool DigitizerIce::values(::Factory::TDoubleArray & workV, ::Factory::TDoubleArray & workI, ::Factory::TDoubleArray & probeV, ::Factory::TDoubleArray & probeI, const ::Ice::Current& )
{
    bool res = m_mainWnd->iceValues( workV, workI, probeV, probeI );
    return res;
}


/*
int HumidityIce::enumDevices( const ::Ice::Current& c )
{
    IceUtil::Mutex::Lock lock( m_mutex );
    int cnt = m_hum->enumDevices();
    return cnt;
}
*/




