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
extern unsigned long cycleTime_u64;

/*===============================*/
/* Functions definitions         */
/*===============================*/

void accelerometerInit( void )
{
    for ( uint8_t i = 0U; i < INIT_OFFSET_CYCLES; i++ )
    {
        accelerometerData.x_axis.offset_f32 = ( ( accelerometerData.x_axis.offset_f32 * (float)i ) + ( float )analogRead( ACC_X_PIN ) ) / (float)( i + 1 );
        accelerometerData.y_axis.offset_f32 = ( ( accelerometerData.y_axis.offset_f32 * (float)i ) + ( float )analogRead( ACC_Y_PIN ) ) / (float)( i + 1 );
        accelerometerData.z_axis.offset_f32 = ( ( accelerometerData.z_axis.offset_f32 * (float)i ) + ( float )analogRead( ACC_Z_PIN ) ) / (float)( i + 1 );
    }
    
    // calmanFilterInit(&(accelerometerData.x_axis.calmanData), 0.001F, 0.01F, 0.0F);
    // calmanFilterInit(&(accelerometerData.y_axis.calmanData), 0.001F, 0.01F, 0.0F);
    // calmanFilterInit(&(accelerometerData.z_axis.calmanData), 0.001F, 0.01F, 0.0F);
};

void accelerometerCycle( void )
{
    //float raw_axis;
    float deltaTime_f32 = (float)cycleTime_u64 / 1000.0F;

    if (fabs(accelerometerData.x_axis.value_f32) > ZERO_THRESHOLD)
        accelerometerData.x_axis.velocity_f32 += accelerometerData.x_axis.value_f32 * deltaTime_f32;
    else
        accelerometerData.x_axis.velocity_f32 = 0.0F;

    if (fabs(accelerometerData.y_axis.value_f32) > ZERO_THRESHOLD)
        accelerometerData.y_axis.velocity_f32 += accelerometerData.y_axis.value_f32 * deltaTime_f32;
    else
        accelerometerData.y_axis.velocity_f32 = 0.0F;

    if (fabs(accelerometerData.z_axis.value_f32) > ZERO_THRESHOLD)
        accelerometerData.z_axis.velocity_f32 += accelerometerData.z_axis.value_f32 * deltaTime_f32;
    else
        accelerometerData.z_axis.velocity_f32 = 0.0F;

    accelerometerData.x_axis.value_f32 = ( ( float )analogRead( ACC_X_PIN ) - accelerometerData.x_axis.offset_f32 ) * ACC_SCALE * G_TO_MSS;
    accelerometerData.y_axis.value_f32 = ( ( float )analogRead( ACC_Y_PIN ) - accelerometerData.y_axis.offset_f32 ) * ACC_SCALE * G_TO_MSS;
    accelerometerData.z_axis.value_f32 = ( ( float )analogRead( ACC_Z_PIN ) - accelerometerData.z_axis.offset_f32 ) * ACC_SCALE * G_TO_MSS;

    // raw_axis = ( ( float )analogRead( ACC_X_PIN ) - accelerometerData.x_axis.offset_f32 ) * ACC_SCALE * G_TO_MSS;
    // accelerometerData.x_axis.value_f32 = calmanFilterUpdate( &(accelerometerData.x_axis.calmanData), raw_axis);

    // raw_axis = ( ( float )analogRead( ACC_Y_PIN ) - accelerometerData.y_axis.offset_f32 ) * ACC_SCALE * G_TO_MSS;
    // accelerometerData.y_axis.value_f32 = calmanFilterUpdate( &(accelerometerData.y_axis.calmanData), raw_axis);

    // raw_axis = ( ( float )analogRead( ACC_Z_PIN ) - accelerometerData.z_axis.offset_f32 ) * ACC_SCALE * G_TO_MSS;
    // accelerometerData.z_axis.value_f32 = calmanFilterUpdate( &(accelerometerData.z_axis.calmanData), raw_axis);
}