
#ifndef __LUA_DIGITIZER_H_
#define __LUA_DIGITIZER_H_

#include <vector>
#include <QtCore>

extern "C"
{
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

#include "digitizer_prx.h"

class DigiEnvelope
{
public:
    DigitizerPrx * d;
    std::vector<qreal> workV;
    std::vector<qreal> workI;
    std::vector<qreal> probeV;
    std::vector<qreal> probeI;

    DigiEnvelope()
    {
        d = 0;
    }

    ~DigiEnvelope()
    {
        if ( d )
            delete d;
    }
};

void createDigitizerMeta( lua_State * L );
void applyDigitizerMeta( lua_State * L );


#endif


