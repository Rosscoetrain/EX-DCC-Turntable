/*
 * This is an accessory decoder to control the RT_EX-Turntable board.
 * 
 * The board is available here:
 * 
 * 
 */
#include <Arduino.h>

#include <NmraDcc.h>

#include "version.h"

#include "defines.h"

#include "TtMover.h"

#include "variables.h"

#include "functions.h"



/**********************************************************************
 * setup
 */

void setup()
 {

  pinMode(LED_BUILTIN, OUTPUT);

  MYSERIAL.begin(115200);

  uint8_t maxWaitLoops = 255;
  while(!MYSERIAL && maxWaitLoops--)
    delay(20);

  setVersion();

  // Configure the DCC CV Programing ACK pin for an output
#ifdef ENABLE_DCC_ACK
  pinMode( DccAckPin, OUTPUT );
#endif

  // Setup which External Interrupt, the Pin it's associated with that we're using and enable the Pull-Up
  // Many Arduino Cores now support the digitalPinToInterrupt() function that makes it easier to figure out the
  // Interrupt Number for the Arduino Pin number, which reduces confusion. 

#ifndef GRBL
#ifdef digitalPinToInterrupt
  Dcc.pin(DCC_PIN, 0);
#else
  Dcc.pin(0, DCC_PIN, 1);
#endif
#endif

  // Call the main DCC Init function to enable the DCC Receiver
  Dcc.init( MAN_ID_DIY, DCC_DECODER_VERSION_NUM, CV29_ACCESSORY_DECODER | CV29_OUTPUT_ADDRESS_MODE, 0 );

  ttMover.setHomeLimitPin(HOME_SENSOR_PIN, LIMIT_SENSOR_PIN);
  ttMover.setInvert(INVERT_DIRECTION, INVERT_STEP, INVERT_ENABLE);

  ttMover.init(Dcc.getCV(CV_ACCESSORY_DECODER_WAIT_TIME) * 10);
  ttMover.enableOutputs();

  ttMover.setTrackOne(Dcc.getCV(CV_USER_ADDRESS + 12), Dcc.getCV(CV_USER_ADDRESS + 13), Dcc.getCV(CV_USER_ADDRESS + 14), Dcc.getCV(CV_USER_ADDRESS + 15));
  ttMover.setFullTurnSteps(Dcc.getCV(CV_USER_ADDRESS + 2), Dcc.getCV(CV_USER_ADDRESS + 3));
  ttMover.setTrackAngle(Dcc.getCV(CV_USER_ADDRESS + 4), Dcc.getCV(CV_USER_ADDRESS + 5));

#ifdef DUAL_MOTOR
  ttMover2.setHomeLimitPin(HOME_SENSOR_2_PIN, LIMIT_SENSOR_2_PIN);
  ttMover2.setInvert(INVERT_DIRECTION_2, INVERT_STEP_2, INVERT_ENABLE_2);

  ttMover2.init(Dcc.getCV(CV_ACCESSORY_DECODER_WAIT_TIME) * 10);
  ttMover2.enableOutputs();

  ttMover2.setTrackOne(Dcc.getCV(CV_USER_ADDRESS + 32), Dcc.getCV(CV_USER_ADDRESS + 33), Dcc.getCV(CV_USER_ADDRESS + 34), Dcc.getCV(CV_USER_ADDRESS + 35));
  ttMover2.setFullTurnSteps(Dcc.getCV(CV_USER_ADDRESS + 22), Dcc.getCV(CV_USER_ADDRESS + 23));
  ttMover2.setTrackAngle(Dcc.getCV(CV_USER_ADDRESS + 24), Dcc.getCV(CV_USER_ADDRESS + 25));
#endif

#if TURNTABLE_EX_MODE == TRAVERSER
  ttMover.setNumOfTracks(Dcc.getCV(CV_USER_ADDRESS + 8));
#endif



#ifdef FORCE_RESET_FACTORY_DEFAULT_CV
  MYSERIAL.println("Resetting CVs to Factory Defaults");
  notifyCVResetFactoryDefault(); 
#endif

BaseTurnoutAddress = (((Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_MSB) * 256) + Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_LSB) - 1) * 4) + 1  ;

  MYSERIAL.print(F("Rosscoe Train"));
#if TURNTABLE_EX_MODE == TURNTABLE
  MYSERIAL.print(F(" Turntable "));
#elif TURNTABLE_EX_MODE == TRAVERSER
  MYSERIAL.print(F(" Traverser "));
#endif
  MYSERIAL.print(F(" decoder: "));Serial.println(DCC_DECODER_VERSION_NUM,DEC);
  MYSERIAL.print("DCC Base Address: "); Serial.println(BaseTurnoutAddress, DEC);
  MYSERIAL.print(F("Version: "));
  MYSERIAL.print(versionBuffer[0]);
  MYSERIAL.print(F("."));
  MYSERIAL.print(versionBuffer[1]);
  MYSERIAL.print(F("."));
  MYSERIAL.println(versionBuffer[2]);
  MYSERIAL.println("Init Done");

}

void loop()
{

#ifdef LEARNING_BUTTON
  static int learningbuttonOldval = 0,learningbuttonVal = 0;
#endif

  // You MUST call the NmraDcc.process() method frequently from the Arduino loop() function for correct library operation
  Dcc.process();

  if( FactoryDefaultCVIndex && Dcc.isSetCVReady())
  {
    FactoryDefaultCVIndex--; // Decrement first as initially it is the size of the array 
    Dcc.setCV( FactoryDefaultCVs[FactoryDefaultCVIndex].CV, FactoryDefaultCVs[FactoryDefaultCVIndex].Value);
  }

  ttMover.process();
  ttMover.processLed();

#ifdef DUAL_MOTOR
  ttMover2.process();
#endif


#ifdef LEARNING_BUTTON
  learningbuttonVal = digitalRead(LEARN_BUTTON);

  if (learningbuttonOldval != learningbuttonVal) {
    learningMode = learningbuttonVal;
    if (learningMode == HIGH)
     {
#ifdef DEBUG_MSG_3
      MYSERIAL.println("Learning pin high");
#endif
     }
   }
  learningbuttonOldval = learningbuttonVal;
#endif

    // see if there are serial commands
  readString="";              //empty for next input

while (MYSERIAL.available())
   {
    char c = MYSERIAL.read();     //gets one byte from serial buffer
    readString += c;            //makes the string readString
    delay(10);                   //slow looping to allow buffer to fill with next character
   }

  // act on serial commands

  if (readString.length() >0)
   {
    doSerialCommand(readString);
   } 

}

