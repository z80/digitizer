
#include "adc_ctrl.h"
#include "ch.h"
#include "hal.h"

#include "hdw_config.h"
#include "led_ctrl.h"

#define ADC_QUEUE_SZ   (320)

#define ADC_MUX_0    0
#define ADC_MUX_1    1

InputQueue  adc_queue;
uint8_t     adc_queue_buffer[ADC_QUEUE_SZ];

uint8_t     adc_rx_buffer[4];
static int  adcIndex = 0;

static int instantAdcData[4] = { -1, -1, -1, -1 };

// Oscilloscope parameters.
int signalMask = 15;
int period     = 40;
int elapsed    = 0;


// Possible indices 0, 1, 2, 3.
static void selectAdcIndex( int index );
static void onSpiComplete( SPIDriver * spid );
static void filterAdc( uint8_t index, int value );
/*
 * Maximum speed SPI configuration (18MHz, CPHA=0, CPOL=0, MSb first).
 */
static const SPIConfig hs_spicfg =
{
    onSpiComplete,
    GPIOA,
    GPIOA_SPI1NSS,
    SPI_CR1_BR_1 + SPI_CR1_CPOL // SPI_CR1_BR_2 + SPI_CR1_BR_0 + SPI_CR1_CPOL
};


void initAdc( void )
{
	//palSetPadMode( GPIOB, 8, PAL_MODE_OUTPUT_PUSHPULL );

	chIQInit( &adc_queue,     adc_queue_buffer,   ADC_QUEUE_SZ,   0 );

    palSetPadMode( GPIOA, ADC_MUX_0, PAL_MODE_OUTPUT_PUSHPULL ); 		// MUX_0
    palSetPadMode( GPIOA, ADC_MUX_1, PAL_MODE_OUTPUT_PUSHPULL );        // MUX_1

    palSetPadMode( GPIOA, 5, PAL_MODE_STM32_ALTERNATE_PUSHPULL );     	// SCK
    palSetPadMode( GPIOA, 6, PAL_MODE_STM32_ALTERNATE_PUSHPULL );     	// MISO
    palSetPadMode( GPIOA, GPIOA_SPI1NSS, PAL_MODE_OUTPUT_PUSHPULL );
    palSetPad( GPIOA, GPIOA_SPI1NSS );

    selectAdcIndex( adcIndex );

    spiStart( &SPID1, &hs_spicfg );
}

void queryAdcI( void )
{
	spiSelectI( &SPID1 );
	spiStartReceiveI( &SPID1, 3, adc_rx_buffer );

	//adcIndex = (adcIndex + 1) % 4;
	//selectAdcIndex( adcIndex );
}

void onSpiComplete( SPIDriver * spid )
{
	(void)spid;
											//palSetPad( GPIOB, 8 );
	chSysLockFromIsr();
		// The very first thing - switch another signal to ADC input.
		int prevIndex = adcIndex;
		adcIndex = (adcIndex + 1) % 4;
		selectAdcIndex( adcIndex );
		// Unselect SPI slave.
		spiUnselectI( &SPID1 );

		int value = (int)(adc_rx_buffer[2]) +
					((int)(adc_rx_buffer[1]) << 8) +
					((int)(adc_rx_buffer[0]) << 16);
		value = (value >> 1) & 0xFFFF;

		filterAdc( prevIndex, value );
    	//instantAdcData[prevIndex] = value;

		// if (adcIndex == 0) this means it was 3 just
		// in time of measure. So a full cycle was just completed.
		// And now it's time to process oscilloscope. Put
		// data to input queue.
    	if ( prevIndex == 3 )
    	{
			elapsed += 1;
			if ( elapsed >= period )
			{
				elapsed -= period;
				if ( chIQGetEmptyI( &adc_queue ) >= 12 )
				{
					// Before I don't really know which bits to get I get all bits obtained.
					// In datasheet it is written that minimum 22 cycles are required and 24
					// are shown as an example.
					// Save ADC signal index first.
					uint8_t v;

					value = instantAdcData[0];
					v = (uint8_t)( value & 0xFF );
					chIQPutI( &adc_queue, v );

					v = (uint8_t)( (value >> 8) & 0xFF );
					chIQPutI( &adc_queue, v );

					v = (uint8_t)( (value >> 16) & 0xFF );
					chIQPutI( &adc_queue, v );



					value = instantAdcData[1];
					v = (uint8_t)( value & 0xFF );
					chIQPutI( &adc_queue, v );

					v = (uint8_t)( (value >> 8) & 0xFF );
					chIQPutI( &adc_queue, v );

					v = (uint8_t)( (value >> 16) & 0xFF );
					chIQPutI( &adc_queue, v );



					value = instantAdcData[2];
					v = (uint8_t)( value & 0xFF );
					chIQPutI( &adc_queue, v );

					v = (uint8_t)( (value >> 8) & 0xFF );
					chIQPutI( &adc_queue, v );

					v = (uint8_t)( (value >> 16) & 0xFF );
					chIQPutI( &adc_queue, v );



					value = instantAdcData[3];
					v = (uint8_t)( value & 0xFF );
					chIQPutI( &adc_queue, v );

					v = (uint8_t)( (value >> 8) & 0xFF );
					chIQPutI( &adc_queue, v );

					v = (uint8_t)( (value >> 16) & 0xFF );
					chIQPutI( &adc_queue, v );
				}
			}
    	}
	chSysUnlockFromIsr();
												//palClearPad( GPIOB, 8 );
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


















void instantAdc( int * vals )
{
	chSysLock();
		instantAdcI( vals );
	chSysUnlock();
}

void instantAdcI( int * vals )
{
	if ( vals )
	{
		vals[0] = instantAdcData[0];
		vals[1] = instantAdcData[1];
		vals[2] = instantAdcData[2];
		vals[3] = instantAdcData[3];
	}
}

void setOscSignals( int mask )
{
	chSysLock();
		signalMask = mask;
	chSysUnlock();
}

void setOscPeriod( uint32_t interval )
{
	chSysLock();
		period = (int)interval;
	chSysUnlock();
}




int adcRaw[4][5];
int adcFiltered[4][5];
uint8_t adcPointer[4] = { 0, 0, 0, 0 };
static void filterAdc( uint8_t index, int value )
{
	int * raw = adcRaw[ index ];
	uint8_t * ptr = &adcPointer[ index ];
	raw[ *ptr ] = value;
	*ptr = ( (*ptr) + 1 ) % 5;

	int * filtered = adcFiltered[ index ];
	uint8_t i, j;
	for ( i=0; i<4; i++ )
		filtered[i] = raw[i];
	for ( i=0; i<4; i++ )
	{
		for ( j=i+1; j<5; j++ )
		{
			if ( filtered[i] < filtered[j] )
			{
				int a = filtered[i];
				filtered[i] = filtered[j];
				filtered[j] = a;
			}
		}
	}
	instantAdcData[index] = filtered[2];
}


















