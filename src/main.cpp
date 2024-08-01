#include <Arduino.h>
#include <stdint.h>

#include <DRV8835MotorShield.h>
#include "Sensors/accelerometer.h"
#include "Sensors/ultrasonicSensor.h"
#include "Sensors/bmm150.h"
#include "Motors/motors.h"
#include "Utils/cycle.h"

void setup() 
{
  Serial.begin(9600);

  //accelerometerInit();
  ultrasonicSensorInit();
  motorsInit();
}

void loop() 
{
  cycle();
  
  // Main functions
  //accelerometerCycle();
  ultrasonicSensorCycle();
  motorsCycle();
}