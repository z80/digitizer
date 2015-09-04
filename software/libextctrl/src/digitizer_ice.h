
#ifndef __BIPOT_ICE_H_
#define __BIPOT_ICE_H_

#include <IceE/IceE.h>
class MainWnd;

class DigitizerIce: public ::Device::Digitizer
{
public:
    HumidityIce();
    ~HumidityIce();
    
    ::std::string status( const ::Ice::Current& = ::Ice::Current() );
    void release(const ::Ice::Current& = ::Ice::Current() );

private:
    MainWnd * m_mainWnd;
    IceUtil::Mutex  m_mutex;
};

#endif



