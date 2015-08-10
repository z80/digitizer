
#include "ch.h"
#include "hal.h"

#include "iwdg.h"
#include "led_ctrl.h"
#include "dac_ctrl.h"
#include "adc_ctrl.h"
#include "timer_ctrl.h"
#include "cpu_io.h"

int main(void)
{
    halInit();
    chSysInit();

    initAdc();
    initDac();
    initLed();
    initCpuIo();
    initTimer();

    while ( 1 )
    {
    	processCpuIo();

    	/*
    	setLeds( 1 );
    	chThdSleepMilliseconds( 1000 );
    	setLeds( 2 );
    	chThdSleepMilliseconds( 1000 );
    	*/
    }
    return 0;
}
