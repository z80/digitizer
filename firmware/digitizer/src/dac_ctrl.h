
#ifndef __DAC_H_
#define __DAC_H_

#include "ch.h"

void initDac( void );
void setDacI( int index, int value );
int  setDac( int index, int value );
int  setDac1( int valueL, int valueH );
int  setDac2( int valueL, int valueH );
void processDacI( void );
void currentDacs( int * dacs );


#endif



