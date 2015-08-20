
#ifndef __SWEEP_CTRL_H_
#define __SWEEP_CTRL_H_

#include "ch.h"

void initSweep( void );
void setSweepRange( int * dacTo0 );
void setSweepTime( int ptsCnt, int period );
void processSweepI( uint8_t dacIndex ); // It should be called in DAC handler.
void setSweepEn( uint8_t en ); // Enables automatic sweep.
uint8_t sweepEn( void );
void setTrigEn( uint8_t en ); // Record ADC by external trigger. This should only read ADC.
InputQueue * sweepQueue( void ); // Sweep data.

#endif




