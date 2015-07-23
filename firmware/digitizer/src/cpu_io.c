
#include "cpu_io.h"

#include "ch.h"
#include "hal.h"

#include "led_ctrl.h"
#include "adc_ctrl.h"
#include "dac_ctrl.h"

#include "hdw_config.h"
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


void initCpuIo( void )
{
	// Initialize serial driver.
	sdStart( &SERIAL, 0 );

	// Setup pad settings.
	palSetPadMode( GPIOA, 9, PAL_MODE_STM32_ALTERNATE_PUSHPULL );
	palSetPadMode( GPIOA, 10, PAL_MODE_INPUT );
}

void processCpuIo( void )
{
	static uint8_t slash = 0;
	static int out_index = 0;

	// Try reading serial.
	/*
	msg_t msg;
	msg = sdGetTimeout( &SERIAL, TIME_INFINITE );
	sdPut( &SERIAL, msg );
	return;
	*/


	msg_t msg;
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
static void set_dac1( uint8_t * args );
static void set_dac2( uint8_t * args );
static void get_adc( uint8_t * args );

static TFunc funcs[] =
{
	hardware_version,
	firmware_version,
    set_led,
	set_dac1,
	set_dac2,
	get_adc
};

static void exec_func( void )
{
	int func_index = (int)buffer[1];// + ((int)buffer[2]) * 256;
	// Just to avoid troubles.
	int funcs_sz = (int)(sizeof(funcs)/sizeof(TFunc));
    func_index = (func_index < funcs_sz) ? func_index : 0;
	funcs[func_index]( args );
}

static void hardware_version( uint8_t * args )
{
	(void)args;
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
	const uint8_t stri[] = FIRMWARE_VERSION;
	int l = strlen( (char *)stri );
	int i;
	for ( i=0; i<l; i++ )
		writeResult( stri[i] );
	writeEom();
}

static void set_led( uint8_t * args )
{
	setLeds( args[0] );
}


static void set_dac1( uint8_t * args )
{
	int valL = (int)args[0] + ((int)args[1] << 8);
	int valH = (int)args[2] + ((int)args[3] << 8);
	setDac1( valL, valH );
}

static void set_dac2( uint8_t * args )
{
	int valL = (int)args[0] + ((int)args[1] << 8);
	int valH = (int)args[2] + ((int)args[3] << 8);
	setDac2( valL, valH );
}

static void get_adc( uint8_t * args )
{
	(void)args;
	int res = 123;
	uint8_t b = (uint8_t)(res & 0xFF);
	writeResult( b );
	b = (uint8_t)((res >> 8) & 0xFF);
	writeResult( b );
	writeEom();
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






