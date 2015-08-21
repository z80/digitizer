
#include "ch.h"
#include "hal.h"

#include "iwdg.h"
#include "led_ctrl.h"
#include "dac_ctrl.h"
#include "adc_ctrl.h"
#include "timer_ctrl.h"
#include "temp_ctrl.h"
#include "sweep_ctrl.h"
#include "output_ctrl.h"
#include "cpu_io.h"

int main(void)
{
    halInit();
    chSysInit();

    initAdc();
    initDac();
    initSweep();
    initTimer();
    initCpuIo();
    initTemp();
    initLed();
    initOutput();
    setLeds( 3 );

    while ( 1 )
    {
    	processCpuIo();

    	/*
    	setLeds( 1 );
    	chThdSleepMilliseconds( 1000 );
    	setLeds( 2 );
    	chThdSleepMilliseconds( 1000 );
    	*/

    	/*
        setDac( 0, 32767 );
        chThdSleepMilliseconds( 50 );
        setDac( 1, 32767 );
        chThdSleepMilliseconds( 50 );
        setDac( 2, 32767 );
        chThdSleepMilliseconds( 50 );
        setDac( 3, 32767 );
        chThdSleepMilliseconds( 50 );


        setDac( 0, 65530 );
        chThdSleepMilliseconds( 50 );
        setDac( 1, 65530 );
        chThdSleepMilliseconds( 50 );
        setDac( 2, 65530 );
        chThdSleepMilliseconds( 50 );
        setDac( 3, 65530 );
        chThdSleepMilliseconds( 50 );
        */
    }
    return 0;
}
