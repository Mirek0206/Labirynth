#include "Sensors/ultrasonicSensor.h"
#include <stdint.h>
#include <Arduino.h>

/*===============================*/
/* Local functions declarations  */
/*===============================*/

static float measureDistance( const uint8_t trigPin_u8, const uint8_t echoPin_u8 );

/*===============================*/
/* Global data definition        */
/*===============================*/

ultrasonicSensorData_t ultrasonicSensorData;

/*===============================*/
/* Functions definitions         */
/*===============================*/

void ultrasonicSensorInit( void )
{
    pinMode( LEFT_SENSOR_ECHO,  INPUT );
    pinMode( RIGHT_SENSOR_ECHO, INPUT );
    pinMode( FRONT_SENSOR_ECHO, INPUT );
    pinMode( LEFT_SENSOR_TRIG,  OUTPUT );
    pinMode( RIGHT_SENSOR_TRIG, OUTPUT );
    pinMode( FRONT_SENSOR_TRIG, OUTPUT );
};

void ultrasonicSensorCycle( void )
{
    ultrasonicSensorData.left_f32 = measureDistance( LEFT_SENSOR_TRIG, LEFT_SENSOR_ECHO );
    ultrasonicSensorData.right_f32 = measureDistance( RIGHT_SENSOR_TRIG, RIGHT_SENSOR_ECHO );
    ultrasonicSensorData.front_f32 = measureDistance( FRONT_SENSOR_TRIG, FRONT_SENSOR_ECHO );
};

static float measureDistance( const uint8_t trigPin_u8, const uint8_t echoPin_u8 )
{
  digitalWrite( trigPin_u8, LOW );        //ustawienie stanu wysokiego na 2 uS - impuls inicjalizujacy
  delayMicroseconds( 2U );
  digitalWrite( trigPin_u8, HIGH );       //ustawienie stanu wysokiego na 10 uS - impuls inicjalizujacy
  delayMicroseconds( 10U );
  digitalWrite( trigPin_u8, LOW );
  digitalWrite( echoPin_u8, HIGH );

  return ( pulseIn( echoPin_u8, HIGH ) / 58.0F );   
};