
#include "timer_ctrl.h"
#include "adc_ctrl.h"
#include "dac_ctrl.h"
#include "hal.h"

static void gptCb( GPTDriver * gptp );


static const GPTConfig gptCfg =
{
  9000000,   // 9MHz timer clock.
  gptCb      // Timer callback.
};


void initTimer( void )
{
    gptStart( &GPTD1, &gptCfg );
    gptStartContinuous( &GPTD1, 90 );
}

static void gptCb(GPTDriver *gptp)
{
  (void)gptp;
  queryAdcI();
  processDacI();
}











