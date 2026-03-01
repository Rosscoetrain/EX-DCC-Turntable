/*
 *
 * Ross Scanlon 2026
 * 
 * based on the example described below and EX-Turntable by Pete Cole 2023
 * 
 * Built for the RT_EX-Turntable all in one board available here:
 * 
 * https://rosscoe.com/shop/index.php?main_page=product_info&cPath=7&products_id=24
 * 
 * Uses TMC2209 stepper motor controller.
 * 
 * 
 */

// DCC Stepper Motor Controller ( TMC2209 ) Example for Model Railroad Turntable Control
//
// See: https://www.dccinterface.com/product/arduino-model-railway-dcc-stepper-motor-controller-TMC2209-assembled/
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

#include <EEPROM.h>

#include <AccelStepper.h>
#include <NmraDcc.h>

#include "version.h"

#include "defines.h"
#include "variables.h"

#include "Led.h"



// --------------------------------------------------------------------------------------------
// You shouldn't need to edit anything below this line unless you're needing to make big changes... ;)
// --------------------------------------------------------------------------------------------
#if defined(ALWAYS_MOVE_POSITIVE) && defined(ALWAYS_MOVE_NEGATIVE)
#error ONLY uncomment one of ALWAYS_MOVE_POSITIVE or ALWAYS_MOVE_NEGATIVE but NOT both
#endif



#include "functions.h"


void setup()
{
  MYSERIAL.begin(115200);
  uint8_t maxWaitLoops = 255;
  while(!Serial && maxWaitLoops--)  // Wait for the USB Device to Enumerate
    delay(20);

  setVersion();

  MYSERIAL.println(F("DCC Turntable Control of RT_EX_Turntable board"));
  MYSERIAL.print("Verion");
  MYSERIAL.print(F("Version: "));
  MYSERIAL.print(versionBuffer[0]);
  MYSERIAL.print(F("."));
  MYSERIAL.print(versionBuffer[1]);
  MYSERIAL.print(F("."));
  MYSERIAL.println(versionBuffer[2]);
  MYSERIAL.println();  MYSERIAL.print(F("Full Rotation Steps: "));
  MYSERIAL.println(FULL_TURN_STEPS);

  MYSERIAL.print(F("Movement Strategy: "));
#if defined ALWAYS_MOVE_POSITIVE
  MYSERIAL.println(F("Positive Direction Only"));
#elif defined ALWAYS_MOVE_NEGATIVE
  MYSERIAL.println(F("Negative Direction Only"));
#else
  MYSERIAL.println(F("Shortest Distance"));
#endif

  setupPins();

  setupStepperDriver();
  if(moveToHomePosition())
  { 
    setupDCCDecoder();

    getAddresses();


    for(uint8_t i = 0; i < MAX_TURNOUT_POSITIONS; i++)
     {
      MYSERIAL.print(F("DCC Addr: "));
      MYSERIAL.print(turnoutPositions[i].dccAddress);

      MYSERIAL.print(F(" Front: "));
      MYSERIAL.print(turnoutPositions[i].positionFront);

      MYSERIAL.print(F(" Back: "));
      MYSERIAL.println(turnoutPositions[i].positionBack);
     }




    // Fake a DCC Packet to cause the Turntable to move to Position 1
    processTurnoutCommand(POSITION_01_DCC_ADDRESS, 1, 1);
  }
}

void loop()
{
  // You MUST call the NmraDcc.process() method frequently from the Arduino loop() function for correct library operation
  Dcc.process();
  if( FactoryDefaultCVIndex && Dcc.isSetCVReady())
   {
    FactoryDefaultCVIndex--; // Decrement first as initially it is the size of the array 
    Dcc.setCV( FactoryDefaultCVs[FactoryDefaultCVIndex].CV, FactoryDefaultCVs[FactoryDefaultCVIndex].Value);
   }

  // Process the Stepper Library
  stepper.run();

#ifdef DISABLE_OUTPUTS_IDLE
  if(stepper.isRunning() != lastIsRunningState)
  {
    lastIsRunningState = stepper.isRunning();
    if(!lastIsRunningState)
    {
      stepper.disableOutputs();
      MYSERIAL.println(F("Disable Stepper Outputs"));
    }
  }
#endif

  Led.process();

//#ifdef ENABLE_SERIAL
  doSerialCommand();
//#endif

}

