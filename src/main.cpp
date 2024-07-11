#include <Arduino.h>
#include <stdint.h>

#include <DRV8835MotorShield.h>
#include "Sensors/accelerometer.h"
#include "Sensors/ultrasonicSensor.h"
#include "Motors/motors.h"

unsigned long cycleTime_u64;

void setup() 
{
  cycleTime_u64 = millis();
  Serial.begin(9600);

  accelerometerInit();
  ultrasonicSensorInit();
  motorsInit();
}

void loop() 
{
  cycleTime_u64 = millis() - cycleTime_u64;

  // Main functions
  accelerometerCycle();
  ultrasonicSensorCycle();
  motorsCycle();

  Serial.print( "Left: " );
  Serial.print( ultrasonicSensorData.left_f32 );
  Serial.print( " cm | Front: ");
  Serial.print( ultrasonicSensorData.front_f32 );
  Serial.print( " cm | Right: " );
  Serial.print( ultrasonicSensorData.right_f32 );
  Serial.print( " cm | " );
}