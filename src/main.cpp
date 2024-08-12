#include <Arduino.h>
#include <stdint.h>
#include <DRV8835MotorShield.h>

#include "Sensors/magnetometer.h"
#include "Sensors/ultrasonicSensor.h"
#include "Motors/motors.h"
#include "Utils/cycle.h"
#include "config.h"

void setup() 
{
  Serial.begin(9600);

  #ifdef MAGNETOMETER
    magnetometerInit();
  #endif

  #ifdef ULTRASONIC_SENSORS
    ultrasonicSensorInit();
  #endif

  #ifdef MOTORS
    motorsInit();
  #endif            
}

void loop() 
{
    cycle();
    ultrasonicSensorCycle();
    motorsCycle();
}