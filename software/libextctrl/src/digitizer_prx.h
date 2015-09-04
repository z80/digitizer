
#ifndef __DIGITIZER_PRX_H_
#define __DIGITIZER_PRX_H_

#include <string>
#include <QtCore>

class FactoryPrx;

class DigitizerPrx
{
public:
    ~DigitizerPrx();

    bool instantValues( qreal & workV, qreal & workI, qreal & probeV, qreal & probeI );
    bool setTrigEn( bool en );
    bool values( std::vector<qreal> & workV, std::vector<qreal> & workI, std::vector<qreal> & probeV, std::vector<qreal> & probeI );

private:
    DigitizerPrx( FactoryPrx * owner );

    class PD;
    PD * pd;

    friend class FactoryPrx;
};



#endif





