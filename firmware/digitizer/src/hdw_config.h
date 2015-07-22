
#ifndef __HDW_CONFIG_H_
#define __HDW_CONFIG_H_

#define HARDWARE_VERSION "voltamp-1.0.0"
#define FIRMWARE_VERSION "0.0.1"

//SHELL WORKING AREA
#define SHELL_WA_SIZE 2048


// LEDs
#define LED_0_PORT   GPIOC
#define LED_0_PIN    5
#define LED_1_PORT   GPIOB
#define LED_1_PIN    0
#define LED_2_PORT   GPIOB
#define LED_2_PIN    1

// ADC inputs
#define PORT_ADC	GPIOC
#define PIN_EAUX	10
#define PIN_EREF	11
#define PIN_IAUX	12

// Relays.
#define OUT_RELAY_PORT  GPIOB
#define OUT_RELAY_PIN   9

#define SC_RELAY_PORT   GPIOB
#define SC_RELAY_PIN    8

#endif


