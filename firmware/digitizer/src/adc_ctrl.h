
#ifndef __ADC_CTRL_H_
#define __ADC_CTRL_H_

#include "ch.h"

// Modes of operation.
#define TDAC      0
#define TONEPULSE 1
#define TMEANDR   2
#define TSWEEP    3
#define TFEEDBACK 4

void initAdc( void );
void queryAdcI( void );
InputQueue * adcQueue( void );

#endif



