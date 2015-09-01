
#include "sweep_ctrl.h"
#include "hal.h"
#include "dac_ctrl.h"
#include "adc_ctrl.h"

#define SWEEP_QUEUE_SZ (320)

InputQueue  sweep_queue;
uint8_t     sweep_queue_buffer[SWEEP_QUEUE_SZ];

OutputQueue sweepCmdQueue;
uint8_t     sweepCmdBuffer[2];


static int swDacFrom[4];
static int swDacTo[4];

static int swPeriod;
static int swElapsed;
static int swNextPtTime;

static int swPtsCnt;
static int swPtIndex;

static uint8_t swEnabled;

static void extCb( EXTDriver * extp, expchannel_t channel );

static const EXTConfig extcfg = {
  {
   {EXT_CH_MODE_DISABLED, NULL},
   {EXT_CH_MODE_RISING_EDGE /*| EXT_CH_MODE_AUTOSTART*/, extCb },
   {EXT_CH_MODE_DISABLED, NULL},
   {EXT_CH_MODE_DISABLED, NULL},
   {EXT_CH_MODE_DISABLED, NULL},
   {EXT_CH_MODE_DISABLED, NULL},
   {EXT_CH_MODE_DISABLED, NULL},
   {EXT_CH_MODE_DISABLED, NULL},
   {EXT_CH_MODE_DISABLED, NULL},
   {EXT_CH_MODE_DISABLED, NULL},
   {EXT_CH_MODE_DISABLED, NULL},
   {EXT_CH_MODE_DISABLED, NULL},
   {EXT_CH_MODE_DISABLED, NULL},
   {EXT_CH_MODE_DISABLED, NULL},
   {EXT_CH_MODE_DISABLED, NULL},
   {EXT_CH_MODE_DISABLED, NULL},
  },
  EXT_MODE_EXTI(0,
                EXT_MODE_GPIOB,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0)
};










void initSweep( void )
{
	swEnabled     = 0;

	chIQInit( &sweep_queue,   sweep_queue_buffer, SWEEP_QUEUE_SZ, 0 );
	chOQInit( &sweepCmdQueue, sweepCmdBuffer,     2,              0 );


	// Initialize external interrupt input here.
	extStart(&EXTD1, &extcfg);
}

void setSweepRange( int * dacTo0 )
{
	swDacTo[0] = dacTo0[0];
	swDacTo[1] = dacTo0[1];
	swDacTo[2] = dacTo0[2];
	swDacTo[3] = dacTo0[3];
}

void setSweepTime( int ptsCnt, int period )
{
	swPeriod = period;
	swPtsCnt = ptsCnt;
}

static void recordAdc( void );

void processSweepI( uint8_t dacIndex )
{
	if ( swEnabled )
	{
		if ( dacIndex == 0 )
		{
			swElapsed += 1;
			// Record data if it's time to do that.
			if ( swElapsed >= swNextPtTime )
			{
				// Measure signals.
				recordAdc();
				// Calc next point time.

				swPtIndex += 1;

				uint64_t t;
				t = (int64_t)(2*swPeriod) * (int64_t)swPtIndex / (int64_t)( 2*swPtsCnt-1 );
				swNextPtTime = (int)t;
			}
		}
		// Move DACs.
		//if ( swPtIndex < 2*swPtsCnt )
		if ( swElapsed < 2*swPeriod )
		{
			// Calc current DAC values.
			int time = (swElapsed < swPeriod) ? swElapsed : (2*swPeriod - swElapsed);
			int64_t dac64 = (int64_t)swDacFrom[dacIndex] + (int64_t)(swDacTo[dacIndex] - swDacFrom[dacIndex])*(int64_t)time / (int64_t)swPeriod;
			int dac = (int)dac64;
			setDacI( dacIndex, dac );
		}
		else
		{
			swEnabled = 0;
			setDacI( 0, swDacFrom[0] );
			setDacI( 1, swDacFrom[1] );
			setDacI( 2, swDacFrom[2] );
			setDacI( 3, swDacFrom[3] );
			//return 0;
		}
	}
	//return 1;
}

void setSweepEn( uint8_t en )
{
	swNextPtTime = 0;
	swElapsed    = 0;
	swPtIndex    = 0;
	currentDacs( swDacFrom );

	//chOQPut( &sweepCmdQueue, en ? 1 : 0 );
	swEnabled = en;
}

uint8_t sweepEn( void )
{
	chSysLock();
		uint8_t en = (swEnabled) ? 1 : 0;
	chSysUnlock();
	return en;
}

void setTrigEn( uint8_t en )
{
	if ( en )
		extChannelEnable( &EXTD1, 1 );
	else
		extChannelDisable( &EXTD1, 1 );
}

InputQueue * sweepQueue( void )
{
	return &sweep_queue;
}

















static void recordAdc( void )
{
	if ( chIQGetEmptyI( &sweep_queue ) >= 12 )
	{
		int adc[4];
		instantAdcI( adc );


		uint8_t v;
		int value;

		value = adc[0];
		v = (uint8_t)( value & 0xFF );
		chIQPutI( &sweep_queue, v );

		v = (uint8_t)( (value >> 8) & 0xFF );
		chIQPutI( &sweep_queue, v );

		v = (uint8_t)( (value >> 16) & 0xFF );
		chIQPutI( &sweep_queue, v );



		value = adc[1];
		v = (uint8_t)( value & 0xFF );
		chIQPutI( &sweep_queue, v );

		v = (uint8_t)( (value >> 8) & 0xFF );
		chIQPutI( &sweep_queue, v );

		v = (uint8_t)( (value >> 16) & 0xFF );
		chIQPutI( &sweep_queue, v );



		value = adc[2];
		v = (uint8_t)( value & 0xFF );
		chIQPutI( &sweep_queue, v );

		v = (uint8_t)( (value >> 8) & 0xFF );
		chIQPutI( &sweep_queue, v );

		v = (uint8_t)( (value >> 16) & 0xFF );
		chIQPutI( &sweep_queue, v );



		value = adc[3];
		v = (uint8_t)( value & 0xFF );
		chIQPutI( &sweep_queue, v );

		v = (uint8_t)( (value >> 8) & 0xFF );
		chIQPutI( &sweep_queue, v );

		v = (uint8_t)( (value >> 16) & 0xFF );
		chIQPutI( &sweep_queue, v );
	}
}


















static void extCb( EXTDriver * extp, expchannel_t channel )
{
  (void)extp;
  (void)channel;
  chSysLockFromIsr();
    // Record ADC data.
    recordAdc();
  chSysUnlockFromIsr();
}




