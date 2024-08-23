/*===============================*/
/* Includes                      */
/*===============================*/

#include <stdint.h>
#include <DRV8835MotorShield.h>
#include "Motors/motors.h"
#include "Sensors/ultrasonicSensor.h"
#include "Sensors/magnetometer.h"
#include "Utils/cycle.h"
#include "config.h"

/*===============================*/
/* Local functions declarations  */
/*===============================*/
 
static void drivingDistance( const float distance_f32 );

static void drivingForward( void );

static void turnLeft( void );

static void turnRight( void );

static void followWall( const float wallDistance_f32, int16_t * const firstMotorSpeed_s16, int16_t * const secondMotorSpeed_s16 );

/*===============================*/
/* Global data definition        */
/*===============================*/

DRV8835MotorShield motors( MOTOR_SHIELD_M1_DIR, 
                           MOTOR_SHIELD_M1_PWM, 
                           MOTOR_SHIELD_M2_DIR, 
                           MOTOR_SHIELD_M2_PWM );

uint64_t cycleNumber_u64;    /* Number of the current cycle */
uint8_t cycleTime_u8;        /* Previous cycle duration [ms] */
int16_t leftMotorSpeed_s16;  /* Speed for left motor */
int16_t rightMotorSpeed_s16; /* Speed for right motor */
motorsJob_t job_t;
motorsJob_t prevJob_t;

/*===============================*/
/* Functions definitions         */
/*===============================*/

void motorsInit( void )
{
    pinMode( MOTOR_SHIELD_MODE, OUTPUT );
    digitalWrite( MOTOR_SHIELD_MODE, HIGH );

    job_t = FOLLOW_LEFT_WALL;
}

void motorsCycle( void )
{
  motorsJob_t currentCycleJob_t = job_t;

  Serial.print(currentCycleJob_t);

  getNumOfCycle( &cycleNumber_u64 );
  cycleTime_u8 = getCycleTime();

  // Stop the algorith if w are exit the maze
  if (   ( ultrasonicSensorData.front_f32 > DIST_FINISH )
      && ( ultrasonicSensorData.left_f32 > DIST_FINISH )
      && ( ultrasonicSensorData.right_f32 > DIST_FINISH )   
      && ( job_t != TURN_LEFT)
      && ( job_t != TURN_RIGHT)   )
  {
    motors.setM1Speed( MOTOR_SPEED_STOPPED ); // prawy
    motors.setM2Speed( MOTOR_SPEED_STOPPED ); 
    while (1);
  }
  // Otherwise,if we are finished the job or we are unable to drive forward - change the job.
  else if (   (   ( ultrasonicSensorData.front_f32 < DIST_FRONT_COLLISION)  
               && ( FOLLOW_LEFT_WALL == job_t )   )
           || ( AWAIT == job_t )   )
  {
    if ( ultrasonicSensorData.left_f32 >= DIST_REQ_FOR_MOVE )
    {
      job_t = TURN_LEFT;
    }
    else if ( ultrasonicSensorData.front_f32 >= DIST_FRONT_COLLISION )
    {
      job_t = FOLLOW_LEFT_WALL;
    }
    else 
    {
      job_t = TURN_RIGHT;
    }
  }
    
  // Call particular function which is responsible for current job.
  switch (currentCycleJob_t)
  {
  case FOLLOW_LEFT_WALL:
      followWall( ultrasonicSensorData.left_f32, &leftMotorSpeed_s16, &rightMotorSpeed_s16 );
      break;
  case FOLLOW_RIGHT_WALL:
      followWall( ultrasonicSensorData.left_f32, &rightMotorSpeed_s16, &leftMotorSpeed_s16 );
      break;
  case TURN_LEFT:
      turnLeft();
      break;
  case TURN_RIGHT:
      turnRight();
      break;
  case AWAIT:
  default:
      rightMotorSpeed_s16 = MOTOR_SPEED_STOPPED;
      leftMotorSpeed_s16 = MOTOR_SPEED_STOPPED;
      break;
  }

  // Save the current job and set motors speed
  prevJob_t = currentCycleJob_t;
  motors.setM1Speed( rightMotorSpeed_s16 ); // prawy
  motors.setM2Speed( leftMotorSpeed_s16 ); // lewy
}

