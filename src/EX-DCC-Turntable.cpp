// DCC Stepper Motor Controller ( A4988 ) Example for Model Railroad Turntable Control
//
// See: https://www.dccinterface.com/product/arduino-model-railway-dcc-stepper-motor-controller-a4988-assembled/
// 
// Author: Alex Shepherd 2020-06-01
// 
// This example requires two Arduino Libraries:
//
// 1) The AccelStepper library from: http://www.airspayce.com/mikem/arduino/AccelStepper/index.html
//
// 2) The NmraDcc Library from: http://mrrwa.org/download/
//
// Both libraries can be found and installed via the Arduino IDE Library Manager
//
// Also checkout the artical I wrote in this project here:
//         http://mrrwa.org/2017/12/23/dcc-controlled-turntable-stepper-motor-driver/ 
//

/*
 *
 *  Modified by Ross Scanlon (RosscoeTrain) 2026-02-07
 *
 * - Moved defines, variables and functions to their own files.
 * - Setup for platformio
 * - created version.h file
 * 
 * Future changes noted in version.h
 * 
 */


#include <AccelStepper.h>
#include <NmraDcc.h>

#include "version.h"

#include "defines.h"
#include "variables.h"




// --------------------------------------------------------------------------------------------
// You shouldn't need to edit anything below this line unless you're needing to make big changes... ;)
// --------------------------------------------------------------------------------------------
#if defined(ALWAYS_MOVE_POSITIVE) && defined(ALWAYS_MOVE_NEGATIVE)
#error ONLY uncomment one of ALWAYS_MOVE_POSITIVE or ALWAYS_MOVE_NEGATIVE but NOT both
#endif

#define MAX_TURNOUT_POSITIONS (sizeof(turnoutPositions) / sizeof(TurnoutPosition))


#include "functions.h"


void setup()
{
  Serial.begin(115200);
  uint8_t maxWaitLoops = 255;
  while(!Serial && maxWaitLoops--)  // Wait for the USB Device to Enumerate
    delay(20);

  setVersion();

  Serial.println(F("DCC Turntable Control of RT_EX_Turntable board"));
  Serial.print("Verion");
  Serial.print(F("Version: "));
  Serial.print(versionBuffer[0]);
  Serial.print(F("."));
  Serial.print(versionBuffer[1]);
  Serial.print(F("."));
  Serial.println(versionBuffer[2]);
  Serial.println();  Serial.print(F("Full Rotation Steps: "));
  Serial.println(FULL_TURN_STEPS);

  Serial.print(F("Movement Strategy: "));
#if defined ALWAYS_MOVE_POSITIVE
  Serial.println(F("Positive Direction Only"));
#elif defined ALWAYS_MOVE_NEGATIVE
  Serial.println(F("Negative Direction Only"));
#else
  Serial.println(F("Shortest Distance"));
#endif


  for(uint8_t i = 0; i < MAX_TURNOUT_POSITIONS; i++)
  {
    Serial.print(F("DCC Addr: "));
    Serial.print(turnoutPositions[i].dccAddress);

    Serial.print(F(" Front: "));
    Serial.print(turnoutPositions[i].positionFront);

    Serial.print(F(" Back: "));
    Serial.println(turnoutPositions[i].positionBack);
  }


  setupStepperDriver();
  if(moveToHomePosition())
  { 
    setupDCCDecoder();

//    getAddresses();

    // Fake a DCC Packet to cause the Turntable to move to Position 1
    processTurnoutCommand(POSITION_01_DCC_ADDRESS, 1, 1);
  }
}

void loop()
{
  // You MUST call the NmraDcc.process() method frequently from the Arduino loop() function for correct library operation
  Dcc.process();

  // Process the Stepper Library
  stepper.run();

#ifdef DISABLE_OUTPUTS_IDLE
  if(stepper.isRunning() != lastIsRunningState)
  {
    lastIsRunningState = stepper.isRunning();
    if(!lastIsRunningState)
    {
      stepper.disableOutputs();
      Serial.println(F("Disable Stepper Outputs"));
    }
  }
#endif

//#ifdef ENABLE_SERIAL
  doSerialCommand();
//#endif

}

