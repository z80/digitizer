/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"

#define LED_0_PORT   GPIOC
#define LED_0_PIN    14
#define LED_1_PORT   GPIOC
#define LED_1_PIN    13

static void onSpiComplete( SPIDriver * spid );

/*
 * Maximum speed SPI configuration (18MHz, CPHA=0, CPOL=0, MSb first).
 */
static const SPIConfig hs_spicfg = {
    onSpiComplete,
    GPIOA,
    GPIOA_SPI1NSS,
    0
};

static uint8_t txbuf[5];
static uint8_t rxbuf[5];




/*
 * Application entry point.
 */
int main(void) {
  halInit();
  chSysInit();

  palSetPadMode(IOPORT1, 5, PAL_MODE_STM32_ALTERNATE_PUSHPULL);     /* SCK. */
  palSetPadMode(IOPORT1, 6, PAL_MODE_STM32_ALTERNATE_PUSHPULL);     /* MISO.*/
  palSetPadMode(IOPORT1, 7, PAL_MODE_STM32_ALTERNATE_PUSHPULL);     /* MOSI.*/
  palSetPadMode(IOPORT1, GPIOA_SPI1NSS, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPad(IOPORT1, GPIOA_SPI1NSS);

  palSetPadMode( LED_0_PORT, LED_0_PIN, PAL_MODE_OUTPUT_PUSHPULL );
  palSetPadMode( LED_1_PORT, LED_1_PIN, PAL_MODE_OUTPUT_PUSHPULL );
  palClearPad( LED_0_PORT, LED_0_PIN );
  palClearPad( LED_1_PORT, LED_1_PIN );

  spiStart( &SPID1, &hs_spicfg );       /* Setup transfer parameters.       */

  while (TRUE)
  {
    palSetPad( LED_0_PORT, LED_0_PIN );

    //spiAcquireBus( &SPID1 );              /* Acquire ownership of the bus.    */
    spiSelect( &SPID1 );                  /* Slave Select assertion.          */
    spiStartReceive( &SPID1, 3, rxbuf );          /* Atomic transfer operations.      */
    //spiUnselect( &SPID1 );                /* Slave Select de-assertion.       */
    //spiReleaseBus( &SPID1 );              /* Ownership release.               */

    //palClearPad( LED_0_PORT, LED_0_PIN );

    chThdSleepMilliseconds( 10 );
  }
  return 0;
}




static void onSpiComplete( SPIDriver * spid )
{
	(void)spid;
	chSysLockFromIsr();
		spiUnselectI( &SPID1 );
	chSysUnlockFromIsr();

	palClearPad( LED_0_PORT, LED_0_PIN );
}














