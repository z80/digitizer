
#ifndef __ADC_CTRL_H_
#define __ADC_CTRL_H_

#include "ch.h"

void initAdc( void );
void queryAdcI( void );
InputQueue * adcQueue( void );
// Returns 4 values. So pointer should reference at least 4 integers array.
void instantAdc( int * vals );
void instantAdcI( int * vals );

void setOscSignals( int mask );
void setOscPeriod( uint32_t interval );

// Program simple sweep.
InputQueue * sweepData( void );
void setSweepOsc( uint8_t sigs, uint32_t period );
void setSweep( uint16_t * from, uint16_t * to, uint32_t period );



#endif



