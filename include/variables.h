/*
 *
 * @file variables.h
 *
 */

#ifndef VARIABLES_H
#define VARIABLES_H


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
 *                       1    2    3    4    5    6    7    8    9   10   11   12
 * CV
 *  frontposition_lsb   33   37   41   45   49   53   57   61   65   69   73   77
 *  frontposition_msb   34   38   42   46   50   54   58   62   66   70   74   78
 *  rearposition_lsb    35   39   43   47   51   55   59   63   67   71   75   79
 *  rearposition_msb    36   40   44   48   52   56   60   64   68   72   76   80
 */


/*
 * These are stored in an array of CV pairs.
 * Default array is as below
 * 
 */

CVPair FactoryDefaultCVs [] =
{
//  {CV_ACCESSORY_DECODER_ADDRESS_LSB, DEFAULT_ACCESSORY_DECODER_ADDRESS & 0xFF},
//  {CV_ACCESSORY_DECODER_ADDRESS_MSB, DEFAULT_ACCESSORY_DECODER_ADDRESS >> 8},
//  {CV_29_CONFIG, CV29_ACCESSORY_DECODER},
  {CV_ACCESSORY_DECODER_ADDRESS_LSB, 1},
  {CV_ACCESSORY_DECODER_ADDRESS_MSB, 0},
  {CV_29_CONFIG, CV29_ACCESSORY_DECODER},



// full turn steps
// FULL_TURN_STEPS
// allows for uint32_t number 
//  ((FULL_TURN_STEPS >> 24) & 0xFF);
//  ((FULL_TURN_STEPS >> 16) & 0xFF);
//  ((FULL_TURN_STEPS >> 8) & 0xFF);
//  (FULL_TURN_STEPS & 0xFF);

  {CV_USER_ADDRESS, 0x00},
  {CV_USER_ADDRESS + 1, 0x00},

  {CV_USER_ADDRESS + 2, ((FULL_TURN_STEPS >> 8) & 0xFF)},
  {CV_USER_ADDRESS + 3, (FULL_TURN_STEPS & 0xFF)},


// fast led flash rate / 10 ms  multiply by 10 to get actual ms
  {CV_USER_ADDRESS + 4, 10},
// slow led flash rate / 10 ms  multiply by 10 to get actual ms
  {CV_USER_ADDRESS + 5, 50},
  {CV_USER_ADDRESS + 6, 0x00},
  {CV_USER_ADDRESS + 7, 0x00},
  {CV_USER_ADDRESS + 8, 0x00},
  {CV_USER_ADDRESS + 9, 0x00},
  {CV_USER_ADDRESS + 10, 0x00},
  {CV_USER_ADDRESS + 11, 0x00},



// track 1
// front position steps
  {CV_USER_ADDRESS + 12, (((POSITION_01 * 1) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 13, ((POSITION_01 * 1) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 14, (((POSITION_01 * 1 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 15, ((POSITION_01 * 1 + HALF_TURN_STEPS) & 0xFF)},

// track 2
// front position steps

  {CV_USER_ADDRESS + 16, (((POSITION_01 * 2) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 17, ((POSITION_01 * 2) & 0xFF)},
  // rear position steps

  {CV_USER_ADDRESS + 18, (((POSITION_01 * 2 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 19, ((POSITION_01 * 2 + HALF_TURN_STEPS) & 0xFF)},

// track 3
// front position steps

  {CV_USER_ADDRESS + 20, (((POSITION_01 * 3) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 21, ((POSITION_01 * 3) & 0xFF)},
  // rear position steps

  {CV_USER_ADDRESS + 22, (((POSITION_01 * 3 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 23, ((POSITION_01 * 3 + HALF_TURN_STEPS) & 0xFF)},



// track 4
// front position steps
  {CV_USER_ADDRESS + 36, (((POSITION_01 * 4) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 37, ((POSITION_01 * 4) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 40, (((POSITION_01 * 4 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 41, ((POSITION_01 * 4 + HALF_TURN_STEPS) & 0xFF)},

// track 5
// front position steps
  {CV_USER_ADDRESS + 44, (((POSITION_01 * 5) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 45, ((POSITION_01 * 5) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 48, (((POSITION_01 * 5 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 49, ((POSITION_01 * 5 + HALF_TURN_STEPS) & 0xFF)},

// track 6
// front position steps
  {CV_USER_ADDRESS + 52, (((POSITION_01 * 6) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 53, ((POSITION_01 * 6) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 56, (((POSITION_01 * 6 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 57, ((POSITION_01 * 6 + HALF_TURN_STEPS) & 0xFF)},

// track 7
// front position steps
  {CV_USER_ADDRESS + 60, (((POSITION_01 * 7) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 61, ((POSITION_01 * 7) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 64, (((POSITION_01 * 7 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 65, ((POSITION_01 * 7 + HALF_TURN_STEPS) & 0xFF)},

// track 8
// front position steps
  {CV_USER_ADDRESS + 68, (((POSITION_01 * 8) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 69, ((POSITION_01 * 8) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 72, (((POSITION_01 * 8 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 73, ((POSITION_01 * 8 + HALF_TURN_STEPS) & 0xFF)},

// track 9
// front position steps
  {CV_USER_ADDRESS + 76, (((POSITION_01 * 9) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 77, ((POSITION_01 * 9) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 80, (((POSITION_01 * 9 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 81, ((POSITION_01 * 9 + HALF_TURN_STEPS) & 0xFF)},


// track 10
// front position steps
  {CV_USER_ADDRESS + 84, (((POSITION_01 * 10) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 85, ((POSITION_01 * 10) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 88, (((POSITION_01 * 10 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 89, ((POSITION_01 * 10 + HALF_TURN_STEPS) & 0xFF)},

// track 11
// front position steps
  {CV_USER_ADDRESS + 92, (((POSITION_01 * 11) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 93, ((POSITION_01 * 11) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 96, (((POSITION_01 * 11 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 97, ((POSITION_01 * 11 + HALF_TURN_STEPS) & 0xFF)},

// track 12
// front position steps
  {CV_USER_ADDRESS + 100, (((POSITION_01 * 12) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 101, ((POSITION_01 * 12) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 104, (((POSITION_01 * 12 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 105, ((POSITION_01 * 12 + HALF_TURN_STEPS) & 0xFF)},

// track 13
// front position steps
  {CV_USER_ADDRESS + 108, (((POSITION_01 * 13) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 109, ((POSITION_01 * 13) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 112, (((POSITION_01 * 13 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 113, ((POSITION_01 * 13 + HALF_TURN_STEPS) & 0xFF)},

// track 14
// front position steps
  {CV_USER_ADDRESS + 116, (((POSITION_01 * 14) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 117, ((POSITION_01 * 14) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 120, (((POSITION_01 * 14 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 121, ((POSITION_01 * 14 + HALF_TURN_STEPS) & 0xFF)},

// track 15
// front position steps
  {CV_USER_ADDRESS + 124, (((POSITION_01 * 15) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 125, ((POSITION_01 * 15) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 128, (((POSITION_01 * 15 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 129, ((POSITION_01 * 15 + HALF_TURN_STEPS) & 0xFF)},

// track 16
// front position steps
  {CV_USER_ADDRESS + 132, (((POSITION_01 * 16) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 133, ((POSITION_01 * 16) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 136, (((POSITION_01 * 16 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 137, ((POSITION_01 * 16 + HALF_TURN_STEPS) & 0xFF)},

// track 17
// front position steps
  {CV_USER_ADDRESS + 140, (((POSITION_01 * 17) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 141, ((POSITION_01 * 17) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 144, (((POSITION_01 * 17 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 145, ((POSITION_01 * 17 + HALF_TURN_STEPS) & 0xFF)},

// track 18
// front position steps
  {CV_USER_ADDRESS + 148, (((POSITION_01 * 18) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 149, ((POSITION_01 * 18) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 152, (((POSITION_01 * 18 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 153, ((POSITION_01 * 18 + HALF_TURN_STEPS) & 0xFF)},

// track 19
// front position steps
  {CV_USER_ADDRESS + 156, (((POSITION_01 * 19) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 157, ((POSITION_01 * 19) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 160, (((POSITION_01 * 19 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 161, ((POSITION_01 * 19 + HALF_TURN_STEPS) & 0xFF)},

// track 20
// front position steps
  {CV_USER_ADDRESS + 164, (((POSITION_01 * 20) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 165, ((POSITION_01 * 20) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 168, (((POSITION_01 * 20 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 169, ((POSITION_01 * 20 + HALF_TURN_STEPS) & 0xFF)},

// track 21
// front position steps
  {CV_USER_ADDRESS + 172, (((POSITION_01 * 21) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 173, ((POSITION_01 * 21) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 176, (((POSITION_01 * 21 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 177, ((POSITION_01 * 21 + HALF_TURN_STEPS) & 0xFF)},

// track 22
// front position steps
  {CV_USER_ADDRESS + 180, (((POSITION_01 * 22) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 181, ((POSITION_01 * 22) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 184, (((POSITION_01 * 22 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 185, ((POSITION_01 * 22 + HALF_TURN_STEPS) & 0xFF)},

// track 23
// front position steps
  {CV_USER_ADDRESS + 188, (((POSITION_01 * 23) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 189, ((POSITION_01 * 23) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 192, (((POSITION_01 * 23 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 193, ((POSITION_01 * 23 + HALF_TURN_STEPS) & 0xFF)},

// track 24
// front position steps
  {CV_USER_ADDRESS + 196, (((POSITION_01 * 24) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 197, ((POSITION_01 * 24) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 200, (((POSITION_01 * 24 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 201, ((POSITION_01 * 24 + HALF_TURN_STEPS) & 0xFF)},




// track 
// front position steps
//  {CV_USER_ADDRESS + 1, (((POSITION_01 * 2) >> 8)  & 0xFF)},
//  {CV_USER_ADDRESS + 1, ((POSITION_01 * 2) & 0xFF)},
// rear position steps
//  {CV_USER_ADDRESS + 1, (((POSITION_01 * 2 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
//  {CV_USER_ADDRESS + 1, ((POSITION_01 * 2 + HALF_TURN_STEPS) & 0xFF)},

//  {CV_ACCESSORY_DECODER_SERIAL_MSB, uint8_t(SERIAL_NUMBER / 256)},
//  {CV_ACCESSORY_DECODER_SERIAL_LSB, uint8_t(SERIAL_NUMBER - ((SERIAL_NUMBER / 256) * 256))},

};



uint8_t FactoryDefaultCVIndex = 0;

// This structure holds the values for a turntable position with the DCC Address, Front Position in Steps from Home Sensor
typedef struct
{
  uint16_t dccAddress;
  uint32_t positionFront;
  uint32_t positionBack;
}
TurnoutPosition;

TurnoutPosition turnoutPositions[] = {
  {POSITION_01_DCC_ADDRESS + 0, POSITION_01 * 1, POSITION_01 * 1 + HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 1, POSITION_01 * 2, POSITION_01 * 2 + HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 2, POSITION_01 * 3, POSITION_01 * 3 + HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 3, POSITION_01 * 4, POSITION_01 * 4 + HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 4, POSITION_01 * 5, POSITION_01 * 5 + HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 5, POSITION_01 * 6, POSITION_01 * 6 + HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 6, POSITION_01 * 7, POSITION_01 * 7 + HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 7, POSITION_01 * 8, POSITION_01 * 8 + HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 8, POSITION_01 * 9, POSITION_01 * 9 + HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 9, POSITION_01 * 10, POSITION_01 * 10 + HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 10, POSITION_01 * 11, POSITION_01 * 11 + HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 11, POSITION_01 * 12, POSITION_01 * 12 + HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 12, POSITION_01 * 13, POSITION_01 * 13 + HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 13, POSITION_01 * 14, POSITION_01 * 14 + HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 14, POSITION_01 * 15, POSITION_01 * 15 + HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 15, POSITION_01 * 16, POSITION_01 * 16 + HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 16, POSITION_01 * 17, POSITION_01 * 17 + HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 17, POSITION_01 * 18, POSITION_01 * 18 + HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 18, POSITION_01 * 19, POSITION_01 * 19 + HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 19, POSITION_01 * 20, POSITION_01 * 20 + HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 20, POSITION_01 * 21, POSITION_01 * 21 + HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 21, POSITION_01 * 22, POSITION_01 * 22 + HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 22, POSITION_01 * 23, POSITION_01 * 23 + HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 23, POSITION_01 * 24, POSITION_01 * 24 + HALF_TURN_STEPS },
};



// Setup the AccelStepper object for the A4988 Stepper Motor Driver
AccelStepper stepper(AccelStepper::DRIVER, A4988_STEP_PIN, A4988_DIRECTION_PIN);

// Dcc Accessory Decoder object
NmraDcc  Dcc ;

// Variables to store the last DCC Turnout message Address and Direction  
uint16_t lastAddr = 0xFFFF ;
uint8_t  lastDirection = 0xFF;
uint16_t lastStep = 0;

//extern bool testCommandSent;    // Flag a test command has been sent via serial.
//extern uint8_t testActivity;    // Activity sent via serial.
//extern uint8_t testStepsMSB;
//extern uint8_t testStepsLSB;
//extern bool debug;
//extern bool sensorTesting;

unsigned long gearingFactor = REDUCTION_GEARBOX_RATIO;
const byte numChars = 20;
char serialInputChars[numChars];
bool newSerialData = false;
bool testCommandSent = false;
uint8_t testActivity = 0;
uint8_t testStepsMSB = 0;
uint8_t testStepsLSB = 0;
#ifdef DEBUG
bool debug = true;
#else
bool debug = false;
#endif
#ifdef SENSOR_TESTING
bool sensorTesting = true;
#else
bool sensorTesting = false;
#endif

char* version;
uint8_t versionBuffer[3];

#ifdef DISABLE_OUTPUTS_IDLE
bool lastIsRunningState;
#endif 

uint16_t baseAddress;


#endif
