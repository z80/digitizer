
#include "timer_ctrl.h"
#include "adc_ctrl.h"
#include "dac_ctrl.h"
#include "hal.h"

static void gptCb( GPTDriver * gptp );


static const GPTConfig gptCfg =
{
  1000000,   // 1MHz timer clock.
  gptCb      // Timer callback.
};









void initTimer( void )
{
    gptStart( &GPTD1, &gptCfg );
    gptStartContinuous( &GPTD1, 5000 );
}

static void gptCb(GPTDriver *gptp)
{
  (void)gptp;
	chSysLockFromIsr();
		queryAdcI();
		processDacI();
	chSysUnlockFromIsr();
}





























