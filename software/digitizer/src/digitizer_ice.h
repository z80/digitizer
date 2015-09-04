
#ifndef __DIGITIZER_ICE_H_
#define __DIGITIZER_ICE_H_

#include <Ice/Ice.h>
#include "digitizer.h"
class MainWnd;

class DigitizerIce: public Device::Digitizer
{
public:
    DigitizerIce( MainWnd * mw );
    ~DigitizerIce();
    
    ::std::string status( const ::Ice::Current& = ::Ice::Current() );
    void release(const ::Ice::Current& = ::Ice::Current() );

    bool instantValues( ::Ice::Double&, ::Ice::Double&, ::Ice::Double&, ::Ice::Double&, const ::Ice::Current& = ::Ice::Current() );
    bool setTrigEn( bool, const ::Ice::Current& = ::Ice::Current() );
    bool values( ::Factory::TDoubleArray&, ::Factory::TDoubleArray&, ::Factory::TDoubleArray&, ::Factory::TDoubleArray&, const ::Ice::Current& = ::Ice::Current() );

private:
    MainWnd * m_mainWnd;
    IceUtil::Mutex  m_mutex;
};

#endif



