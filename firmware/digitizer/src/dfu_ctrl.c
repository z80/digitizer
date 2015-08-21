
#include "dfu_ctrl.h"
#include "hal.h"

#define DFU_ADDRESS 	0x08000000
typedef  void (*pFunction)(void);

void firmwareUpgrade( void )
{
	/*
	// Invoke DFU only if there seems to be a valid firmware.
	// And it is determined using the very first word of firmware at that address.
	if ( ((*(__IO uint32_t*)DFU_ADDRESS) & 0x2FFE0000 ) == 0x20000000 )
	{
	    uint32_t JumpAddress;
	    pFunction Jump_To_Application;
	    
	    JumpAddress = *(__IO uint32_t*) (DFU_ADDRESS + 4);
	    Jump_To_Application = (pFunction) JumpAddress;
	    // Initialize user application's Stack Pointer
	    __set_MSP(*(__IO uint32_t*) DFU_ADDRESS);
	    Jump_To_Application();
	}
	*/
	NVIC_SystemReset();
}





