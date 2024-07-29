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

  accelerometerInit();
  ultrasonicSensorInit();
  motorsInit();
}

void loop() 
{
  cycleTime_u64 = millis() - previousCycleTime_u64;
  previousCycleTime_u64 = millis();

  // Main functions
  accelerometerCycle();
  ultrasonicSensorCycle();
  motorsCycle();
}