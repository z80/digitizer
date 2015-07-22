
#ifndef __CPU_IO_H_
#define __CPU_IO_H_

#include "ch.h"

void cpu_io_init( void );
void cpu_io_process( void );
uint8_t * funcArgs( void );

typedef void (* TFunc)( uint8_t * args );




#endif



