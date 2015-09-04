
#include "digitizer_ice.h"
#include "main_wnd.h"


HumidityIce::HumidityIce()
: ::Device::Humidity()
{
    m_hum = new CHumidityCtrl();
}

HumidityIce::~HumidityIce()
{
    if ( m_hum )
        delete m_hum;
}

::std::string HumidityIce::status( const ::Ice::Current& c )
{
    return "ok";
}

void HumidityIce::release(const ::Ice::Current& c )
{
}


/*
int HumidityIce::enumDevices( const ::Ice::Current& c )
{
    IceUtil::Mutex::Lock lock( m_mutex );
    int cnt = m_hum->enumDevices();
    return cnt;
}
*/




