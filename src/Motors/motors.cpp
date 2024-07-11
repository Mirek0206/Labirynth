/*===============================*/
/* Includes                      */
/*===============================*/

#include <Arduino.h>
#include <DRV8835MotorShield.h>
#include "Motors/motors.h"
#include "Sensors/accelerometer.h"
#include "Sensors/ultrasonicSensor.h"

/*===============================*/
/* Local functions declarations  */
/*===============================*/
 
static void drivingForward( void );

/*===============================*/
/* Global data definition        */
/*===============================*/

DRV8835MotorShield motors( MOTOR_SHIELD_M1_DIR, 
                           MOTOR_SHIELD_M1_PWM, 
                           MOTOR_SHIELD_M2_DIR, 
                           MOTOR_SHIELD_M2_PWM );

int16_t leftMotorSpeed_s16;
int16_t rightMotorSpeed_s16;
motorsJob_t job_t;
motorsJob_t prevJob_t;

/*===============================*/
/* Functions definitions         */
/*===============================*/

void motorsInit( void )
{
    pinMode( MOTOR_SHIELD_MODE, OUTPUT );
    digitalWrite( MOTOR_SHIELD_MODE, HIGH );
}

void motorsCycle( void )
{
    prevJob_t = job_t;
    job_t = ( ultrasonicSensorData.front_f32 <= DIST_SIDE_MIN ) ? AWAIT : FOLLOW_LEFT_WALL ;

    switch (job_t)
    {
    case FOLLOW_LEFT_WALL:
        followWall( ultrasonicSensorData.left_f32, &rightMotorSpeed_s16, &leftMotorSpeed_s16 );
        break;
    case FOLLOW_RIGHT_WALL:
        followWall( ultrasonicSensorData.left_f32, &leftMotorSpeed_s16, &rightMotorSpeed_s16 );
        break;
    case AWAIT:
    default:
        rightMotorSpeed_s16 = MOTOR_SPEED_STOPPED;
        leftMotorSpeed_s16 = MOTOR_SPEED_STOPPED;
        break;
    }

    motors.setM1Speed( rightMotorSpeed_s16 ); // prawy
    motors.setM2Speed( leftMotorSpeed_s16 ); // lewy
}

static void followWall ( const float wallDistance_f32, int16_t * const firstMotorSpeed_s16, int16_t * const secondMotorSpeed_s16 )
{
  static uint8_t finishCounter_u8;  
  static boolean wallDetected_b;
  float distanceDiff_f32;

  // If job changed - reset data
  if ( prevJob_t != job_t ) 
  {
    wallDetected_b = false;
    finishCounter_u8 = 0U;
  }
  // If wall detected - set flag and reset final counter
  else if ( wallDistance_f32 <= DIST_SIDE_MAX ) 
  {
    wallDetected_b = true;
    finishCounter_u8 = 0U;
  }
  // If wall have been detected and in current cycle wall dissapeared - start counting final counter
  else if (   ( true == wallDetected_b ) 
           && ( wallDistance_f32 <= DIST_SIDE_MAX )   )
  {
    finishCounter_u8++;
  }


  if ( false == wallDetected_b )
  {
    drivingForward();
  }
  else if ( finishCounter_u8 < 5  )
  {
    distanceDiff_f32 = wallDistance_f32 - DIST_SIDE_MIN;
    
    if ( distanceDiff_f32 > DIST_SIDE_OFFSET )
    {
      *firstMotorSpeed_s16 = MOTOR_SPEED_DEFAULT - (int16_t)( distanceDiff_f32 * SPEED_EQUATION_A + SPEED_EQUATION_B );
      *secondMotorSpeed_s16 = MOTOR_SPEED_DEFAULT;
    }
    else if ( -distanceDiff_f32 > DIST_SIDE_OFFSET )
    {
      *firstMotorSpeed_s16 = MOTOR_SPEED_DEFAULT;
      *secondMotorSpeed_s16 = MOTOR_SPEED_DEFAULT - (int16_t)( distanceDiff_f32 * SPEED_EQUATION_A + SPEED_EQUATION_B );
    }
  }
  else
  {
    // TO DO: driving straight for next 20 cm
  }

  prevJob_t = job_t;
}

static void drivingForward( void )
{
  leftMotorSpeed_s16 = MOTOR_SPEED_DEFAULT;
  rightMotorSpeed_s16 = MOTOR_SPEED_DEFAULT;
}
