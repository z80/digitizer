
#include "led_ctrl.h"
#include "dfu_ctrl.h"
#include "hal.h"

#include "iwdg.h"
#include "hdw_config.h"

static Mutex    mutex;
static uint32_t value = 0;

static const int DURATION_MS = 200;


static WORKING_AREA( waLeds, 256 );
static msg_t ledsThread( void *arg )
{
    (void)arg;
    chRegSetThreadName( "ld" );
    while ( 1 )
    {
        static uint32_t arg;
    	chMtxLock( &mutex );
            arg = value;
            if ( arg & 1 )
                palTogglePad( LED_0_PORT, LED_0_PIN );
            else
            	palClearPad( LED_0_PORT, LED_0_PIN );

            if ( arg & 2 )
                palTogglePad( LED_1_PORT, LED_1_PIN );
            else
            	palClearPad( LED_1_PORT, LED_1_PIN );
        chMtxUnlock();
        chThdSleepMilliseconds( DURATION_MS );
    	processDfu( DURATION_MS );
    }

    return 0;
}

void initLed( void )
{
	palClearPad( LED_0_PORT,   LED_0_PIN );
	palClearPad( LED_1_PORT,   LED_1_PIN );
	palSetPadMode( LED_0_PORT, LED_0_PIN, PAL_MODE_OUTPUT_PUSHPULL );
	palSetPadMode( LED_1_PORT, LED_1_PIN, PAL_MODE_OUTPUT_PUSHPULL );

	// Initializing mutex.
	chMtxInit( &mutex );
	// Creating thread.
	chThdCreateStatic( waLeds, sizeof(waLeds), NORMALPRIO, ledsThread, NULL );
}

void setLeds( uint32_t arg )
{
    chMtxLock( &mutex );
        value = arg;
    chMtxUnlock();
}

void toggleLeds( uint32_t arg )
{
    chMtxLock( &mutex );
        value = ( value & ( ~arg ) ) |
                ( (value ^ arg ) & (arg & 0x07) );
    chMtxUnlock();
}

void toggleLedsImmediate( uint32_t val )
{
    if ( val & 1 )
        palTogglePad( LED_0_PORT, LED_0_PIN );
    if ( val & 2 )
        palTogglePad( LED_1_PORT, LED_1_PIN );
}

void toggleLedsI( uint32_t arg )
{
    if ( arg & 1 )
        palTogglePad( LED_0_PORT, LED_0_PIN );
    if ( arg & 2 )
        palTogglePad( LED_1_PORT, LED_1_PIN );
}









