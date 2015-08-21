
#include "timer_ctrl.h"
#include "adc_ctrl.h"
#include "dac_ctrl.h"
#include "hal.h"

static void gptCb( GPTDriver * gptp );


static const GPTConfig gptCfg =
{
  1600000,   // 8MHz timer clock.
  gptCb      // Timer callback.
};









void initTimer( void )
{
	//palSetPadMode( GPIOC, 15, PAL_MODE_OUTPUT_PUSHPULL );


    gptStart( &GPTD1, &gptCfg );
    //gptStartContinuous( &GPTD1, 50 );
    gptStartContinuous( &GPTD1, 100 );
}

static void gptCb(GPTDriver *gptp)
{
	(void)gptp;
								//palSetPad( GPIOC, 15 );
	chSysLockFromIsr();
		queryAdcI();
		processDacI();
	chSysUnlockFromIsr();
								//palClearPad( GPIOC, 15 );
}





























