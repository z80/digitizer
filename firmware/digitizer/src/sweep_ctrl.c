
#include "sweep_ctrl.h"
#include "hal.h"
#include "dac_ctrl.h"
#include "adc_ctrl.h"

#define SWEEP_QUEUE_SZ (320)

InputQueue  sweep_queue;
uint8_t     sweep_queue_buffer[SWEEP_QUEUE_SZ];

# define SWEEP_CMD_SZ (sizeof( int ) * 6 * 16)
OutputQueue sweepCmdQueue;
uint8_t     sweepCmdBuffer[ SWEEP_CMD_SZ ];


static int swDacFrom[4];
static int swDacTo[4];

static int swPeriod;
static int swElapsed;
static int swNextPtTime;

static int swPtsCnt;
static int swPtIndex;

static uint8_t swEnabled;
static uint8_t swEnabledNew;

static uint8_t swDacMode;

static int trigRecordPtsCnt = 0;

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
	swEnabledNew  = 0;
	swDacMode     = 0;
	swPeriod      = 8000000;
	trigRecordPtsCnt = 0;

	chIQInit( &sweep_queue,   sweep_queue_buffer, SWEEP_QUEUE_SZ, 0 );
	chOQInit( &sweepCmdQueue, sweepCmdBuffer,     SWEEP_CMD_SZ,   0 );


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

static void recordAdcI( void );
static uint8_t checkForNewTransitionI( void );

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
				recordAdcI();
				// Calc next point time.

				swPtIndex += 1;

				uint64_t t;
				t = (int64_t)(swPeriod) * (int64_t)swPtIndex / (int64_t)( swPtsCnt-1 );
				swNextPtTime = (int)t;
			}

			// Turning sweep off if it's on.
			if ( !swEnabledNew )
			{
				// Clear output queue.
				// If it was turned off just clear command queue.
				int space = chOQGetFullI( &sweepCmdQueue );
				int i;
				for ( i=0; i<space; i++ )
					chOQGetI( &sweepCmdQueue );
				// Turn sweep off.
				swEnabled = 0;
			}
		}
		// Move DACs.
		if ( swElapsed < swPeriod )
		{
			// Calc current DAC values.
			int time = (swElapsed < swPeriod) ? swElapsed : (2*swPeriod - swElapsed);
			int64_t dac64 = (int64_t)swDacFrom[dacIndex] + (int64_t)(swDacTo[dacIndex] - swDacFrom[dacIndex])*(int64_t)time / (int64_t)swPeriod;
			int dac = (int)dac64;
			setDacI( dacIndex, dac );
		}
		else
		{
			setDacI( 0, swDacTo[0] );
			setDacI( 1, swDacTo[1] );
			setDacI( 2, swDacTo[2] );
			setDacI( 3, swDacTo[3] );

			swEnabled = checkForNewTransitionI();
		}
	}
	else
	{
		if ( trigRecordPtsCnt > 0 )
		{
			recordAdcI();
			trigRecordPtsCnt--;
		}
		if ( swEnabledNew )
		{
			// Check if there is next transition.
			swEnabled = checkForNewTransitionI();
		}
	}
}

static uint8_t checkForNewTransitionI( void )
{
	int space = chOQGetFullI( &sweepCmdQueue );
	if ( space >= 16 )
	{
		// Current DAC values.
		currentDacsI( swDacFrom );

		uint8_t v;
		v = chOQGetI( &sweepCmdQueue );
		swPtsCnt = ((int)(v) << 24);

		v = chOQGetI( &sweepCmdQueue );
		swPtsCnt += ((int)(v) << 16);

		v = chOQGetI( &sweepCmdQueue );
		swPtsCnt += ((int)(v) << 8);

		v = chOQGetI( &sweepCmdQueue );
		swPtsCnt += (int)(v);


		v = chOQGetI( &sweepCmdQueue );
		swPeriod = ((int)(v) << 24);

		v = chOQGetI( &sweepCmdQueue );
		swPeriod += ((int)(v) << 16);

		v = chOQGetI( &sweepCmdQueue );
		swPeriod += ((int)(v) << 8);

		v = chOQGetI( &sweepCmdQueue );
		swPeriod += (int)(v);

		uint8_t i;
		for ( i=0; i<4; i++ )
		{
			v = chOQGetI( &sweepCmdQueue );
			swDacTo[i] = ((int)(v) << 8);

			v = chOQGetI( &sweepCmdQueue );
			swDacTo[i] += (int)(v);
		}

		// Initial counter values.
		swElapsed    = 0;
		swPtIndex    = 0;
		swNextPtTime = 0;

		return 1;
	}

	swEnabledNew = 0;
	return 0;
}

void setSweepEn( uint8_t en )
{
	chSysLock();
		swEnabledNew = en;
	chSysUnlock();
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

void setSweepDacMode( uint8_t en )
{
	chSysLock();
		swDacMode = en;
	chSysUnlock();
}

uint8_t sweepDacMode( void )
{
	chSysLock();
		uint8_t res = swDacMode;
	chSysUnlock();
	return res;
}

uint8_t sweepPush( int ptsCnt, int period, int * dacTo )
{
	uint8_t v;
	chSysLock();
		int space = chOQGetEmptyI( &sweepCmdQueue );
	chSysUnlock();
	if ( space >= 16 )
	{
		// ptsCnt
		v = (uint8_t)(( ptsCnt >> 24 ) & 0xFF);
		chOQPut( &sweepCmdQueue, v );

		v = (uint8_t)(( ptsCnt >> 16 ) & 0xFF);
		chOQPut( &sweepCmdQueue, v );

		v = (uint8_t)(( ptsCnt >> 8 ) & 0xFF);
		chOQPut( &sweepCmdQueue, v );

		v = (uint8_t)(( ptsCnt ) & 0xFF);
		chOQPut( &sweepCmdQueue, v );

		// period
		v = (uint8_t)(( period >> 24 ) & 0xFF);
		chOQPut( &sweepCmdQueue, v );

		v = (uint8_t)(( period >> 16 ) & 0xFF);
		chOQPut( &sweepCmdQueue, v );

		v = (uint8_t)(( period >> 8 ) & 0xFF);
		chOQPut( &sweepCmdQueue, v );

		v = (uint8_t)(( period ) & 0xFF);
		chOQPut( &sweepCmdQueue, v );


		// dacTo
		uint8_t i;
		for ( i=0; i<4; i++ )
		{
			v = (uint8_t)(( dacTo[i] >> 8 ) & 0xFF);
			chOQPut( &sweepCmdQueue, v );

			v = (uint8_t)(( dacTo[i] ) & 0xFF);
			chOQPut( &sweepCmdQueue, v );
		}
	}
	else
		return 1;
	return 0;
}

















static void recordAdcI( void )
{
	if ( chIQGetEmptyI( &sweep_queue ) >= 12 )
	{
		int adc[4];
		instantAdcI( adc );

		if ( swDacMode )
		{
			int dacs[4];
			currentDacsI( dacs );
			// Replace obtained ADC values with rough DAC values.
			adc[0] = dacs[1];
			adc[1] = dacs[3];
		}

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
  //chSysLockFromIsr();
  	// Just indicate that point is supposed to be recorded.
    trigRecordPtsCnt++;
    // And it will be recorded in normal timer ISR.
  //chSysUnlockFromIsr();
}




