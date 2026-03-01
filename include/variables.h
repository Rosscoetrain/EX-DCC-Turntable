/*
 *
 * @file variables.h
 *
 */

#ifndef VARIABLES_H
#define VARIABLES_H

#include <Arduino.h>
#include <NmraDcc.h>

#include "Led.h"
//#include "TtMover.h"

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
  {CV_ACCESSORY_DECODER_ADDRESS_LSB, 200},
  {CV_ACCESSORY_DECODER_ADDRESS_MSB, 0},
  {CV_29_CONFIG, CV29_ACCESSORY_DECODER},

  {CV_USER_ADDRESS, 0x00},
  {CV_USER_ADDRESS + 1, 0x00},

// full turn steps
// FULL_TURN_STEPS
// allows for uint16_t number 
//  ((FULL_TURN_STEPS >> 8) & 0xFF);
//  (FULL_TURN_STEPS & 0xFF);

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
  {CV_USER_ADDRESS + 24, (((POSITION_01 * 4) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 25, ((POSITION_01 * 4) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 26, (((POSITION_01 * 4 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 27, ((POSITION_01 * 4 + HALF_TURN_STEPS) & 0xFF)},

// track 5
// front position steps
  {CV_USER_ADDRESS + 28, (((POSITION_01 * 5) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 29, ((POSITION_01 * 5) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 30, (((POSITION_01 * 5 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 31, ((POSITION_01 * 5 + HALF_TURN_STEPS) & 0xFF)},

// track 6
// front position steps
  {CV_USER_ADDRESS + 32, (((POSITION_01 * 6) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 33, ((POSITION_01 * 6) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 34, (((POSITION_01 * 6 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 35, ((POSITION_01 * 6 + HALF_TURN_STEPS) & 0xFF)},

// track 7
// front position steps
  {CV_USER_ADDRESS + 36, (((POSITION_01 * 7) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 37, ((POSITION_01 * 7) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 38, (((POSITION_01 * 7 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 39, ((POSITION_01 * 7 + HALF_TURN_STEPS) & 0xFF)},

// track 8
// front position steps
  {CV_USER_ADDRESS + 40, (((POSITION_01 * 8) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 41, ((POSITION_01 * 8) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 42, (((POSITION_01 * 8 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 43, ((POSITION_01 * 8 + HALF_TURN_STEPS) & 0xFF)},

// track 9
// front position steps
  {CV_USER_ADDRESS + 44, (((POSITION_01 * 9) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 45, ((POSITION_01 * 9) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 46, (((POSITION_01 * 9 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 47, ((POSITION_01 * 9 + HALF_TURN_STEPS) & 0xFF)},

// track 10
// front position steps
  {CV_USER_ADDRESS + 48, (((POSITION_01 * 10) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 49, ((POSITION_01 * 10) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 50, (((POSITION_01 * 10 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 51, ((POSITION_01 * 10 + HALF_TURN_STEPS) & 0xFF)},

// track 11
// front position steps
  {CV_USER_ADDRESS + 52, (((POSITION_01 * 11) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 53, ((POSITION_01 * 11) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 54, (((POSITION_01 * 11 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 55, ((POSITION_01 * 11 + HALF_TURN_STEPS) & 0xFF)},


// track 12
// front position steps
  {CV_USER_ADDRESS + 56, (((POSITION_01 * 12) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 57, ((POSITION_01 * 12) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 58, (((POSITION_01 * 12 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 59, ((POSITION_01 * 12 + HALF_TURN_STEPS) & 0xFF)},


// track 13
// front position steps
  {CV_USER_ADDRESS + 60, (((POSITION_01 * 13) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 61, ((POSITION_01 * 13) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 62, (((POSITION_01 * 13 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 63, ((POSITION_01 * 13 + HALF_TURN_STEPS) & 0xFF)},


// track 14
// front position steps
  {CV_USER_ADDRESS + 64, (((POSITION_01 * 14) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 65, ((POSITION_01 * 14) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 66, (((POSITION_01 * 14 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 67, ((POSITION_01 * 14 + HALF_TURN_STEPS) & 0xFF)},


// track 15
// front position steps
  {CV_USER_ADDRESS + 68, (((POSITION_01 * 15) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 69, ((POSITION_01 * 15) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 70, (((POSITION_01 * 15 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 71, ((POSITION_01 * 15 + HALF_TURN_STEPS) & 0xFF)},


// track 16
// front position steps
  {CV_USER_ADDRESS + 72, (((POSITION_01 * 16) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 73, ((POSITION_01 * 16) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 74, (((POSITION_01 * 16 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 75, ((POSITION_01 * 16 + HALF_TURN_STEPS) & 0xFF)},


// track 17
// front position steps
  {CV_USER_ADDRESS + 76, (((POSITION_01 * 17) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 77, ((POSITION_01 * 17) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 78, (((POSITION_01 * 17 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 79, ((POSITION_01 * 17 + HALF_TURN_STEPS) & 0xFF)},


// track 18
// front position steps
  {CV_USER_ADDRESS + 80, (((POSITION_01 * 18) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 81, ((POSITION_01 * 18) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 82, (((POSITION_01 * 18 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 83, ((POSITION_01 * 18 + HALF_TURN_STEPS) & 0xFF)},


// track 19
// front position steps
  {CV_USER_ADDRESS + 84, (((POSITION_01 * 19) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 85, ((POSITION_01 * 19) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 86, (((POSITION_01 * 19 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 87, ((POSITION_01 * 19 + HALF_TURN_STEPS) & 0xFF)},


// track 20
// front position steps
  {CV_USER_ADDRESS + 88, (((POSITION_01 * 20) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 89, ((POSITION_01 * 20) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 90, (((POSITION_01 * 20 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 91, ((POSITION_01 * 20 + HALF_TURN_STEPS) & 0xFF)},


// track 21
// front position steps
  {CV_USER_ADDRESS + 92, (((POSITION_01 * 21) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 93, ((POSITION_01 * 21) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 94, (((POSITION_01 * 21 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 95, ((POSITION_01 * 21 + HALF_TURN_STEPS) & 0xFF)},


// track 22
// front position steps
  {CV_USER_ADDRESS + 96, (((POSITION_01 * 22) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 97, ((POSITION_01 * 22) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 98, (((POSITION_01 * 22 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 99, ((POSITION_01 * 22 + HALF_TURN_STEPS) & 0xFF)},


// track 23
// front position steps
  {CV_USER_ADDRESS + 100, (((POSITION_01 * 23) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 101, ((POSITION_01 * 23) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 102, (((POSITION_01 * 23 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 103, ((POSITION_01 * 23 + HALF_TURN_STEPS) & 0xFF)},


// track 24
// front position steps
  {CV_USER_ADDRESS + 104, (((POSITION_01 * 24) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 105, ((POSITION_01 * 24) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 106, (((POSITION_01 * 24 + HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 107, ((POSITION_01 * 24 + HALF_TURN_STEPS) & 0xFF)},


// TRACKS from 25 - 48 rear position is - HALF_TURN_STEPS

// track 25
// front position steps
  {CV_USER_ADDRESS + 108, (((POSITION_01 * 25) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 109, ((POSITION_01 * 25) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 110, (((POSITION_01 * 25 - HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 111, ((POSITION_01 * 25 - HALF_TURN_STEPS) & 0xFF)},

// track 26
// front position steps
  {CV_USER_ADDRESS + 112, (((POSITION_01 * 26) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 113, ((POSITION_01 * 26) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 114, (((POSITION_01 * 26 - HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 115, ((POSITION_01 * 26 - HALF_TURN_STEPS) & 0xFF)},

// track 27
// front position steps
  {CV_USER_ADDRESS + 116, (((POSITION_01 * 27) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 117, ((POSITION_01 * 27) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 118, (((POSITION_01 * 27 - HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 119, ((POSITION_01 * 27 - HALF_TURN_STEPS) & 0xFF)},

// track 28
// front position steps
  {CV_USER_ADDRESS + 120, (((POSITION_01 * 28) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 121, ((POSITION_01 * 28) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 122, (((POSITION_01 * 28 - HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 123, ((POSITION_01 * 28 - HALF_TURN_STEPS) & 0xFF)},

// track 29
// front position steps
  {CV_USER_ADDRESS + 124, (((POSITION_01 * 29) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 125, ((POSITION_01 * 29) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 126, (((POSITION_01 * 29 - HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 127, ((POSITION_01 * 29 - HALF_TURN_STEPS) & 0xFF)},

// track 30
// front position steps
  {CV_USER_ADDRESS + 128, (((POSITION_01 * 30) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 129, ((POSITION_01 * 30) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 130, (((POSITION_01 * 30 - HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 131, ((POSITION_01 * 30 - HALF_TURN_STEPS) & 0xFF)},

// track 31
// front position steps
  {CV_USER_ADDRESS + 132, (((POSITION_01 * 31) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 133, ((POSITION_01 * 31) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 134, (((POSITION_01 * 31 - HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 135, ((POSITION_01 * 31 - HALF_TURN_STEPS) & 0xFF)},

// track 32
// front position steps
  {CV_USER_ADDRESS + 136, (((POSITION_01 * 32) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 137, ((POSITION_01 * 32) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 138, (((POSITION_01 * 32 - HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 139, ((POSITION_01 * 32 - HALF_TURN_STEPS) & 0xFF)},

// track 33
// front position steps
  {CV_USER_ADDRESS + 140, (((POSITION_01 * 33) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 141, ((POSITION_01 * 33) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 142, (((POSITION_01 * 33 - HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 143, ((POSITION_01 * 33 - HALF_TURN_STEPS) & 0xFF)},

// track 34
// front position steps
  {CV_USER_ADDRESS + 144, (((POSITION_01 * 34) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 145, ((POSITION_01 * 34) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 146, (((POSITION_01 * 34 - HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 147, ((POSITION_01 * 34 - HALF_TURN_STEPS) & 0xFF)},

// track 35
// front position steps
  {CV_USER_ADDRESS + 148, (((POSITION_01 * 35) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 149, ((POSITION_01 * 35) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 150, (((POSITION_01 * 35 - HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 151, ((POSITION_01 * 35 - HALF_TURN_STEPS) & 0xFF)},

// track 36
// front position steps
  {CV_USER_ADDRESS + 152, (((POSITION_01 * 36) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 153, ((POSITION_01 * 36) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 154, (((POSITION_01 * 36 - HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 155, ((POSITION_01 * 36 - HALF_TURN_STEPS) & 0xFF)},

// track 37
// front position steps
  {CV_USER_ADDRESS + 156, (((POSITION_01 * 37) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 157, ((POSITION_01 * 37) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 158, (((POSITION_01 * 37 - HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 159, ((POSITION_01 * 37 - HALF_TURN_STEPS) & 0xFF)},

// track 38
// front position steps
  {CV_USER_ADDRESS + 160, (((POSITION_01 * 38) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 161, ((POSITION_01 * 38) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 162, (((POSITION_01 * 38 - HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 163, ((POSITION_01 * 38 - HALF_TURN_STEPS) & 0xFF)},

// track 39
// front position steps
  {CV_USER_ADDRESS + 164, (((POSITION_01 * 39) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 165, ((POSITION_01 * 39) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 166, (((POSITION_01 * 39 - HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 167, ((POSITION_01 * 39 - HALF_TURN_STEPS) & 0xFF)},

// track 40
// front position steps
  {CV_USER_ADDRESS + 168, (((POSITION_01 * 40) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 169, ((POSITION_01 * 40) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 170, (((POSITION_01 * 40 - HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 171, ((POSITION_01 * 40 - HALF_TURN_STEPS) & 0xFF)},

// track 41
// front position steps
  {CV_USER_ADDRESS + 172, (((POSITION_01 * 41) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 173, ((POSITION_01 * 41) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 174, (((POSITION_01 * 41 - HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 175, ((POSITION_01 * 41 - HALF_TURN_STEPS) & 0xFF)},

// track 42
// front position steps
  {CV_USER_ADDRESS + 176, (((POSITION_01 * 42) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 177, ((POSITION_01 * 42) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 178, (((POSITION_01 * 42 - HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 179, ((POSITION_01 * 42 - HALF_TURN_STEPS) & 0xFF)},

// track 43
// front position steps
  {CV_USER_ADDRESS + 180, (((POSITION_01 * 43) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 181, ((POSITION_01 * 43) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 182, (((POSITION_01 * 43 - HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 183, ((POSITION_01 * 43 - HALF_TURN_STEPS) & 0xFF)},

// track 44
// front position steps
  {CV_USER_ADDRESS + 184, (((POSITION_01 * 44) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 185, ((POSITION_01 * 44) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 186, (((POSITION_01 * 44 - HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 187, ((POSITION_01 * 44 - HALF_TURN_STEPS) & 0xFF)},

// track 45
// front position steps
  {CV_USER_ADDRESS + 188, (((POSITION_01 * 45) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 189, ((POSITION_01 * 45) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 190, (((POSITION_01 * 45 - HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 191, ((POSITION_01 * 45 - HALF_TURN_STEPS) & 0xFF)},

// track 46
// front position steps
  {CV_USER_ADDRESS + 192, (((POSITION_01 * 46) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 193, ((POSITION_01 * 46) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 194, (((POSITION_01 * 46 - HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 195, ((POSITION_01 * 46 - HALF_TURN_STEPS) & 0xFF)},

// track 47
// front position steps
  {CV_USER_ADDRESS + 196, (((POSITION_01 * 47) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 197, ((POSITION_01 * 47) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 198, (((POSITION_01 * 47 - HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 199, ((POSITION_01 * 47 - HALF_TURN_STEPS) & 0xFF)},

// track 48
// front position steps
  {CV_USER_ADDRESS + 200, (((POSITION_01 * 48) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 201, ((POSITION_01 * 48) & 0xFF)},
// rear position steps
  {CV_USER_ADDRESS + 202, (((POSITION_01 * 48 - HALF_TURN_STEPS) >> 8)  & 0xFF)},
  {CV_USER_ADDRESS + 203, ((POSITION_01 * 48 - HALF_TURN_STEPS) & 0xFF)},



// track 
// front position steps
//  {CV_USER_ADDRESS + 1, (((POSITION_01 * ) >> 8)  & 0xFF)},
//  {CV_USER_ADDRESS + 1, ((POSITION_01 * ) & 0xFF)},
// rear position steps
//  {CV_USER_ADDRESS + 1, (((POSITION_01 *  - HALF_TURN_STEPS) >> 8)  & 0xFF)},
//  {CV_USER_ADDRESS + 1, ((POSITION_01 *  - HALF_TURN_STEPS) & 0xFF)},

//  {CV_ACCESSORY_DECODER_SERIAL_MSB, uint8_t(SERIAL_NUMBER / 256)},
//  {CV_ACCESSORY_DECODER_SERIAL_LSB, uint8_t(SERIAL_NUMBER - ((SERIAL_NUMBER / 256) * 256))},

};



uint8_t FactoryDefaultCVIndex = 0;

// This structure holds the values for a turntable position with the DCC Address, Front Position in Steps from Home Sensor
struct TurnoutPosition
{
  uint16_t dccAddress;
  uint32_t positionFront;
  uint32_t positionBack;
};

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
  {POSITION_01_DCC_ADDRESS + 24, POSITION_01 * 25, POSITION_01 * 25 - HALF_TURN_STEPS },   // these are the reverse for positions 24 to 48
  {POSITION_01_DCC_ADDRESS + 25, POSITION_01 * 26, POSITION_01 * 26 - HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 26, POSITION_01 * 27, POSITION_01 * 27 - HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 27, POSITION_01 * 28, POSITION_01 * 28 - HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 28, POSITION_01 * 29, POSITION_01 * 29 - HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 29, POSITION_01 * 30, POSITION_01 * 30 - HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 30, POSITION_01 * 31, POSITION_01 * 31 - HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 31, POSITION_01 * 32, POSITION_01 * 32 - HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 32, POSITION_01 * 33, POSITION_01 * 33 - HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 33, POSITION_01 * 34, POSITION_01 * 34 - HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 34, POSITION_01 * 35, POSITION_01 * 35 - HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 35, POSITION_01 * 36, POSITION_01 * 36 - HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 36, POSITION_01 * 37, POSITION_01 * 37 - HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 37, POSITION_01 * 38, POSITION_01 * 38 - HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 38, POSITION_01 * 39, POSITION_01 * 39 - HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 39, POSITION_01 * 40, POSITION_01 * 40 - HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 40, POSITION_01 * 41, POSITION_01 * 41 - HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 41, POSITION_01 * 42, POSITION_01 * 42 - HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 42, POSITION_01 * 43, POSITION_01 * 43 - HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 43, POSITION_01 * 44, POSITION_01 * 44 - HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 44, POSITION_01 * 45, POSITION_01 * 45 - HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 45, POSITION_01 * 46, POSITION_01 * 46 - HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 46, POSITION_01 * 47, POSITION_01 * 47 - HALF_TURN_STEPS },
  {POSITION_01_DCC_ADDRESS + 47, POSITION_01 * 48, POSITION_01 * 48 - HALF_TURN_STEPS },

};

#define MAX_TURNOUT_POSITIONS (sizeof(turnoutPositions) / sizeof(TurnoutPosition))


// Setup the AccelStepper object for the TMC2209 Stepper Motor Driver
AccelStepper stepper(AccelStepper::DRIVER, TMC2209_STEP_PIN, TMC2209_DIRECTION_PIN);

// Dcc Accessory Decoder object
NmraDcc  Dcc ;

// Variables to store the last DCC Turnout message Address and Direction  
uint16_t lastAddr = 0xFFFF ;
uint8_t  lastDirection = 0xFF;
uint16_t lastStep = 0;


uint8_t phaseSwitch = 0;
long phaseSwitchStartSteps;                         // Defines the step count at which phase should automatically invert.
long phaseSwitchStopSteps;                          // Defines the step count at which phase should automatically revert.
long fullTurnSteps;                                 // Assign our defined full turn steps from config.h.
long halfTurnSteps;                                 // Defines a half turn to enable moving the least distance.

//extern bool testCommandSent;    // Flag a test command has been sent via serial.
//extern uint8_t testActivity;    // Activity sent via serial.
//extern uint8_t testStepsMSB;
//extern uint8_t testStepsLSB;
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

LED Led;

#endif
