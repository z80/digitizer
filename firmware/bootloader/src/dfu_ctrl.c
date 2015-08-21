
#include "dfu_ctrl.h"
#include "hal.h"

#include "stm32f10x_flash.h"

#define CONF_START_PAGE  20
#define CONF_PAGE_SIZE   1024
#define CONF_FLASH_START 0x08000000

typedef  void (*pFunction)(void);

static void startFirmwareI( void * arg );

static uint8_t sector[ CONF_PAGE_SIZE ];
static int     byteIndex = 0;
static int     timeLeft = 3000;
static bool_t  dontStartFirmware = 0;


void initDfu( int msecs )
{
	dontStartFirmware = 0;
	byteIndex = 0;
	timeLeft = msecs;
}

void processDfu( int msecsElapsed )
{
	timeLeft -= msecsElapsed;
	if ( timeLeft <= 0 )
	{
		if ( !dontStartFirmware )
			dfuStartFirmware();
	}
}

void dfuPushBytes( uint8_t cnt, uint8_t * bytes )
{
	uint8_t i;
	for ( i=0; i<cnt; i++ )
	{
		sector[ byteIndex++ ] = bytes[i];
		byteIndex %= CONF_PAGE_SIZE;
	}
}

uint8_t dfuWriteSector( int index )
{
	// Zero bytes counter. It is necessary for the next sector.
	byteIndex = 0;

    uint32_t * flash = ( uint32_t * )( CONF_FLASH_START + (CONF_START_PAGE + index) * CONF_PAGE_SIZE );

    // Comparing byte-by-byte.
    int i;
    uint8_t theSame = 1;
    uint32_t * datad = (uint32_t *)sector;
    int cnt = ( CONF_PAGE_SIZE/sizeof( uint32_t) );
    for ( i=0; i<cnt; i++ )
    {
        if ( flash[i] != datad[i] )
        {
            theSame = 0;
            break;
        }
    }
    if ( theSame )
    	return 0;

    // If different write data.
    FLASH_Unlock();
    uint32_t flashD = CONF_FLASH_START + (CONF_START_PAGE + index) * CONF_PAGE_SIZE;
    FLASH_Status st = FLASH_ErasePage( flashD );
    if ( st != FLASH_COMPLETE )
    {
        FLASH_Lock();
        return 1;
    }
    for ( i=0; i<cnt; i++ )
    {
        st = FLASH_ProgramWord( flashD+4*i, datad[i] );
        if ( st != FLASH_COMPLETE )
        {
            FLASH_Lock();
            return 2;
        }
    }
    FLASH_Lock();

    return 0;
}

void dfuStartFirmware( void )
{
	startFirmwareI( 0 );
}











static void startFirmwareI( void * arg )
{
	(void)arg;
	// Invoke DFU only if there seems to be a valid firmware.
	// And it is determined using the very first word of firmware at that address.
	const uint32_t addr = CONF_FLASH_START + CONF_START_PAGE * CONF_PAGE_SIZE;
	if ( ((*(__IO uint32_t*)addr) & 0x2FFE0000 ) == 0x20000000 )
	{
	    uint32_t JumpAddress;
	    pFunction Jump_To_Application;

	    JumpAddress = *(__IO uint32_t*) (addr + 4);
	    Jump_To_Application = (pFunction) JumpAddress;
	    // Initialize user application's Stack Pointer
	    __set_MSP(*(__IO uint32_t*) addr);
	    Jump_To_Application();
	}
}

void turnCountdownOff( void )
{
	dontStartFirmware = 1;
}





