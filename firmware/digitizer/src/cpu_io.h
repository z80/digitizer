
#ifndef __CPU_IO_H_
#define __CPU_IO_H_

#include "ch.h"

void initCpuIo( void );
void processCpuIo( void );
uint8_t * funcArgs( void );

typedef void (* TFunc)( uint8_t * args );




#endif



