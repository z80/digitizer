
#include "adc_ctrl.h"
#include "ch.h"
#include "hal.h"

#include "hdw_config.h"
#include "led_ctrl.h"

#define ADC_QUEUE_SZ (16)

InputQueue  adc_queue;
uint8_t     adc_queue_buffer[ADC_QUEUE_SZ];

uint8_t     adc_rx_buffer[2];

void onSpiComplete( SPIDriver * spid );

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
	spiStartReceiveI( SPID1, 2, adc_rx_buffer );
}

void onSpiComplete( SPIDriver * spid )
{
	chSysLockFromIsr();
		spiUnselectI( SPID1 );
		if ( chIQGetEmptyI( &adc_queue ) >= 2 )
		{
			chIQPutI( &adc_queue, adc_rx_buffer[0] );
			chIQPutI( &adc_queue, adc_rx_buffer[1] );
		}
	chSysUnlockFromIsr();
}

InputQueue * adcQueue( void )
{
	return &adc_queue;
}















