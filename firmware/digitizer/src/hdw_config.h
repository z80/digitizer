
#ifndef __HDW_CONFIG_H_
#define __HDW_CONFIG_H_

#define HARDWARE_VERSION "digitizer-1.0.0"
//#define FIRMWARE_VERSION "1.0.0"
//#define FIRMWARE_VERSION "1.1.0"  // Added DACs readout.
#define FIRMWARE_VERSION "1.2.0"  // Changed sweep procedure. Now it is controlled via OutputQueue. And many transitions are possible.

//SHELL WORKING AREA
#define SHELL_WA_SIZE 2048


// LEDs
#define LED_0_PORT   GPIOC
#define LED_0_PIN    14
#define LED_1_PORT   GPIOC
#define LED_1_PIN    13


#endif


