#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

/*===============================*/
/* Includes                      */
/*===============================*/


/*===============================*/
/* Pins connfiguration           */
/*===============================*/

#define ACC_X_PIN A2
#define ACC_Y_PIN A1
#define ACC_Z_PIN A0

#define ACC_OFFSET_X 344
#define ACC_OFFSET_Y 344
#define ACC_OFFSET_Z 281

#define ACC_SCALE 0.0033F; // Współczynnik przeliczeniowy w g/LSB
#define G_TO_MSS 9.81F;    // Przyspieszenie ziemskie w m/s^2

/*===============================*/
/* Types definitions             */
/*===============================*/

typedef struct accelerometerData_tag
{
    float x_f32;
    float y_f32;
    float z_f32;
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