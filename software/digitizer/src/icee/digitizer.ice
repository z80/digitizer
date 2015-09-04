
#ifndef __DIGITIZER_ICE_
#define __DIGITIZER_ICE_

#include "factory.ice"

module Device
{
    interface Digitizer extends Factory::DeviceAbs
    {
        bool instantValues( out double workV, out double workI, out double probeV, out double probeI );
        bool setTrigEn( bool en );
        bool values( out Factory::TDoubleArray workV, out Factory::TDoubleArray workI, out Factory::TDoubleArray probeV, out Factory::TDoubleArray probeI );
    };
};


#endif





