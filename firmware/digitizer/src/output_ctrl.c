
#include "output_ctrl.h"
#include "hal.h"

#define PORT_WORK GPIOC
#define PAD_WORK  15

#define PORT_PROBE GPIOB
#define PAD_PROBE  8

void initOutput( void )
{
	palClearPad( PORT_WORK,  PAD_WORK );
	palClearPad( PORT_PROBE, PAD_PROBE );
	palSetPadMode( PORT_WORK,  PAD_WORK,  PAL_MODE_OUTPUT_PUSHPULL );
	palSetPadMode( PORT_PROBE, PAD_PROBE, PAL_MODE_OUTPUT_PUSHPULL );
}

void setOutput( uint8_t o )
{
	palClearPad( PORT_WORK,  PAD_WORK );
	palClearPad( PORT_PROBE, PAD_PROBE );
	if ( o == 1 )
	{
		palSetPad( PORT_WORK,  PAD_WORK );
	}
	else if ( o == 2 )
	{
		palSetPad( PORT_PROBE,  PAD_PROBE );
	}
}

