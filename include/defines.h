/*
 * @file defines.h
 */

#include <Arduino.h>


#ifndef DEFINES_H
#define DEFINES_H

#define TURNTABLE 0
#define TRAVERSER 1

// If we haven't got a custom config.h, use the example.
#if __has_include ( "config.h")
  #include "config.h"
  #ifndef TURNTABLE_EX_MODE
  #error Your config.h must include a TURNTABLE_EX_MODE definition. If you see this warning in spite of having a config.h, you have a buggy preprocessor and must copy config.example.h to config.h
  #endif
#else
  #warning config.h not found. Using defaults from config.example.h
  #include "config.example.h"
#endif


#define MYSERIAL Serial


// Uncomment to print all DCC Packets
//#define  NOTIFY_DCC_MSG

// Uncomment to print all DCC turnout messages
//#define NOTIFY_TURNOUT_MSG

// Un-Comment the line below to force CVs to be written to the Factory Default values
// defined in the FactoryDefaultCVs below on Start-Up
//#define FORCE_RESET_FACTORY_DEFAULT_CV

// Un-Comment the line below to Enable DCC ACK for Service Mode Programming Read CV Capablilty 
// For reference only not usable in this decoder.
//#define ENABLE_DCC_ACK  15  // This is A1 on the Iowa Scaled Engineering ARD-DCCSHIELD DCC Shield
//#define DccAckPin ENABLE_DCC_ACK



#define LEDCONTROL LED_BUILTIN

// Define the Arduino input Pin number for the DCC Signal 
#define DCC_PIN     2

// Set the Decoder Version - Used by JMRI to Identify the decoder
#define DCC_DECODER_VERSION_NUM 10

// decoder CV wait time between checksensors in ms
#define CV_ACCESSORY_DECODER_WAIT_TIME 2

// define clockwise value
#define DIR_CW 1

// define anti clockwise
#define DIR_ACW !DIR_CW

// Set the number of tracks  THIS is only used to create other defines.  Actual number of tracks is determined by trackAngle CVs.
#define NUM_TRACKS 48

// Set the number of commands that are required.  This is number of tracks plus the other commands

#define NUM_COMMANDS NUM_TRACKS + 14

// Define learning button if it's not defined then applicable functions are not included.
//#define LEARN_BUTTON A7

// command defines track commands

#define CMD_GOTO_1_CW 1
#define CMD_GOTO_1_ACW 2

/* these are not required just kept here for reference */
/*
#define CMD_GOTO_2_CW 11
#define CMD_GOTO_2_ACW 12
#define CMD_GOTO_3_CW 21
#define CMD_GOTO_3_ACW 22
#define CMD_GOTO_4_CW 31
#define CMD_GOTO_4_ACW 32
#define CMD_GOTO_5_CW 41
#define CMD_GOTO_5_ACW 42
#define CMD_GOTO_6_CW 51
#define CMD_GOTO_6_ACW 52
#define CMD_GOTO_7_CW 61
#define CMD_GOTO_7_ACW 62
#define CMD_GOTO_8_CW 71
#define CMD_GOTO_8_ACW 72
#define CMD_GOTO_9_CW 81
#define CMD_GOTO_9_ACW 82
#define CMD_GOTO_10_CW 91
#define CMD_GOTO_10_ACW 92
#define CMD_GOTO_11_CW 101
#define CMD_GOTO_11_ACW 102
#define CMD_GOTO_12_CW 111
#define CMD_GOTO_12_ACW 112
#define CMD_GOTO_13_CW 121
#define CMD_GOTO_13_ACW 122
#define CMD_GOTO_14_CW 131
#define CMD_GOTO_14_ACW 132
#define CMD_GOTO_15_CW 141
#define CMD_GOTO_15_ACW 142
#define CMD_GOTO_16_CW 151
#define CMD_GOTO_16_ACW 152
#define CMD_GOTO_17_CW 161
#define CMD_GOTO_17_ACW 162
#define CMD_GOTO_18_CW 171
#define CMD_GOTO_18_ACW 172
#define CMD_GOTO_19_CW 181
#define CMD_GOTO_19_ACW 182
#define CMD_GOTO_20_CW 191
#define CMD_GOTO_20_ACW 192
#define CMD_GOTO_21_CW 201
#define CMD_GOTO_21_ACW 202
#define CMD_GOTO_22_CW 211
#define CMD_GOTO_22_ACW 212
#define CMD_GOTO_23_CW 221
#define CMD_GOTO_23_ACW 222
#define CMD_GOTO_24_CW 231
#define CMD_GOTO_24_ACW 232
#define CMD_GOTO_25_CW 241
#define CMD_GOTO_25_ACW 242
#define CMD_GOTO_26_CW 251
#define CMD_GOTO_26_ACW 252
#define CMD_GOTO_27_CW 261
#define CMD_GOTO_27_ACW 262
#define CMD_GOTO_28_CW 271
#define CMD_GOTO_28_ACW 272
#define CMD_GOTO_29_CW 281
#define CMD_GOTO_29_ACW 282
#define CMD_GOTO_30_CW 291
#define CMD_GOTO_30_ACW 292
#define CMD_GOTO_31_CW 301
#define CMD_GOTO_31_ACW 302
#define CMD_GOTO_32_CW 311
#define CMD_GOTO_32_ACW 322
#define CMD_GOTO_33_CW 331
#define CMD_GOTO_33_ACW 332
#define CMD_GOTO_34_CW 331
#define CMD_GOTO_34_ACW 332
#define CMD_GOTO_35_CW 341
#define CMD_GOTO_35_ACW 342
#define CMD_GOTO_36_CW 351
#define CMD_GOTO_36_ACW 352
#define CMD_GOTO_37_CW 361
#define CMD_GOTO_37_ACW 362
#define CMD_GOTO_38_CW 371
#define CMD_GOTO_38_ACW 372
#define CMD_GOTO_39_CW 381
#define CMD_GOTO_39_ACW 382
#define CMD_GOTO_40_CW 391
#define CMD_GOTO_40_ACW 392
#define CMD_GOTO_41_CW 401
#define CMD_GOTO_41_ACW 402
#define CMD_GOTO_42_CW 411
#define CMD_GOTO_42_ACW 412
#define CMD_GOTO_43_CW 421
#define CMD_GOTO_43_ACW 422
#define CMD_GOTO_44_CW 431
#define CMD_GOTO_44_ACW 432
#define CMD_GOTO_45_CW 441
#define CMD_GOTO_45_ACW 442
#define CMD_GOTO_46_CW 451
#define CMD_GOTO_46_ACW 452
#define CMD_GOTO_47_CW 461
#define CMD_GOTO_47_ACW 462
*/
#define CMD_GOTO_48_CW 471
#define CMD_GOTO_48_ACW 472

