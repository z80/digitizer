
#ifndef __ADC_CTRL_H_
#define __ADC_CTRL_H_

#include "ch.h"

void initAdc( void );
void queryAdcI( void );
InputQueue * adcQueue( void );
// Returns 4 values. So pointer should reference at least 4 integers array.
void instantAdc( int * vals );

#endif



