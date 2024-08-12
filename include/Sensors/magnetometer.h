#ifndef MAGNETOMETER_H
#define MAGNETOMETER_H

/*===============================*/
/* Includes                      */
/*===============================*/

#include <bmm150.h>

/*===============================*/
/* Const data                    */
/*===============================*/

#define HISTORY_SIZE 50U
#define OFFSET_X 45
#define OFFSET_Y -10
#define OFFSET_Z -40

/*===============================*/
/* Types definitions             */
/*===============================*/

/*===============================*/
/* Global data                   */
/*===============================*/

/*===============================*/
/* Global functions declarations */
/*===============================*/

void magnetometerInit( void );

void getAzimuth( float * const retValue );

#endif