static void followWall( const float wallDistance_f32, int16_t * const firstMotorSpeed_s16, int16_t * const secondMotorSpeed_s16 )
{
         float distanceDiff_f32;
  static boolean wallDetected_b;
         boolean wallDisappear_b = false;

  // If job changed - reset data
  if ( prevJob_t != job_t ) 
  {
    wallDetected_b = false;
    wallDisappear_b = false;
  }
  // If wall detected - set flag and reset final counter
  else if (   (   ( wallDetected_b == false )
               && ( wallDistance_f32 <= DIST_SIDE_MAX )   )
           || (   ( wallDetected_b == true )
               && ( wallDistance_f32 <= DIST_SIDE_MAX_CONTINUOUS )   )   )
  {
    wallDetected_b = true;
    wallDisappear_b = false;
  }
  // If wall have been detected and in current cycle wall dissapeared - start counting final counter
  else if ( true == wallDetected_b )
  {
    wallDisappear_b = true;
  }

  if ( false == wallDetected_b )
  {
    drivingForward();
  }
  else if ( wallDisappear_b != true )
  {
    distanceDiff_f32 = wallDistance_f32 - DIST_SIDE_MIN;
    
    if ( distanceDiff_f32 > DIST_SIDE_OFFSET )
    {
      *firstMotorSpeed_s16 = MOTOR_DRIVING_SPEED_MAX - (int16_t)( distanceDiff_f32 * SPEED_EQUATION_A + SPEED_EQUATION_B );
      *secondMotorSpeed_s16 = MOTOR_DRIVING_SPEED_MAX;
    }
    else if ( -distanceDiff_f32 > DIST_SIDE_OFFSET )
    {
      *firstMotorSpeed_s16 = MOTOR_DRIVING_SPEED_MAX;
      *secondMotorSpeed_s16 = MOTOR_DRIVING_SPEED_MAX - (int16_t)( -distanceDiff_f32 * SPEED_EQUATION_A + SPEED_EQUATION_B );
    }

    *firstMotorSpeed_s16 = max( MOTOR_DRIVING_SPEED_MIN, min( MOTOR_DRIVING_SPEED_MAX , *firstMotorSpeed_s16) );
    *secondMotorSpeed_s16 = max( MOTOR_DRIVING_SPEED_MIN, min( MOTOR_DRIVING_SPEED_MAX , *secondMotorSpeed_s16) );
  }
  else
  {
    drivingDistance( DIST_AFTER_WALL_FOLLOWING );
  }  
}

static void drivingDistance( const float distance_f32 )
{
  static float prevFrontDist_f32;
  static float remainingDistance_f32;
  static uint64_t prevCycleNumber_u64;         /* Cycle number when this functian was called last time */

  if ( prevCycleNumber_u64 != ( cycleNumber_u64 - 1U ) )
  {
    remainingDistance_f32 = distance_f32;
  }
  else
  {
    remainingDistance_f32 -= prevFrontDist_f32 - ultrasonicSensorData.front_f32;
    
    if ( remainingDistance_f32 <= 0.0F )
    {
      job_t = AWAIT;
    }
    else 
    {
      drivingForward();
    }
  }

  prevCycleNumber_u64 = cycleNumber_u64;
  prevFrontDist_f32 = ultrasonicSensorData.front_f32;
}

static void drivingForward( void )
{
  leftMotorSpeed_s16 = MOTOR_DRIVING_SPEED_MAX;
  rightMotorSpeed_s16 = MOTOR_DRIVING_SPEED_MAX;
}

