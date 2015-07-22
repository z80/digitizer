
#ifndef __ADC_CTRL_H_
#define __ADC_CTRL_H_

#include "ch.h"

void initAdc( void );
void queryAdcI( void );
InputQueue * adcQueue( void );
// Possible indices 0, 1, 2, 3.
void selectAdc( int index );

#endif



