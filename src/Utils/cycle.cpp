#include <Arduino.h>
#include "Utils/cycle.h"

/*===============================*/
/* Local functions declarations  */
/*===============================*/

/*===============================*/
/* Global data definition        */
/*===============================*/

uint64_t cycle_u64;             /* Number of processing cycle */
uint64_t previousCycleTime_u64; /* Time from preevious cycle */
uint8_t cycleDuration_u8;       /* Cycle length */

/*===============================*/
/* Functions definitions         */
/*===============================*/

void cycle( void )
{   
    /* Calcuulate cycle duration */
    cycleDuration_u8 = millis() - previousCycleTime_u64;
    previousCycleTime_u64 = millis();
}

void increaseCycle( void )
{
    /* Increase cycle number */ 
    cycle_u64++;
}

void getNumOfCycle( uint64_t * const numOfCycle_u64 )
{
    *numOfCycle_u64 = cycle_u64;
}

uint8_t getCycleTime( void )
{
    return cycleDuration_u8;
}
