/*
 *  © 2026 Ross Scanlon
 *
 *
 */


/*
 *
 *  Configuration options to be determined.
 *  Some items from defines.h will be moved here.
 * 
 */


#ifndef CONFIG_H
#define CONFIG_H

// change or add to this to define which serial to use for console output
#define MYSERIAL Serial

// uncomment to build dual motor version.
// the motors do not operate together only as individual entities
#define DUAL_MOTOR

// uncomment to use nano grbl board
#define GRBL



/////////////////////////////////////////////////////////////////////////////////////
//  Define the mode for turntable or traverser
//  0 = TURNTABLE : Use this for normal, 360 degree rotation turntables (Default).
//  1 = TRAVERSER : Use this for vertical or horizontal traversers, or turntables that do
//              do not rotate a full 360 degrees.
// 
#define TURNTABLE_EX_MODE TURNTABLE
//#define TURNTABLE_EX_MODE TRAVERSER

#ifdef DUAL_MOTOR
#define TURNTABLE_2_EX_MODE TURNTABLE
//#define TURNTABLE_2_EX_MODE TRAVERSER
#endif

/*
 * these are configs for the stepper
 */

// When using a two wire driver (eg. A4988, DRV8825, TMC2208), it may be necessary to invert
// the direction pin. This is likely required when using a TMC2208 and TMC2209
#define INVERT_DIRECTION false
// 
// When using a two wire driver (eg. A4988, DRV8825, TMC2208), it may be necessary to invert
// the step pin. If so, uncomment this line.
#define INVERT_STEP false
// 
// When using a two wire driver (eg. A4988, DRV8825, TMC2208), it may be necessary to invert
// the enable pin behaviour if you wish to have the stepper driver disabled when not moving.
#define INVERT_ENABLE true

#ifdef DUAL_MOTOR
// When using a two wire driver (eg. A4988, DRV8825, TMC2208), it may be necessary to invert
// the direction pin. This is likely required when using a TMC2208 and TMC2209
#define INVERT_DIRECTION true
// 
// When using a two wire driver (eg. A4988, DRV8825, TMC2208), it may be necessary to invert
// the step pin. If so, uncomment this line.
#define INVERT_STEP false
// 
// When using a two wire driver (eg. A4988, DRV8825, TMC2208), it may be necessary to invert
// the enable pin behaviour if you wish to have the stepper driver disabled when not moving.
#define INVERT_ENABLE true
#endif


 // The line below defines the number of "Full Steps" your stepper motor does for a full rotation
#define MOTOR_FULL_STEPS_PER_REVOLUTION 200

// The line below defines any reduction gearbox multiplier. No gearbox = 1 
#define REDUCTION_GEARBOX_RATIO 1

#define STEPS_PER_REVOLUTION (MOTOR_FULL_STEPS_PER_REVOLUTION * REDUCTION_GEARBOX_RATIO)

// The TMC2209 Driver Board has 2 pins that set the Stepping Mode which are connected to 2 jumpers on the board. 
// Uncomment the line below to match the Boards jumper setting        MS1,     MS2,     
// --------------------------------------------------------------------------------------------
//#define FULL_TURN_STEPS (STEPS_PER_REVOLUTION * 8)  // 1/8  steps - MS1=OFF, MS2=OFF
//#define FULL_TURN_STEPS (STEPS_PER_REVOLUTION * 16) // 1/16 steps - MS1=ON,  MS2=OFF
//#define FULL_TURN_STEPS (STEPS_PER_REVOLUTION * 64) // 1/64 steps - MS1=OFF, MS2=ON
#define FULL_TURN_STEPS (STEPS_PER_REVOLUTION * 32) // 1/32 steps - MS1=ON,  MS2=ON

// TMC2209 can also be programmed for other settings via uart


// By default the stepper motor will move the shortest distance to the desired position.
// If you need the turntable to only move in the Positive/Increasing or Negative/Decreasing step numbers to better handle backlash in the mechanism
// Then uncomment the appropriate line below
//#define ALWAYS_MOVE_POSITIVE
//#define ALWAYS_MOVE_NEGATIVE

/////////////////////////////////////////////////////////////////////////////////////
//  If dealing with steppers that have a lot of slop, it can be beneficial to force
//  rotating in one direction only. Enable one (and one only) of the below options if
//  a single rotation direction is required.
//  NOTE this does not apply in TRAVERSER mode.
//
//  #define ROTATE_FORWARD_ONLY
//  #define ROTATE_REVERSE_ONLY

/////////////////////////////////////////////////////////////////////////////////////
//  Define phase switching behaviour.
// 
//  PHASE_SWITCHING options:
//  AUTO    : When defined, phase will invert at PHASE_SWITCH_START_ANGLE, and revert
//            at PHASE_SWITCH_STOP_ANGLE (see below).
//  MANUAL  : When defined, phase will only invert using the Turn_PInvert command.
//  
//  Refer to the documentation for the full explanation on phase switching, and when
//  it is recommended to change these options.
// 
#define PHASE_SWITCHING AUTO
// #define PHASE_SWITCHING MANUAL

/////////////////////////////////////////////////////////////////////////////////////
//  Define automatic phase switching angle.
// 
//  If PHASE_SWITCHING is set to AUTO (see above), then when the turntable rotates
//  PHASE_SWITCH_ANGLE degrees from home, the phase will automatically invert.
//  Once the turntable reaches a further 180 degrees, the phase will automatically
//  revert.
// 
//  Refer to the documentation for the full explanation on phase switching, and how to
//  define the angle that's relevant for your layout.
// 
#define PHASE_SWITCH_ANGLE 45



#ifndef FULL_TURN_STEPS
#error You need to select one of the FULL_TURN_STEPS to match the TMC2209 Driver Board jumper settings
#endif

// This constant is useful to know the number of steps to rotate the turntable 180 degrees for the back entrance position
#define HALF_TURN_STEPS (FULL_TURN_STEPS / 2)

// I've set up 48 positions for the tracks.  This is 7.5 deg for each track position
// The positions have a front position and a back position
#define POSITION_01 (HALF_TURN_STEPS / 24)

// angle between tracks * 10
// value 0 - 3600
// default = 7.5 degrees

#define DEFAULT_TRACK_ANGLE 75


//#define NOTIFY_DCC_MSG

#endif
