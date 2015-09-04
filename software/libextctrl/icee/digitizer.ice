
#ifndef __HUMIDITY_ICE_
#define __HUMIDITY_ICE_

#include "factory.ice"

module Device
{
    interface Digitier extends Factory::Device
    {
        bool instantValues( var double workV, var double workI, var double probeV, var double probeI );
        bool setTrigEn( bool en );
        bool values( var TDoubleArray workV, var TDoubleArray workI, var TDoubleArray probeV, var double probeI );
    };
};


#endif





