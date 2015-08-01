
#include "adc_ctrl.h"
#include "ch.h"
#include "hal.h"

#include "hdw_config.h"
#include "led_ctrl.h"

#define ADC_QUEUE_SZ   (320)
#define SWEEP_QUEUE_SZ (320)

#define ADC_MUX_0    0
#define ADC_MUX_1    1

InputQueue  adc_queue;
uint8_t     adc_queue_buffer[ADC_QUEUE_SZ];

InputQueue  sweep_queue;
uint8_t     sweep_queue_buffer[SWEEP_QUEUE_SZ];

OutputQueue sweepCmdQueue;
uint8_t     sweepCmdBuffer[2];

uint8_t     adc_rx_buffer[4];
static int  adcIndex = 0;

static int instantAdcData[4] = { -1, -1, -1, -1 };

// Oscilloscope parameters.
int signalMask = 15;
int period     = 500;
int elapsed    = 0;


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
	chIQInit( &adc_queue,     adc_queue_buffer,   ADC_QUEUE_SZ,   0 );
	chIQInit( &sweep_queue,   sweep_queue_buffer, SWEEP_QUEUE_SZ, 0 );
	chOQInit( &sweepCmdQueue, sweepCmdBuffer,     2,              0 );

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
		int prevIndex = adcIndex;
		adcIndex = (adcIndex + 1) % 4;
		selectAdcIndex( adcIndex );
		// Unselect SPI slave.
		spiUnselectI( &SPID1 );

		int value = (int)(adc_rx_buffer[0]) +
					((int)(adc_rx_buffer[1]) << 8) +
					((int)(adc_rx_buffer[1]) << 16);
    		instantAdcData[prevIndex] = value;

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
					if ( signalMask & 1 )
					{
						value = instantAdcData[0];
						v = (uint8_t)( value & 0xFF );
						chIQPutI( &adc_queue, v );

						v = (uint8_t)( (value >> 8) & 0xFF );
						chIQPutI( &adc_queue, v );

						v = (uint8_t)( (value >> 16) & 0xFF );
						chIQPutI( &adc_queue, v );
					}

					if ( signalMask & 2 )
					{
						value = instantAdcData[1];
						v = (uint8_t)( value & 0xFF );
						chIQPutI( &adc_queue, v );

						v = (uint8_t)( (value >> 8) & 0xFF );
						chIQPutI( &adc_queue, v );

						v = (uint8_t)( (value >> 16) & 0xFF );
						chIQPutI( &adc_queue, v );
					}

					if ( signalMask & 4 )
					{
						value = instantAdcData[2];
						v = (uint8_t)( value & 0xFF );
						chIQPutI( &adc_queue, v );

						v = (uint8_t)( (value >> 8) & 0xFF );
						chIQPutI( &adc_queue, v );

						v = (uint8_t)( (value >> 16) & 0xFF );
						chIQPutI( &adc_queue, v );
					}

					if ( signalMask & 8 )
					{
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


















void instantAdc( int * vals )
{
	if ( vals )
	{
		chSysLock();
			vals[0] = instantAdcData[0];
			vals[1] = instantAdcData[1];
			vals[2] = instantAdcData[2];
			vals[3] = instantAdcData[3];
		chSysUnlock();
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

InputQueue * sweepData( void )
{
	return &sweep_queue;
}

static uint8_t  sweepSigs      = 15;
static uint32_t sweepOscPeriod = 8000;

void setSweepOsc( uint8_t sigs, uint32_t period )
{
	sweepSigs   = sigs;
	sweepPeriod = period;
}

static uint16_t sweepDacsFrom[4];
static uint16_t sweepDacsTo[4];
static uint32_t sweepPeriod = 8000000;

void setSweep( uint16_t * from, uint16_t * to, uint32_t period )
{
	sweepDacsFrom[0] = from[0];
	sweepDacsFrom[1] = from[1];
	sweepDacsFrom[2] = from[2];
	sweepDacsFrom[3] = from[3];

	sweepDacsTo[0] = to[0];
	sweepDacsTo[1] = to[1];
	sweepDacsTo[2] = to[2];
	sweepDacsTo[3] = to[3];

	sweepPeriod = period;

	chOQPut( &sweepCmdQueue, 1 );
}

















