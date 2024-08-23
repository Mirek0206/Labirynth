#ifndef MOTORS_H
#define MOTORS_H

/*===============================*/
/* Includes                      */
/*===============================*/

#include "Utils/calmanFilter.h"

/*===============================*/
/* Pins connfiguration           */
/*===============================*/

#define MOTOR_SHIELD_MODE   13U
#define MOTOR_SHIELD_M1_DIR 12U
#define MOTOR_SHIELD_M1_PWM 11U
#define MOTOR_SHIELD_M2_DIR 10U
#define MOTOR_SHIELD_M2_PWM 9U

/*===============================*/
/* Constant values               */
/*===============================*/
#define SPEED_MULTI 1.25F

#define TURNING_TIME 950

#define MOTOR_DRIVING_SPEED_MAX 125
#define MOTOR_DRIVING_SPEED_MIN 0
#define MOTOR_TURNING_SPEED 100
#define MOTOR_SPEED_STOPPED 0

#define DIST_SIDE_MIN 7.0F
#define DIST_SIDE_MAX 20.0F
#define DIST_SIDE_MAX_CONTINUOUS 10.0F
#define DIST_SIDE_OFFSET 0.5F
#define DIST_FRONT_COLLISION 7.5F
#define DIST_REQ_FOR_MOVE 25.0F
#define DIST_FINISH 100.0F
#define DIST_AFTER_WALL_FOLLOWING 15.0F

#define SPEED_EQUATION_A 30.0F
#define SPEED_EQUATION_B 0.0F

/*===============================*/
/* Types definitions             */
/*===============================*/

typedef enum motorsJob_tag
{
    AWAIT = 0U,
    FOLLOW_LEFT_WALL,
    FOLLOW_RIGHT_WALL,
    TURN_LEFT,
    TURN_RIGHT
}motorsJob_t;

/*===============================*/
/* Global data                   */
/*===============================*/

extern motorsJob_t job_t;

/*===============================*/
/* Global functions declarations */
/*===============================*/

void motorsInit( void );

void motorsCycle( void );

#endif