
#include "sweep_ctrl.h"
#include "hal.h"

#define SWEEP_QUEUE_SZ (320)

InputQueue  sweep_queue;
uint8_t     sweep_queue_buffer[SWEEP_QUEUE_SZ];

OutputQueue sweepCmdQueue;
uint8_t     sweepCmdBuffer[2];


static int swDacFrom[4];
static int swDacTo[4];

static int swPeriod;
static int swElapsed;

static int swPtsCnt;
static int swPtIndex;

static uint8_t swEnabled;
static uint8_t swTrigEnabled;

void initSweep( void )
{
	swEnabled     = 0;
	swTrigEnabled = 0;

	chIQInit( &sweep_queue,   sweep_queue_buffer, SWEEP_QUEUE_SZ, 0 );
	chOQInit( &sweepCmdQueue, sweepCmdBuffer,     2,              0 );


	// Initialize external interrupt input here.
}

void setSweepRange0( int * dacTo0 )
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

void processSweepI( void )
{

}

void setSweepEn( uint8_t en )
{
	swDacFrom[0] = 0;
	swDacFrom[1] = 0;
	swDacFrom[2] = 0;
	swDacFrom[3] = 0;

	chOQPut( &sweepCmdQueue, en ? 1 : 0 );
	swEnabled = en;
}

uint8_t sweepEn( void )
{
	chSysLock();
		uint8_t en = swEnabled;
	chSysUnlock();
	return en;
}

void setTrigSweepEn( uint8_t en )
{
	swDacFrom[0] = 0;
	swDacFrom[1] = 0;
	swDacFrom[2] = 0;
	swDacFrom[3] = 0;

	swTrigEnabled = en;
	chOQPut( &sweepCmdQueue, en ? 2 : 0 );
}

InputQueue * sweepQueue( void )
{
	return &sweep_queue;
}






