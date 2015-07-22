
#include "usb_io.h"
#include "ch.h"
#include "hal.h"
#include "hdw_config.h"


static void cmd_sdiotest(BaseSequentialStream *chp, int argc, char *argv[]);
static void setDac( BaseSequentialStream *chp, int argc, char *argv[] );
static void fillDac( BaseSequentialStream *chp, int argc, char *argv[] );
static void setDacDiv( BaseSequentialStream *chp, int argc, char *argv[] );

void initUsbIo( void )
{
	/*
	static SerialConfig ser_cfg = {
	    9600,
	    0,
	    0,
	    0,
	};
	*/

    sdInit();
    //sdStart( &SD1, &ser_cfg );
    sdStart( &SD1, NULL );
    /* Setting up ports.*/
    palSetPadMode( IOPORT1, 9, PAL_MODE_STM32_ALTERNATE_PUSHPULL );
    palSetPadMode( IOPORT1, 10, PAL_MODE_INPUT );


    static const ShellCommand commands[] = {
      {"sdiotest", cmd_sdiotest},
      {NULL, NULL}
    };

    static const ShellConfig shell_cfg1 = {
      (BaseSequentialStream *)&SD1,
      commands
    };

    shellInit();
    static WORKING_AREA( waShell, SHELL_WA_SIZE );
    shellCreateStatic(&shell_cfg1, waShell, sizeof(waShell), NORMALPRIO);

}

void processUsbIo( void )
{

}




static void cmd_sdiotest(BaseSequentialStream *chp, int argc, char *argv[])
{
	(void)chp;
	(void)argc;
	(void)argv;
}

static void setDac( BaseSequentialStream *chp, int argc, char *argv[] )
{

}

static void fillDac( BaseSequentialStream *chp, int argc, char *argv[] )
{

}

static void setDacDiv( BaseSequentialStream *chp, int argc, char *argv[] )
{

}






