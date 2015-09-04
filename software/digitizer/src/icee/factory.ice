
#ifndef __FACTORY_ICE_
#define __FACTORY_ICE_

module Factory
{
    sequence<string> TStringArray;
    sequence<double> TDoubleArray;

    interface DeviceAbs
    {
        string status();
        void   release();
    };

    interface DeviceFactory
    {
        idempotent TStringArray interfaces();
        DeviceAbs * query( string interfaceName );
    };
};



#endif



