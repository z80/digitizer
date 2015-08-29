
#include "cpu_io.h"

#include "ch.h"
#include "hal.h"

#include "led_ctrl.h"
#include "dfu_ctrl.h"

#include "hdw_config.h"
#include "funcs.h"
#include <string.h>

#define BUFFER_SZ    32
#define ARGS_SZ      32
#define OUT_QUEUE_SZ 32

#define IO_DELAY_MS 1
#define SERIAL 		SD1

#define CMD_SET_ARGS  1
#define CMD_EXEC_FUNC 2

static uint8_t buffer[ BUFFER_SZ ];
static uint8_t args[ ARGS_SZ ];


static void process_command( uint8_t * buf, int sz );
static void writeResult( uint8_t v );
static void writeEom( void );

static const SerialConfig serial_cfg =
{
	115200,
	0,
	USART_CR2_STOP1_BITS | USART_CR2_LINEN,
	USART_CR3_RTSE | USART_CR3_CTSE

};

void initCpuIo( void )
{
	// Setup pad settings.
	palSetPadMode( GPIOA, 9, PAL_MODE_STM32_ALTERNATE_PUSHPULL );
	palSetPadMode( GPIOA, 10, PAL_MODE_INPUT );
	// Setup RTS.
	palSetPadMode( GPIOA, 11, PAL_MODE_STM32_ALTERNATE_PUSHPULL );
	// Setup CTS.
	palSetPadMode( GPIOA, 12, PAL_MODE_INPUT );

	// Initialize serial driver.
	//sdStart( &SERIAL, &serial_cfg );
	sdStart( &SERIAL, 0 );
}

void processCpuIo( void )
{
	static uint8_t slash = 0;
	static int out_index = 0;
	msg_t msg;

	/*
	// Try reading serial.
	msg = sdGetTimeout( &SERIAL, TIME_INFINITE );
	sdPut( &SERIAL, msg );
	return;
	*/


	msg = sdGetTimeout( &SERIAL, TIME_INFINITE );
	uint8_t noData = ( ( msg == Q_TIMEOUT ) || ( msg == Q_RESET ) ) ? 1 : 0;
	if ( !noData )
	{
		uint8_t v = (uint8_t)msg;
		//shift = serial_decode_byte( msg, &(buffer[out_index]), &eom );
		if ( !slash )
		{
			if ( v == '\0' )
			{
				// Execute command
				process_command( buffer, out_index );
				out_index = 0;
			}
			else if ( v != '\\' )
				buffer[ out_index++ ] = v;
			else
				slash = 1;
		}
		else
		{
			slash = 0;
			buffer[ out_index++ ] = v;
		}
		// Just in case of crazy command
		out_index = ( out_index < BUFFER_SZ ) ? out_index : BUFFER_SZ;
	}
}

uint8_t * funcArgs( void )
{
	return args;
}

static void exec_func( void );

static void process_command( uint8_t * buf, int sz )
{
	int i;
	uint8_t cmd;
	cmd = buf[0];
	uint8_t * data = &(buf[1]);
	switch ( cmd )
	{
	case CMD_SET_ARGS:
		for ( i=0; i<(sz-1); i++ )
			args[i] = data[i];
		break;
	case CMD_EXEC_FUNC:
		// Execute function by it's index.
		exec_func();
		break;
	}
}

static void hardware_version( uint8_t * args );
static void firmware_version( uint8_t * args );
static void set_led( uint8_t * args );

static void dfu_push( uint8_t * args );
static void dfu_write_sector( uint8_t * args );
static void dfu_run_firmware( uint8_t * args );

static TFunc funcs[] =
{
	hardware_version,
	firmware_version,
    set_led,

    dfu_push,
    dfu_write_sector,
    dfu_run_firmware
};

static void exec_func( void )
{
	int func_index = (int)buffer[1] - 128; // 128 shift to prevent overlap with main firmware (!!!)
	// Just to avoid troubles.
	int funcs_sz = (int)(sizeof(funcs)/sizeof(TFunc));
    if ( (func_index >= funcs_sz) || ( func_index < 0 ) )
    	return; // Don't even try to execute wrong functions.
	funcs[func_index]( args );
}

static void hardware_version( uint8_t * args )
{
	(void)args;

	turnCountdownOff();

	const uint8_t stri[] = HARDWARE_VERSION;
	int l = strlen( (char *)stri );
	int i;
	for ( i=0; i<l; i++ )
		writeResult( stri[i] );
	writeEom();
}

static void firmware_version( uint8_t * args )
{
	(void)args;

	turnCountdownOff();

	const uint8_t stri[] = FIRMWARE_VERSION;
	int l = strlen( (char *)stri );
	int i;
	for ( i=0; i<l; i++ )
		writeResult( stri[i] );
	writeEom();
}

static void set_led( uint8_t * args )
{
	turnCountdownOff();

	setLeds( args[0] );
}

static void dfu_push( uint8_t * args )
{
	turnCountdownOff();

	dfuPushBytes( args[0], &(args[1]) );
}

static void dfu_write_sector( uint8_t * args )
{
	int index = ( ((int)args[0]) << 8 ) + ( (int)args[1] );
	uint8_t res = dfuWriteSector( index );

	writeResult( res );
	writeEom();
}

static void dfu_run_firmware( uint8_t * args )
{
	(void)args;
	dfuStartFirmware();

}
























static void writeResult( uint8_t v )
{
	if ( ( v == '\0' ) || ( v == '\\') )
		sdPut( &SERIAL, '\\' );
	sdPut( &SERIAL, v );
}

static void writeEom( void )
{
	sdPut( &SERIAL, '\0' );
}