#define CMD_LED_OFF 481
#define CMD_LED_ON 482
#define CMD_LED_SLOW 491
#define CMD_LED_FAST 492

#define CMD_ACC_OFF 501
#define CMD_ACC_ON 502

#define CMD_EXT1_OFF 511
#define CMD_EXT1_ON 512
#define CMD_EXT2_OFF 521
#define CMD_EXT2_ON 522
#define CMD_EXT3_OFF 531
#define CMD_EXT3_ON 532
#define CMD_EXT4_OFF 541
#define CMD_EXT4_ON 542


/*
 *
 *  TMC2209 driver pins
 * 
 */
#ifdef GRBL
#define TMC2209_STEP_PIN  5
#define TMC2209_DIRECTION_PIN 2
#define TMC2209_ENABLE_PIN 8
#ifdef DUAL_MOTOR
#define TMC2209_2_STEP_PIN  6
#define TMC2209_2_DIRECTION_PIN 3
#define TMC2209_2_ENABLE_PIN 8
#endif
#else
#define TMC2209_STEP_PIN  A0
#define TMC2209_DIRECTION_PIN A1
#define TMC2209_ENABLE_PIN A2
#ifdef DUAL_MOTOR
#endif
#endif

/*
 *
 * stepper motor settings
 * 
 */

#define STEPPER_MAX_SPEED 400
#define STEPPER_ACCELERATION 100
#define STEPPER_SPEED 200

/*
 *
 * other stepper settings
 *
 */

#ifdef TMC2209_ENABLE_PIN
#define DISABLE_OUTPUTS_IDLE
#endif

#define SANITY_STEPS 30000

#define HOME_SENSITIVITY 600


/*
 *
 * sensor settings
 * 
 */

// home sensor active setting

#define HOME_SENSOR_ACTIVE_STATE LOW

// limit sensor active setting

#define LIMIT_SENSOR_ACTIVE_STATE LOW




/*
 *
 * board pins
 * 
 */

#ifdef GRBL
#define HOME_SENSOR_PIN A0
#define LIMIT_SENSOR_PIN A1
#define LED_PIN 13
#define ACC_PIN A2
#define EXT1_PIN 9
#define EXT2_PIN 10
#ifdef DUAL_MOTOR
#define HOME_SENSOR_2_PIN 11
#define LIMIT_SENSOR_2_PIN 12
#else
#define EXT3_PIN 11
#define EXT4_PIN 12
#endif
#define RELAY_PIN A3
#else
#define HOME_SENSOR_PIN 5
#define LIMIT_SENSOR_PIN 8

#define LED_PIN 6
#define ACC_PIN 7
#define EXT1_PIN 9
#define EXT2_PIN 10
#define EXT3_PIN 11
#define EXT4_PIN 12

#define RELAY_PIN 4
#endif


/*
 *  led flash defines
 */

#define LED_SLOW 500
#define LED_FAST 100

/*
 *
 */

#define CV_USER_ADDRESS 33


#endif
