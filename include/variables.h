/*
 * @file variables.h
 */

#ifndef VARIABLES_H
#define VARIABLES_H

NmraDcc  Dcc ;
DCC_MSG  Packet ;

uint8_t FactoryDefaultCVIndex = 0;

uint16_t BaseTurnoutAddress;

int learningMode = LOW;

uint16_t thisCommand = 0;

// buffer to hold serial commands
String readString;

TtMover ttMover(AccelStepper::DRIVER, TMC2209_STEP_PIN, TMC2209_DIRECTION_PIN, 0, 0, false);
#ifdef DUAL_MOTOR
TtMover ttMover2(AccelStepper::DRIVER, TMC2209_2_STEP_PIN, TMC2209_2_DIRECTION_PIN, 0, 0, false);
#endif


char* version;
uint8_t versionBuffer[3];

bool debug = false;


struct CVPair
{
  uint16_t  CV;
  uint8_t   Value;
};



/*
 * to use individual CV's for each address output requires four CV's
 * one for the frontposition_lsb
 * one for the frontposition_msb
 * one for the rearposition_lsb
 * one for the rearposition_msb
 * 
 */


/*
 *           address
 *                       1
 * CV
 *  frontposition_lsb   33
 *  frontposition_msb   34
 *  rearposition_lsb    35
 *  rearposition_msb    36
 */


/*
 * These are stored in an array of CV pairs.
 * Default array is as below
 * 
 */

CVPair FactoryDefaultCVs [] =
{
  {CV_ACCESSORY_DECODER_ADDRESS_LSB, 50},
  {CV_ACCESSORY_DECODER_ADDRESS_MSB, 0},
  {CV_ACCESSORY_DECODER_WAIT_TIME, 5},
  {CV_29_CONFIG, CV29_ACCESSORY_DECODER},


// TURNTABLE_EX_MODE motor 1
// 0x00 = TURNTABLE
// 0x01 = TRAVERSER
  {CV_USER_ADDRESS, 0x00},

// TURNTABLE_2_EX_MODE motor 2
// 0x00 = TURNTABLE
// 0x01 = TRAVERSER
  {CV_USER_ADDRESS + 1, 0x00},


// full turn steps
// FULL_TURN_STEPS
// allows for uint16_t number 
//  ((FULL_TURN_STEPS >> 8) & 0xFF);
//  (FULL_TURN_STEPS & 0xFF);

  {CV_USER_ADDRESS + 2, ((FULL_TURN_STEPS >> 8) & 0xFF)},   // MSB
  {CV_USER_ADDRESS + 3, (FULL_TURN_STEPS & 0xFF)},          // LSB


// angle between tracks * 10.  divide by 10 to get actual angle
// allows for uint16_t number
// value is 0-3600
//  ((DEFAULT_TRACK_ANGLE >> 8) & 0xFF);
//  (DEFAULT_TRACK_ANGLE & 0xFF);

  {CV_USER_ADDRESS + 4, ((DEFAULT_TRACK_ANGLE >> 8) & 0xFF)},   // MSB
  {CV_USER_ADDRESS + 5, (DEFAULT_TRACK_ANGLE & 0xFF)},   // LSB

// fast led flash rate / 10 ms  multiply by 10 to get actual ms
  {CV_USER_ADDRESS + 6, 10},
// slow led flash rate / 10 ms  multiply by 10 to get actual ms
  {CV_USER_ADDRESS + 7, 50},

// number of tracks in traverser mode
// motor 1
// motor 2
#if TURNTABLE_EX_MODE == TRAVERSER
  {CV_USER_ADDRESS + 8, 0x05},
  {CV_USER_ADDRESS + 9, 0x05},
#else
  {CV_USER_ADDRESS + 8, 0x00},
  {CV_USER_ADDRESS + 9, 0x00},
#endif

  {CV_USER_ADDRESS + 10, 0x00},
  {CV_USER_ADDRESS + 11, 0x00},


// track 1
// front position steps
  {CV_USER_ADDRESS + 12, (((POSITION_01 * 1) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 13, ((POSITION_01 * 1) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 14, (((POSITION_01 * 1 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 15, ((POSITION_01 * 1 + HALF_TURN_STEPS) & 0xFF)},

// track 1 defined in config.example.h others calculated by using track angle cvs.


#ifdef DUAL_MOTOR
// full turn steps
// FULL_TURN_STEPS
// allows for uint16_t number 
//  ((FULL_TURN_STEPS >> 8) & 0xFF);
//  (FULL_TURN_STEPS & 0xFF);

  {CV_USER_ADDRESS + 22, ((FULL_TURN_STEPS >> 8) & 0xFF)},   // MSB
  {CV_USER_ADDRESS + 23, (FULL_TURN_STEPS & 0xFF)},          // LSB


// angle between tracks * 10.  divide by 10 to get actual angle
// allows for uint16_t number
// value is 0-3600
//  ((DEFAULT_TRACK_ANGLE >> 8) & 0xFF);
//  (DEFAULT_TRACK_ANGLE & 0xFF);

  {CV_USER_ADDRESS + 24, ((DEFAULT_TRACK_ANGLE >> 8) & 0xFF)},   // MSB
  {CV_USER_ADDRESS + 25, (DEFAULT_TRACK_ANGLE & 0xFF)},   // LSB
// track 1
// front position steps
  {CV_USER_ADDRESS + 32, (((POSITION_01 * 1) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 33, ((POSITION_01 * 1) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 34, (((POSITION_01 * 1 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 35, ((POSITION_01 * 1 + HALF_TURN_STEPS) & 0xFF)},

// track 1 defined in config.example.h others calculated by using track angle cvs.

#endif


};

#endif
