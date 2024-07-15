#include <Arduino.h>
#include <stdint.h>

#include <DRV8835MotorShield.h>
#include "Sensors/accelerometer.h"
#include "Sensors/ultrasonicSensor.h"
#include "Sensors/bmm150.h"
#include "Motors/motors.h"

unsigned long cycleTime_u64;
unsigned long previousCycleTime_u64 = 0;

void setup() 
{
  cycleTime_u64 = millis();
  Serial.begin(9600);

  BMM150_init();
  accelerometerInit();
  ultrasonicSensorInit();
  motorsInit();

  job_t = FOLLOW_LEFT_WALL;
}

void loop() 
{
  float azymuth;
  cycleTime_u64 = millis() - previousCycleTime_u64;
  previousCycleTime_u64 = millis();

  // Main functions
  accelerometerCycle();
  ultrasonicSensorCycle();
  motorsCycle();

  //BMM150_readAngle(&azymuth);
  // Serial.print( ultrasonicSensorData.left_f32 );
  // Serial.print( " cm | Front: ");
  // Serial.print( ultrasonicSensorData.front_f32 );
  // Serial.print( " cm | Right: " );
  // Serial.print( ultrasonicSensorData.right_f32 );
  // Serial.print( " cm | " );
  // Serial.print( " | Cycle time: " );
  // Serial.print( cycleTime_u64 );
  // Serial.print( " | Velocity x: " );
  // Serial.print( accelerometerData.x_axis.velocity_f32 );
  // Serial.print( " | Velocity y: " );
  // Serial.print( accelerometerData.y_axis.velocity_f32 );
  // Serial.print( " | Velocity z: " );
  // Serial.print( accelerometerData.z_axis.velocity_f32 );
  // Serial.print( " | Acc x: " );
  // Serial.print( accelerometerData.z_axis.value_f32 );
  // Serial.print( " | Acc y: " );
  // Serial.print( accelerometerData.z_axis.value_f32 );
  // Serial.print( " | Acc z: " );
  // Serial.println( accelerometerData.z_axis.value_f32 );
}