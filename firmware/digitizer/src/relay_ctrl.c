
#include "relay_ctrl.h"

#include "ch.h"
#include "hal.h"

#include "hdw_config.h"

void initRelay( void )
{
    palClearPad( OUT_RELAY_PORT, OUT_RELAY_PIN );
    palClearPad( SC_RELAY_PORT,  SC_RELAY_PIN );
    palSetPadMode( OUT_RELAY_PORT, OUT_RELAY_PIN, PAL_MODE_OUTPUT_PUSHPULL );
    palSetPadMode( SC_RELAY_PORT, SC_RELAY_PIN, PAL_MODE_OUTPUT_PUSHPULL );
}

void setOutRelay( uint8_t en )
{
    if ( en )
        palSetPad( OUT_RELAY_PORT, OUT_RELAY_PIN );
    else
        palClearPad( OUT_RELAY_PORT, OUT_RELAY_PIN );
}

void setScRelay( uint8_t en )
{
    if ( en )
        palSetPad( SC_RELAY_PORT, SC_RELAY_PIN );
    else
        palClearPad( SC_RELAY_PORT, SC_RELAY_PIN );
}