#ifndef MAGNETOMETER
static void turnLeft( void )
{
  static uint64_t prevCycleNumber_u64; /* Cycle number when this functian was called last time */
  static int16_t duration_s16; 
  
  if ( prevCycleNumber_u64 != ( cycleNumber_u64 - 1U ) )
  {
    duration_s16 = TURNING_TIME;
    leftMotorSpeed_s16 = -MOTOR_TURNING_SPEED;
    rightMotorSpeed_s16 = MOTOR_TURNING_SPEED;
  }
  else if (duration_s16 <= 0)
  {
    leftMotorSpeed_s16 = MOTOR_SPEED_STOPPED;
    rightMotorSpeed_s16 = MOTOR_SPEED_STOPPED;

    job_t = ( FOLLOW_HAND == 1U ) ? FOLLOW_LEFT_WALL : AWAIT;
  }
  else 
  {
    duration_s16 -= cycleTime_u8;
  }

  prevCycleNumber_u64 = cycleNumber_u64;
}

static void turnRight( void )
{
  static uint64_t prevCycleNumber_u64;
  static int16_t rightDuration_s16; 

  if ( prevCycleNumber_u64 != ( cycleNumber_u64 - 1U ) )
  {
    rightDuration_s16 = TURNING_TIME;
  }
  else
  {
    if (rightDuration_s16 > 0)
    {
      leftMotorSpeed_s16 = MOTOR_TURNING_SPEED;
      rightMotorSpeed_s16 = -MOTOR_TURNING_SPEED;
      rightDuration_s16 -= cycleTime_u8;
    }
    else
    {
      leftMotorSpeed_s16 = MOTOR_SPEED_STOPPED;
      rightMotorSpeed_s16 = MOTOR_SPEED_STOPPED;
      job_t = ( FOLLOW_HAND == 2U ) ? FOLLOW_RIGHT_WALL : AWAIT;
    }
  }

  prevCycleNumber_u64 = cycleNumber_u64;
}
#else
static void turnLeft( void )
{
  static uint64_t prevCycleNumber_u64; /* Cycle number when this functian was called last time */
  static float headingOffset_f32;     /* Azymut początkowy */
  float currentHeading_f32;
  float headingDiff_f32;
  
  getAzimuth( &currentHeading_f32 );

  // If we have the new turning, set/reset important variables
  if ( prevCycleNumber_u64 != ( cycleNumber_u64 - 1U ) )
  { 
    headingOffset_f32 = currentHeading_f32;

    leftMotorSpeed_s16 = -MOTOR_TURNING_SPEED;
    rightMotorSpeed_s16 = MOTOR_TURNING_SPEED;
  }
  else 
  {
    headingDiff_f32 = currentHeading_f32 - headingOffset_f32;
    
    if (headingDiff_f32 < 0.0F)
    {
      headingDiff_f32 += 360.0F;
    }
    
    if ( headingDiff_f32 > 80.0F )
    {
      leftMotorSpeed_s16 = MOTOR_SPEED_STOPPED;
      rightMotorSpeed_s16 = MOTOR_SPEED_STOPPED;
      job_t = AWAIT;
    }
  }

  prevCycleNumber_u64 = cycleNumber_u64;
}

static void turnRight( void )
{
  static uint64_t prevCycleNumber_u64; /* Cycle number when this functian was called last time */
  static float headingOffset_f32;     /* Azymut początkowy */
  float currentHeading_f32;
  float headingDiff_f32;
  
  getAzimuth( &currentHeading_f32 );

  // If we have the new turning, set/reset important variables
  if ( prevCycleNumber_u64 != ( cycleNumber_u64 - 1U ) )
  { 
    headingOffset_f32 = currentHeading_f32;

    leftMotorSpeed_s16 = MOTOR_TURNING_SPEED;
    rightMotorSpeed_s16 = -MOTOR_TURNING_SPEED;
  }
  else 
  {
    headingDiff_f32 = headingOffset_f32 - currentHeading_f32;
    
    if (headingDiff_f32 < 0.0F)
    {
      headingDiff_f32 += 360.0F;
    }
    
    if ( headingDiff_f32 > 80.0F )
    {
      leftMotorSpeed_s16 = MOTOR_SPEED_STOPPED;
      rightMotorSpeed_s16 = MOTOR_SPEED_STOPPED;
      job_t = AWAIT;
    }
  }

  prevCycleNumber_u64 = cycleNumber_u64;
}
#endif