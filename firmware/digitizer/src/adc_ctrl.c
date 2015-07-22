
#include "adc_ctrl.h"
#include "ch.h"
#include "hal.h"

#include "hdw_config.h"
#include "led_ctrl.h"

#define ADC_QUEUE_SZ (24)
#define ADC_MUX_0    0
#define ADC_MUX_1    1

InputQueue  adc_queue;
uint8_t     adc_queue_buffer[ADC_QUEUE_SZ];

uint8_t     adc_rx_buffer[3];

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


void initAdc( void )
{
	chIQInit( &adc_queue, adc_queue_buffer, OSC_QUEUE_SZ, 0 );

    palSetPadMode( GPIOA, 0, PAL_MODE_OUTPUT_PUSHPULL );              /* SCK. */
    palSetPadMode( GPIOA, 1, PAL_MODE_OUTPUT_PUSHPULL );              /* MISO.*/

	palSetPadMode( GPIOA, 5, PAL_MODE_STM32_ALTERNATE_PUSHPULL );     /* SCK. */
    palSetPadMode( GPIOA, 6, PAL_MODE_STM32_ALTERNATE_PUSHPULL );     /* MISO.*/
    palSetPadMode( GPIOA, 7, PAL_MODE_STM32_ALTERNATE_PUSHPULL );     /* MOSI.*/
    palSetPadMode( GPIOA, GPIOA_SPI1NSS, PAL_MODE_OUTPUT_PUSHPULL );
    palSetPad( GPIOA, GPIOA_SPI1NSS );

    spiStart( &SPID1, &hs_spicfg );
}

void queryAdcI( void )
{
	spiSelectI( SPID1 );
	spiStartReceiveI( SPID1, 3, adc_rx_buffer );
}

void onSpiComplete( SPIDriver * spid )
{
	chSysLockFromIsr();
		spiUnselectI( SPID1 );
		if ( chIQGetEmptyI( &adc_queue ) >= 2 )
		{
			// Before I don't really know which bits to get I get all bits obtained.
			// In datasheet it is written that minimum 22 cycles are required and 24
			// are shown as an example.
			chIQPutI( &adc_queue, adc_rx_buffer[0] );
			chIQPutI( &adc_queue, adc_rx_buffer[1] );
			chIQPutI( &adc_queue, adc_rx_buffer[3] );
		}
	chSysUnlockFromIsr();
}

InputQueue * adcQueue( void )
{
	return &adc_queue;
}

void selectAdc( int index )
{
	if ( 1 & index )
		palSetPad( GPIOA, ADC_MUX_0 );
	else
		palClearPad( GPIOA, ADC_MUX_0 );

	if ( 2 & index )
		palSetPad( GPIOA, ADC_MUX_1 );
	else
		palClearPad( GPIOA, ADC_MUX_1 );
}















