#ifndef ULTRASONICSENSOR_H
#define ULTRASONICSENSOR_H

/*===============================*/
/* Includes                      */
/*===============================*/

#include <stdint.h>

/*===============================*/
/* Pins connfiguration           */
/*===============================*/

#define LEFT_SENSOR_ECHO    6U
#define FRONT_SENSOR_ECHO   4U
#define RIGHT_SENSOR_ECHO   2U
#define LEFT_SENSOR_TRIG    7U
#define FRONT_SENSOR_TRIG   5U
#define RIGHT_SENSOR_TRIG   3U

/*===============================*/
/* Types definitions             */
/*===============================*/

typedef struct ultrasonicSensorData_tag
{
    float left_f32;
    float right_f32;
    float front_f32;
}ultrasonicSensorData_t;

/*===============================*/
/* Global data                   */
/*===============================*/

extern ultrasonicSensorData_t ultrasonicSensorData;

/*===============================*/
/* Global functions declarations */
/*===============================*/

void ultrasonicSensorInit( void );

void ultrasonicSensorCycle( void );

#endif