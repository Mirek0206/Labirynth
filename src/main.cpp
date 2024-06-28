#include <Arduino.h>
#include <stdint.h>

#include <DRV8835MotorShield.h>
#include "Sensors/accelerometer.h"
#include "Sensors/ultrasonicSensor.h"

#define MOTOR_SHIELD_MODE   13U
#define MOTOR_SHIELD_M1_DIR 12U
#define MOTOR_SHIELD_M1_PWM 11U
#define MOTOR_SHIELD_M2_DIR 10U
#define MOTOR_SHIELD_M2_PWM 9U

#define MOTOR_DEFAULT_SPEED 200U

DRV8835MotorShield motors( MOTOR_SHIELD_M1_DIR, 
                           MOTOR_SHIELD_M1_PWM, 
                           MOTOR_SHIELD_M2_DIR, 
                           MOTOR_SHIELD_M2_PWM );

void driveForward( void );

void setup() 
{
  Serial.begin(9600);

  ultrasonicSensorInit();

  /* motor shield */
  pinMode( MOTOR_SHIELD_MODE, OUTPUT );
  digitalWrite( MOTOR_SHIELD_MODE, HIGH );
}

void loop() 
{
  accelerometerCycle();
  ultrasonicSensorCycle();


  driveForward();


  Serial.print( "Left: " );
  Serial.print( ultrasonicSensorData.left_f32 );
  Serial.print( " cm | Front: ");
  Serial.print( ultrasonicSensorData.front_f32 );
  Serial.print( " cm | Right: " );
  Serial.print( ultrasonicSensorData.right_f32 );
  Serial.println( " cm" );
}

void driveForward( void )
{
  static float yAxisAcc_f32 = 0.0F;
  int16_t leftMotorSpeed_s16 = MOTOR_DEFAULT_SPEED;
  int16_t rightMotorSpeed_s16 = MOTOR_DEFAULT_SPEED;

  yAxisAcc_f32 = ( yAxisAcc_f32 * 0.75F ) + (accelerometerData.y_f32 * 0.25); 

  if ( yAxisAcc_f32 > -0.48F )
  {
    leftMotorSpeed_s16 = MOTOR_DEFAULT_SPEED + 15;
  }
  else if ( yAxisAcc_f32 < -0.57F )
  {
    rightMotorSpeed_s16 = MOTOR_DEFAULT_SPEED + 15;
  }

  Serial.print(yAxisAcc_f32);
  Serial.print(" | Left: ");
  Serial.print( leftMotorSpeed_s16 );
  Serial.print(" | Right: ");
  Serial.print( rightMotorSpeed_s16 );

  motors.setM1Speed( rightMotorSpeed_s16 ); // prawy
  motors.setM2Speed( leftMotorSpeed_s16 ); // lewy
}