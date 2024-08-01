#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

/*===============================*/
/* Includes                      */
/*===============================*/

#include "Utils/calmanFilter.h"

/*===============================*/
/* Pins connfiguration           */
/*===============================*/

#define ACC_X_PIN A2
#define ACC_Y_PIN A1
#define ACC_Z_PIN A0

#define ACC_SCALE 0.0033F // Współczynnik przeliczeniowy w g/LSB
#define G_TO_MSS 9.81F    // Przyspieszenie ziemskie w m/s^2

#define INIT_OFFSET_CYCLES 50U
#define ZERO_THRESHOLD 0.2F

/*===============================*/
/* Types definitions             */
/*===============================*/

typedef struct accelerometerAxisData_tag {
    float offset_f32;
    float value_f32;
    float velocity_f32;
    calmanFilter_t calmanData;
}accelerometerAxisData_t;

typedef struct accelerometerData_tag
{
    accelerometerAxisData_t x_axis;
    accelerometerAxisData_t y_axis;
    accelerometerAxisData_t z_axis;
}accelerometerData_t;

/*===============================*/
/* Global data                   */
/*===============================*/

extern accelerometerData_t accelerometerData;

/*===============================*/
/* Global functions declarations */
/*===============================*/

void accelerometerInit( void );

void accelerometerCycle( void );

#endif