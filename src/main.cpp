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
    static uint8_t mainCycleTime_u8;

    cycle();
    mainCycleTime_u8 += getCycleTime();

    if (mainCycleTime_u8 >= TASK_5MS)
    {
      increaseCycle();
      ultrasonicSensorCycle();
      motorsCycle();

      mainCycleTime_u8 = 0U;
    }
}