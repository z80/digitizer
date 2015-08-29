
#include "cpu_io.h"

#include "ch.h"
#include "hal.h"

#include "led_ctrl.h"
#include "adc_ctrl.h"
#include "dac_ctrl.h"
#include "temp_ctrl.h"
#include "sweep_ctrl.h"
#include "output_ctrl.h"
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
static void set_dac1( uint8_t * args );
static void set_dac2( uint8_t * args );
static void get_adc( uint8_t * args );
static void set_osc_signals( uint8_t * args );
static void set_osc_period( uint8_t * args );
static void get_osc_data( uint8_t * args );

static void get_temp( uint8_t * args );

static void set_trigger_en( uint8_t * args );

static void set_sweep_range( uint8_t * args );
static void set_sweep_time( uint8_t * args );
static void set_sweep_en( uint8_t * args );
static void get_sweep_en( uint8_t * args );
static void get_sweep_data( uint8_t * args );

static void set_output( uint8_t * args );

static void firmware_upgrade( uint8_t * args );

static TFunc funcs[] =
{
	hardware_version,
	firmware_version,
    set_led,
	set_dac1,
	set_dac2,
	get_adc,
	set_osc_signals,
	set_osc_period,
	get_osc_data,

	get_temp,

	set_trigger_en,

	set_sweep_range,
	set_sweep_time,
	set_sweep_en,
	get_sweep_en,
	get_sweep_data,

	set_output,

	firmware_upgrade
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
	int res[4];
	instantAdc( res );
	int i;
	for ( i=0; i<4; i++ )
	{
		uint8_t b = (uint8_t)(res[i] & 0xFF);
		writeResult( b );
		b = (uint8_t)((res[i] >> 8) & 0xFF);
		writeResult( b );
		b = (uint8_t)((res[i] >> 16) & 0xFF);
		writeResult( b );
		b = (uint8_t)((res[i] >> 24) & 0xFF);
		writeResult( b );
	}
	writeEom();
}

static void set_osc_signals( uint8_t * arg )
{
	uint8_t v = arg[0];
	setOscSignals( v );
}

static void set_osc_period( uint8_t * arg )
{
	uint32_t v;
	v = (int)( arg[0] );
	v += ( (int)( arg[1] ) << 8 );
	v += ( (int)( arg[1] ) << 16 );
	v += ( (int)( arg[1] ) << 24 );
	setOscPeriod( v );
}

static void get_osc_data( uint8_t * arg )
{
	(void)arg;
	InputQueue * q = adcQueue();
	chSysLock();
		size_t cnt;
		cnt = (chQSpaceI( q ) / 12);
	chSysUnlock();
	size_t recInd;
	for ( recInd=0; recInd<cnt; recInd++ )
	{
		size_t sigInd;
		for ( sigInd=0; sigInd<4; sigInd++ )
		{
			size_t byteInd;
			for ( byteInd = 0; byteInd<3; byteInd++ )
			{
				uint8_t v;
				msg_t msg;

				msg = chIQGet( q );
				v = (uint8_t)msg;
				writeResult( v );
			}
		}
	}
	writeEom();
}

static void get_temp( uint8_t * args )
{
	(void)args;
	uint16_t t = lastTemp();

	uint8_t v = (uint8_t)(t>>8);
	writeResult( v );

	v = (uint8_t)(t & 0xFF);
	writeResult( v );

	writeEom();
}

static void set_trigger_en( uint8_t * args )
{
	setTrigEn( args[0] );
}

static void set_sweep_range( uint8_t * args )
{
	int dacs[4];
	uint8_t i;
	for ( i=0; i<4; i++ )
	{
		dacs[i] = ( (int)args[ 2*i ] << 8 ) + (int)args[2*i+1];
	}
	setSweepRange( dacs );
}

static void set_sweep_time( uint8_t * args )
{
	int ptsCnt = ( (int)args[0] << 24 ) + ( (int)args[1] << 16 ) + ( (int)args[2] << 8 ) + ( (int)args[3] );
	int period = ( (int)args[4] << 24 ) + ( (int)args[5] << 16 ) + ( (int)args[6] << 8 ) + ( (int)args[7] );
	setSweepTime( ptsCnt, period );
}

static void set_sweep_en( uint8_t * args )
{
	setSweepEn( args[0] );
}

static void get_sweep_en( uint8_t * args )
{
	(void)args;
	uint8_t v = sweepEn();
	writeResult( v );
	writeEom();
}

static void get_sweep_data( uint8_t * args )
{
	(void)args;
	InputQueue * q = sweepQueue();
	chSysLock();
		size_t cnt;
		cnt = (chQSpaceI( q ) / 12);
	chSysUnlock();
	size_t recInd;
	for ( recInd=0; recInd<cnt; recInd++ )
	{
		size_t sigInd;
		for ( sigInd=0; sigInd<4; sigInd++ )
		{
			size_t byteInd;
			for ( byteInd = 0; byteInd<3; byteInd++ )
			{
				uint8_t v;
				msg_t msg;

				msg = chIQGet( q );
				v = (uint8_t)msg;
				writeResult( v );
			}
		}
	}
	writeEom();

}

static void set_output( uint8_t * args )
{
	setOutput( args[0] );
}

static void firmware_upgrade( uint8_t * args )
{
	(void)args;

	// Just report to let application know that function was executed successfully.
	uint8_t res[] = "ok";
	writeResult( res[0] );
	writeResult( res[1] );
	writeEom();

	// To give some time to retrieve the result.
	chThdSleepMilliseconds( 500 );

	// Jump to the beginning of FLASH space to invoke bootloader.
	firmwareUpgrade();
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







