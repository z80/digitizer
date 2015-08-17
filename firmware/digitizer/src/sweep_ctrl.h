
#ifndef __SWEEP_CTRL_H_
#define __SWEEP_CTRL_H_

#include "ch.h"

void initSweep( void );
void setSweepRange( int * dacTo0 );
void setSweepTime( int ptsCnt, int period );
void processSweepI( void ); // It should be called in DAC handler.
void setSweepEn( uint8_t en ); // Enables automatic sweep.
uint8_t sweepEn( void );
void setTrigSweepEn( uint8_t en ); // Sweep by external trigger. This sweep should only switch voltage without real sweeping.
InputQueue * sweepQueue( void ); // Sweep data.

#endif




