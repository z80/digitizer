
#include "timer_ctrl.h"
#include "hal.h"

static void gptCb( GPTDriver * gptp );

static const GPTConfig gptCfg =
{
  9000000,   // 100kHz timer clock.
  gptCb      // Timer callback.
};


void initTimer( void )
{
    gptStartContinuous( &GPTD1, 90 );
}

static void gptCb(GPTDriver *gptp)
{

  (void)gptp;
  palClearPad(IOPORT3, GPIOC_LED);
  chSysLockFromIsr();
  chSysUnlockFromIsr();
}



