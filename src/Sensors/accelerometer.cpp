/*===============================*/
/* Includes                      */
/*===============================*/

#include <Arduino.h>
#include "Sensors/accelerometer.h"
#include "Utils/calmanFilter.h"

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
    for ( uint8_t i = 0U; i < INIT_OFFSET_CYCLES; i++ )
    {
        accelerometerData.x_axis.offset_f32 = ( ( accelerometerData.x_axis.offset_f32 * (float)i ) + ( ( float )analogRead( ACC_X_PIN ) * G_TO_MSS * ACC_SCALE ) ) / (float)( i + 1 );
        accelerometerData.y_axis.offset_f32 = ( ( accelerometerData.y_axis.offset_f32 * (float)i ) + ( ( float )analogRead( ACC_Y_PIN ) * G_TO_MSS * ACC_SCALE ) ) / (float)( i + 1 );
        accelerometerData.z_axis.offset_f32 = ( ( accelerometerData.z_axis.offset_f32 * (float)i ) + ( ( float )analogRead( ACC_Z_PIN ) * G_TO_MSS * ACC_SCALE ) ) / (float)( i + 1 );
        delay(5U);
    }
    
    calmanFilterInit(&(accelerometerData.x_axis.calmanData), 0.001F, 0.1F, 0.0F);
    calmanFilterInit(&(accelerometerData.y_axis.calmanData), 0.001F, 0.1F, 0.0F);
    calmanFilterInit(&(accelerometerData.z_axis.calmanData), 0.001F, 0.1F, 0.0F);
};

void accelerometerCycle( void )
{
    float raw_axis = ( ( float )analogRead( ACC_X_PIN ) * ACC_SCALE * G_TO_MSS ) - accelerometerData.x_axis.offset_f32;
    accelerometerData.x_axis.value_f32 = calmanFilterUpdate( &(accelerometerData.x_axis.calmanData), raw_axis);

    raw_axis = ( ( float )analogRead( ACC_Y_PIN ) * ACC_SCALE * G_TO_MSS ) - accelerometerData.y_axis.offset_f32;
    accelerometerData.y_axis.value_f32 = calmanFilterUpdate( &(accelerometerData.y_axis.calmanData), raw_axis);

    raw_axis = ( ( float )analogRead( ACC_Z_PIN ) * ACC_SCALE * G_TO_MSS ) - accelerometerData.z_axis.offset_f32;
    accelerometerData.z_axis.value_f32 = calmanFilterUpdate( &(accelerometerData.z_axis.calmanData), raw_axis);
}