
#ifndef __HDW_CONFIG_H_
#define __HDW_CONFIG_H_

#define HARDWARE_VERSION "digitizer-1.0.0"
//#define FIRMWARE_VERSION "1.0.0"
//#define FIRMWARE_VERSION "1.1.0"  // Added DACs readout.
//#define FIRMWARE_VERSION "1.2.0"  // Changed sweep procedure. Now it is controlled via OutputQueue. And many transitions are possible.
//#define FIRMWARE_VERSION "1.2.1"  // Moved real ADC recording by trigger to timer ISR.
#define FIRMWARE_VERSION "1.2.2"  // Shortened all data transfers to only meaningful bits.

//SHELL WORKING AREA
#define SHELL_WA_SIZE 2048


// LEDs
#define LED_0_PORT   GPIOC
#define LED_0_PIN    14
#define LED_1_PORT   GPIOC
#define LED_1_PIN    13

#define CP2102_OUT_BUF_SZ 640

#endif


