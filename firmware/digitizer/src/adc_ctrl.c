
#include "adc_ctrl.h"
#include "ch.h"
#include "hal.h"

#include "hdw_config.h"
#include "led_ctrl.h"

#define ADC_QUEUE_SZ (32)
#define ADC_MUX_0    0
#define ADC_MUX_1    1

InputQueue  adc_queue;
uint8_t     adc_queue_buffer[ADC_QUEUE_SZ];

uint8_t     adc_rx_buffer[4];
static int adcIndex = 0;

// Possible indices 0, 1, 2, 3.
static void selectAdcIndex( int index );

static void onSpiComplete( SPIDriver * spid );

/*
 * Maximum speed SPI configuration (18MHz, CPHA=0, CPOL=0, MSb first).
 */
static const SPIConfig hs_spicfg =
{
    onSpiComplete,
    GPIOA,
    GPIOA_SPI1NSS,
    0
};


void initAdc( void )
{
	chIQInit( &adc_queue, adc_queue_buffer, ADC_QUEUE_SZ, 0 );

    palSetPadMode( GPIOA, 0, PAL_MODE_OUTPUT_PUSHPULL );              /* SCK. */
    palSetPadMode( GPIOA, 1, PAL_MODE_OUTPUT_PUSHPULL );              /* MISO.*/

	palSetPadMode( GPIOA, 5, PAL_MODE_STM32_ALTERNATE_PUSHPULL );     /* SCK. */
    palSetPadMode( GPIOA, 6, PAL_MODE_STM32_ALTERNATE_PUSHPULL );     /* MISO.*/
    palSetPadMode( GPIOA, 7, PAL_MODE_STM32_ALTERNATE_PUSHPULL );     /* MOSI.*/
    palSetPadMode( GPIOA, GPIOA_SPI1NSS, PAL_MODE_OUTPUT_PUSHPULL );
    palSetPad( GPIOA, GPIOA_SPI1NSS );

    selectAdcIndex( adcIndex );

    spiStart( &SPID1, &hs_spicfg );
}

void queryAdcI( void )
{
	chSysLockFromIsr();
		spiSelectI( &SPID1 );
		spiStartReceiveI( &SPID1, 3, adc_rx_buffer );
	chSysUnlockFromIsr();
}

void onSpiComplete( SPIDriver * spid )
{
	(void)spid;
	chSysLockFromIsr();
		// The very first thing - switch another signal to ADC input.
		adcIndex = (adcIndex + 1) % 4;
		selectAdcIndex( adcIndex );
		// Unselect SPI slave.
		spiUnselectI( &SPID1 );
		// Put data to input queue.
		if ( chIQGetEmptyI( &adc_queue ) >= 4 )
		{
			// Before I don't really know which bits to get I get all bits obtained.
			// In datasheet it is written that minimum 22 cycles are required and 24
			// are shown as an example.
			// Save ADC signal index first.
			chIQPutI( &adc_queue, (uint8_t)adcIndex );
			// Save data here.
			chIQPutI( &adc_queue, adc_rx_buffer[0] );
			chIQPutI( &adc_queue, adc_rx_buffer[1] );
			chIQPutI( &adc_queue, adc_rx_buffer[2] );
		}
	chSysUnlockFromIsr();
}

InputQueue * adcQueue( void )
{
	return &adc_queue;
}

void selectAdcIndex( int index )
{
	if ( index & 1 )
		palSetPad( GPIOA, ADC_MUX_0 );
	else
		palClearPad( GPIOA, ADC_MUX_0 );

	if ( index & 2 )
		palSetPad( GPIOA, ADC_MUX_1 );
	else
		palClearPad( GPIOA, ADC_MUX_1 );
}















