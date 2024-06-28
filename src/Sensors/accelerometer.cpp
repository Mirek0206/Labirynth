/*===============================*/
/* Includes                      */
/*===============================*/

#include <Arduino.h>
#include "Sensors/accelerometer.h"

/*===============================*/
/* Local functions declarations  */
/*===============================*/

/*===============================*/
/* Global data definition        */
/*===============================*/

accelerometerData_t accelerometerData;

/*===============================*/
/* Functions definitions         */
/*===============================*/

void accelerometerInit( void )
{

};

void accelerometerCycle( void )
{
    accelerometerData.x_f32 = ( ( float )analogRead( ACC_X_PIN ) - ACC_OFFSET_X ) * ACC_SCALE; // Read and scale x axis
    accelerometerData.y_f32 = ( ( float )analogRead( ACC_Y_PIN ) - ACC_OFFSET_Y ) * ACC_SCALE; // Read and scale y axis
    accelerometerData.z_f32 = ( ( float )analogRead( ACC_Z_PIN ) - ACC_OFFSET_Z ) * ACC_SCALE; // Read and scale z axis
    accelerometerData.x_f32 *= G_TO_MSS; // Convert x value to [m/s*s]
    accelerometerData.y_f32 *= G_TO_MSS; // Convert y value to [m/s*s]
    accelerometerData.z_f32 *= G_TO_MSS; // Convert z value to [m/s*s]
}