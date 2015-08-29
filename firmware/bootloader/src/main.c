
#include "ch.h"
#include "hal.h"

#include "led_ctrl.h"
#include "cpu_io.h"
#include "dfu_ctrl.h"

int main(void)
{
    halInit();
    chSysInit();

    initCpuIo();
    initLed();
    initDfu( 3000 );

    setLeds( 3 );

    // Use virtual timer to execute regular firmware within
    // some time if nothing happens.
    while ( 1 )
    {
    	processCpuIo();
    }
    return 0;
}
