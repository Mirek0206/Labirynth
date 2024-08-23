#ifndef CYCLE_H
#define CYCLE_H

#include <stdint.h>

/*===============================*/
/* Includes                      */
/*===============================*/

/*===============================*/
/* Types definitions             */
/*===============================*/

/*===============================*/
/* Global data                   */
/*===============================*/

/*===============================*/
/* Global functions declarations */
/*===============================*/

void cycle( void );

void increaseCycle( void );

void getNumOfCycle( uint64_t * const numOfCycle_u64 );

uint8_t getCycleTime( void );

#endif