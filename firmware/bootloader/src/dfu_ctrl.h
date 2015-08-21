
#ifndef __DFU_CTRL_H_
#define __DFU_CTRL_H_

#include "ch.h"

void initDfu( int msecs );
void processDfu( int msecsElapsed );
void dfuPushBytes( uint8_t cnt, uint8_t * bytes );
uint8_t dfuWriteSector( int index );
void dfuStartFirmware( void );

void turnCountdownOff( void );


#endif





