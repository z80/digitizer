
#ifndef __FACTORY_ICE_H_
#define __FACTORY_ICE_H_

#include "factory.h"
class ThreadIce;
class MainWnd;

class FactoryIce: public Factory::DeviceFactory
{
public:
    FactoryIce( ThreadIce * parent, MainWnd * mw );
    FactoryIce();

    ::Factory::TStringArray interfaces(const ::Ice::Current& = ::Ice::Current() );
    ::Factory::DeviceAbsPrx query(const ::std::string&, const ::Ice::Current& = ::Ice::Current() );
private:
    ::Factory::DeviceAbsPtr m_digit;
    ::Factory::DeviceAbsPrx m_digitPrx;
};




#endif


