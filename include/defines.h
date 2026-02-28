/*
 *
 * @file defines.h
 *
 */

#ifndef DEFINES_H
#define DEFINES_H

// Uncomment this to skip home sensor test
//#define SKIP_HOME

//

// Define the Arduino input Pin number for the DCC Signal 
#define DCC_PIN     2

// The lines below define the pins used to connect to the TMC2209 driver module
#define TMC2209_STEP_PIN      A0  //    4
#define TMC2209_DIRECTION_PIN A1  //    5
#define TMC2209_ENABLE_PIN    A2  //    6

#ifdef TMC2209_ENABLE_PIN
// Uncomment the next line to enable Powering-Off the Stepper when its not running to reduce heating the motor and driver
#define DISABLE_OUTPUTS_IDLE
#endif

// Uncomment the following line to enable Debug Print of DCC Messages
//#define NOTIFY_DCC_MSG

// By default the stepper motor will move the shortest distance to the desired position.
// If you need the turntable to only move in the Positive/Increasing or Negative/Decreasing step numbers to better handle backlash in the mechanism
// Then uncomment the appropriate line below
//#define ALWAYS_MOVE_POSITIVE
//#define ALWAYS_MOVE_NEGATIVE

// The lines below define the stepping speed and acceleration, which you may need to tune for your application
#define STEPPER_MAX_SPEED     800   // Sets the maximum permitted speed
#define STEPPER_ACCELARATION  1000  // Sets the acceleration/deceleration rate
#define STEPPER_SPEED         300   // Sets the desired constant speed for use with runSpeed()

// The line below defines the number of "Full Steps" your stepper motor does for a full rotation
#define MOTOR_FULL_STEPS_PER_REVOLUTION 200

// The line below defines any reduction gearbox multiplier. No gearbox = 1 
#define REDUCTION_GEARBOX_RATIO 1

#define STEPS_PER_REVOLUTION (MOTOR_FULL_STEPS_PER_REVOLUTION * REDUCTION_GEARBOX_RATIO)

// The TMC2209 Driver Board has 3 pins that set the Stepping Mode which are connected to 3 jumpers on the board. 
// Uncomment the line below to match the Boards jumper setting        MS1,     MS2,     MS3
// --------------------------------------------------------------------------------------------
//#define FULL_TURN_STEPS (STEPS_PER_REVOLUTION)      // full steps - MS1=OFF, MS2=OFF, MS3=OFF
//#define FULL_TURN_STEPS (STEPS_PER_REVOLUTION * 2)  // 1/2  steps - MS1=ON,  MS2=OFF, MS3=OFF
//#define FULL_TURN_STEPS (STEPS_PER_REVOLUTION * 4)  // 1/4  steps - MS1=OFF, MS2=ON,  MS3=OFF
//#define FULL_TURN_STEPS (STEPS_PER_REVOLUTION * 8)  // 1/8  steps - MS1=ON,  MS2=ON,  MS3=OFF
//#define FULL_TURN_STEPS (STEPS_PER_REVOLUTION * 16) // 1/16 steps - MS1=ON,  MS2=ON,  MS3=ON

// The TMC2209 Driver Board has 2 pins that set the Stepping Mode which are connected to 2 jumpers on the board. 
// Uncomment the line below to match the Boards jumper setting        MS1,     MS2,     
// --------------------------------------------------------------------------------------------
//#define FULL_TURN_STEPS (STEPS_PER_REVOLUTION * 8)  // 1/8  steps - MS1=OFF, MS2=OFF
//#define FULL_TURN_STEPS (STEPS_PER_REVOLUTION * 16) // 1/16 steps - MS1=ON,  MS2=OFF
//#define FULL_TURN_STEPS (STEPS_PER_REVOLUTION * 64) // 1/64 steps - MS1=OFF, MS2=ON
#define FULL_TURN_STEPS (STEPS_PER_REVOLUTION * 32) // 1/32 steps - MS1=ON,  MS2=ON

// TMC2209 can also be programmed for other settings via uart


#ifndef FULL_TURN_STEPS
#error You need to select one of the FULL_TURN_STEPS to match the TMC2209 Driver Board jumper settings
#endif

// This constant is useful to know the number of steps to rotate the turntable 180 degrees for the back entrance position
#define HALF_TURN_STEPS (FULL_TURN_STEPS / 2)

// Home Position Sensor Input
#define HOME_SENSOR_PIN 5 //   3
#define HOME_SENSOR_ACTIVE_STATE LOW // HIGH

// TMC2209 needs enable inverted
#define INVERT_ENABLE

// The constant HOME_POSITION_DCC_ADDRESS is the base DCC Accessory Decoder Address for the Home Position
// with each subsequent position numbered sequentially from there  
#define POSITION_01_DCC_ADDRESS 200

// I decided to divide the turntable up into 10 Positions using #defines and mathc so it all scales with changes
// to the MS1,MS2 stepping jumpers above and to make the math tidy, but you assign positions how ever you like
// #define POSITION_01 (HALF_TURN_STEPS / 10)

// RKS
// I've set up 48 positions for the tracks.  This is 7.5 deg for each track position
// The positions have a front position and a back position
#define POSITION_01 (HALF_TURN_STEPS / 24)


#define CV_USER_ADDRESS 33

#endif
