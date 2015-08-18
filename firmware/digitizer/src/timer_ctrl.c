
#include "timer_ctrl.h"
#include "adc_ctrl.h"
#include "dac_ctrl.h"
#include "sweep_ctrl.h"
#include "hal.h"

static void gptCb( GPTDriver * gptp );


static const GPTConfig gptCfg =
{
  1000000,   // 8MHz timer clock.
  gptCb      // Timer callback.
};









void initTimer( void )
{
    gptStart( &GPTD1, &gptCfg );
    //gptStartContinuous( &GPTD1, 50 );
    gptStartContinuous( &GPTD1, 500 );
}

static void gptCb(GPTDriver *gptp)
{
  (void)gptp;
	chSysLockFromIsr();
		queryAdcI();
		processDacI();
		processSweepI();
	chSysUnlockFromIsr();
}





























