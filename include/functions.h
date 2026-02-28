/*
 *
 * @file functions.h
 *
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H



void processTurnoutCommand(uint16_t Addr, uint8_t Direction, uint8_t OutputPower)
{
  Serial.print(F("processTurnoutCommand: "));
  Serial.print(F(" DCC address :"));
  Serial.println(Addr);

  for (uint8_t i = 0; i < MAX_TURNOUT_POSITIONS ; i++)
  {
    if ((Addr == turnoutPositions[i].dccAddress) && ((Addr != lastAddr) || (Direction != lastDirection)) && OutputPower)
    {
      lastAddr = Addr ;
      lastDirection = Direction ;
      
      Serial.print(F("Moving to "));
      Serial.print(Direction ? F("Front") : F("Back"));
      Serial.print(F(" Position: "));
      Serial.print(i, DEC);
      Serial.print(F(" @ Step: "));

#ifdef TMC2209_ENABLE_PIN
      stepper.enableOutputs();
#endif

      int newStep;
      if(Direction)
        newStep = turnoutPositions[i].positionFront;
      else
        newStep = turnoutPositions[i].positionBack;

      Serial.print(newStep, DEC);
      
      Serial.print(F("  Last Step: "));
      Serial.print(lastStep, DEC);
      
      int diffStep = newStep - lastStep;
      Serial.print(F("  Diff Step: "));
      Serial.print(diffStep, DEC);

#if defined ALWAYS_MOVE_POSITIVE
      Serial.print(F("  Positive"));       
      if(diffStep < 0)
        diffStep += FULL_TURN_STEPS;
        
#elif defined ALWAYS_MOVE_NEGATIVE
      Serial.print(F("  Negative"));       
      if(diffStep > 0)
        diffStep -= FULL_TURN_STEPS;
#else
      if(diffStep > HALF_TURN_STEPS)
        diffStep = diffStep - FULL_TURN_STEPS;
        
      else if(diffStep < -HALF_TURN_STEPS)
        diffStep = diffStep + FULL_TURN_STEPS;
#endif

      Serial.print(F("  Move: "));
      Serial.println(diffStep, DEC);
      stepper.move(diffStep);

      lastStep = newStep;
      break;
    }
  }
}


void setupStepperDriver()
{
#ifdef TMC2209_ENABLE_PIN
  stepper.setPinsInverted(false, false, true); // Its important that these commands are in this order
  stepper.setEnablePin(TMC2209_ENABLE_PIN);    // otherwise the Outputs are NOT enabled initially
#endif
   
  stepper.setMaxSpeed(STEPPER_MAX_SPEED);        // Sets the maximum permitted speed
  stepper.setAcceleration(STEPPER_ACCELARATION); // Sets the acceleration/deceleration rate
  stepper.setSpeed(STEPPER_SPEED);               // Sets the desired constant speed for use with runSpeed()

#ifdef TMC2209_ENABLE_PIN
  stepper.enableOutputs();
#endif

#ifdef DISABLE_OUTPUTS_IDLE
  lastIsRunningState = stepper.isRunning();
#endif
}

bool moveToHomePosition()
{
#ifdef SKIP_HOME
  return true;
#endif
  Serial.println(F("Finding Home Sensor...."));

  pinMode(HOME_SENSOR_PIN, INPUT_PULLUP);

#ifdef ALWAYS_MOVE_NEGATIVE
  stepper1.move(0 - (FULL_TURN_STEPS * 2));
#else
  stepper.move(FULL_TURN_STEPS * 2);
#endif  
  while(digitalRead(HOME_SENSOR_PIN) != HOME_SENSOR_ACTIVE_STATE)
    stepper.run();

  if(digitalRead(HOME_SENSOR_PIN) == HOME_SENSOR_ACTIVE_STATE)
  {
    stepper.stop();
    stepper.setCurrentPosition(0);
    Serial.println(F("Found Home Position - Setting Current Position to 0"));
    return true;
  }
  else
    Serial.println(F("Home Position NOT FOUND - Check Sensor Hardware"));

  return false;  
}

void setupDCCDecoder()
{
  Serial.println(F("Setting up DCC Decorder..."));

  // Setup which External Interrupt, the Pin it's associated with that we're using and enable the Pull-Up
  // Many Arduino Cores now support the digitalPinToInterrupt() function that makes it easier to figure out the
  // Interrupt Number for the Arduino Pin number, which reduces confusion. 
#ifdef digitalPinToInterrupt
  Dcc.pin(DCC_PIN, 0);
#else
  Dcc.pin(0, DCC_PIN, 1);
#endif
  
  // Call the main DCC Init function to enable the DCC Receiver
  Dcc.init( MAN_ID_DIY, 10, CV29_ACCESSORY_DECODER, 0 );
}


void notifyCVChange(uint16_t CV, uint8_t Value)
 {
  Serial.print("notifyCVChange: CV: ") ;
  Serial.print(CV,DEC) ;
  Serial.print(" Value: ") ;
  Serial.println(Value, DEC) ;

  Value = Value;  // Silence Compiler Warnings...
 }






void(* resetFunc) (void) = 0;                     // declare reset function at address 0

// R soft reset
void serialCommandR()
 {
  Serial.println("Resetting");
  delay(2000);
  resetFunc();
 }


// M command to move
void serialCommandM(long steps) {
  if (stepper.isRunning()) {
    Serial.println(F("Stepper is running, ignoring <M>"));
    return;
  }
  if (steps < 0) {
    Serial.println(F("Cannot provide a negative step count"));
  } else if (steps > 32767) {
    Serial.println(F("Step count too large, refer to the documentation for large step counts > 32767"));
  } else {
    Serial.print(F("Test move "));
    Serial.print(steps);
    Serial.print(F(" steps, activity ID "));
    Serial.println(testActivity);
//    testStepsMSB = steps >> 8;
//    testStepsLSB = steps & 0xFF;
//    testCommandSent = true;
//    receiveEvent(3);

    uint8_t addr = (steps / (FULL_TURN_STEPS/24)) + POSITION_01_DCC_ADDRESS;

    Serial.print(F(" addr "));
    Serial.println(addr);

    processTurnoutCommand(addr, 1, 1);
//    processTurnoutCommand(200, 0, 0);
  }
}


void serialCommandS()
 {
  Serial.println(F("Reset factory default CVs"));
  notifyCVResetFactoryDefault();
 }


// calibrate turntable
void serialCommandC()
 {
  if (stepper.isRunning())
   {
    Serial.println(F("Stepper is running, ignoring <C>"));
    return;
   }

  moveToHomePosition();

/*
  if (!calibrating || homed == 2)
   {
    initiateCalibration();
   }
*/
 }


/*
void serialCommand()
 {

 }
*/


void showCVs()
 {
  Serial.println(F("CVs are:"));
  Serial.print(F("CV"));
  Serial.print(CV_ACCESSORY_DECODER_ADDRESS_LSB);
  Serial.print(F(" = "));
  Serial.println(Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_LSB));
  Serial.print(F("CV"));
  Serial.print(CV_ACCESSORY_DECODER_ADDRESS_MSB);
  Serial.print(F(" = "));
  Serial.println(Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_MSB));
  Serial.print(F("CV"));
  Serial.print(CV_29_CONFIG);
  Serial.print(F(" = "));
  Serial.println(Dcc.getCV(CV_29_CONFIG));

  for (int i = 0; i < MAX_TURNOUT_POSITIONS; i++)
   {
    Serial.print(F("CV : "));
    Serial.print(CV_USER_ADDRESS + (8 * i) + 10);
    Serial.print(F(" = "));
    Serial.println(Dcc.getCV(CV_USER_ADDRESS + (8 * i) + 10));
   }

 }


void doSerialCommand()
 {
  static bool serialInProgress = false;
  static byte serialIndex = 0;
  char startMarker = '<';
  char endMarker = '>';
  char serialChar;
  while (Serial.available() > 0 && newSerialData == false) {
    serialChar = Serial.read();
    if (serialInProgress == true) {
      if (serialChar != endMarker) {
        serialInputChars[serialIndex] = serialChar;
        serialIndex++;
        if (serialIndex >= numChars) {
          serialIndex = numChars - 1;
        }
      } else {
        serialInputChars[serialIndex] = '\0';
        serialInProgress = false;
        serialIndex = 0;
        newSerialData = true;
      }
    } else if (serialChar == startMarker) {
      serialInProgress = true;
    }
  }
  if (newSerialData == true) {
    newSerialData = false;
    char * strtokIndex;
    strtokIndex = strtok(serialInputChars," ");
    char command = strtokIndex[0];     // first parameter is activity
    strtokIndex = strtok(NULL," ");     // space separator
    long steps;
    if (command == 'M') {
      steps = atol(strtokIndex);
      strtokIndex = strtok(NULL," ");
      testActivity = atoi(strtokIndex);
    }
    switch (command) {
      case 'C':
        serialCommandC();
        break;
      
      case 'D':
//        serialCommandD();
        break;
      
      case 'E':
//        serialCommandE();
        break;

      case 'H':
//        serialCommandH();
        break;
      
      case 'M':
        serialCommandM(steps);
        break;

      case 'R':
        serialCommandR();
        break;

      case 'S':
        serialCommandS();
        break;

      case 'T':
//        serialCommandT();
        break;

      case 'V':
//        serialCommandV();
        break;

      case '?':
        Serial.println("Commands are");
        Serial.println(" C = Calibrate decoder");
        Serial.println(" D = Enable/Disable debug");
        Serial.println(" E = Erase EPROM contents");
        Serial.println(" H = Initiate Homing");
        Serial.println(" M = Move < steps activity>");
        Serial.println(" R = Reset decoder");
        Serial.println(" T = Enter/Exit sensor test mode");
        Serial.println(" V = Display the startup information");

        break;

      case 'A':
        showCVs();
        break;

      default:
        break;
    }
  }
 }

//#endif


/*
 * setup the version number
 */


void setVersion() {
  const String versionString = VERSION;
  char versionArray[versionString.length() + 1];
  versionString.toCharArray(versionArray, versionString.length() + 1);
  version = strtok(versionArray, "."); // Split version on .
  versionBuffer[0] = atoi(version);  // Major first
  version = strtok(NULL, ".");
  versionBuffer[1] = atoi(version);  // Minor next
  version = strtok(NULL, ".");
  versionBuffer[2] = atoi(version);  // Patch last
}




uint32_t getPosition(int i, bool rear)
 {
  uint32_t pos = 0;
  uint8_t s1, s2;
  if (!rear)
   {
    s1 = Dcc.getCV(CV_USER_ADDRESS + i * 4 + 12);
    s2 = Dcc.getCV(CV_USER_ADDRESS + i * 4 + 13);
   }
  else
   {
    s1 = Dcc.getCV(CV_USER_ADDRESS + i * 4 + 14);
    s2 = Dcc.getCV(CV_USER_ADDRESS + i * 4 + 15);
   }

   pos = ((uint32_t)s1 << 8) | ((uint32_t)s2);

  return pos;
 }


void getAddresses()
 {
  baseAddress = (((Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_MSB) * 256) + Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_LSB) - 1) * 4) + 1  ;

/*
  Serial.print(F("MSB = "));
  Serial.println(Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_MSB));
  Serial.print(F("LSB = "));
  Serial.println(Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_LSB));
  Serial.print(F("baseAddress = "));
  Serial.print(baseAddress);
*/

  for (int i = 0; i < MAX_TURNOUT_POSITIONS; i++)
   {
    turnoutPositions[i].dccAddress = baseAddress + i;
    turnoutPositions[i].positionFront = getPosition(i, false);
    turnoutPositions[i].positionBack = getPosition(i, true);
   }


 }


// This function is called from the Library whenever a normal DCC Turnout Packet is received
void notifyDccAccTurnoutBoard (uint16_t BoardAddr, uint8_t OutputPair, uint8_t Direction, uint8_t OutputPower)
{
  uint16_t Addr = ((BoardAddr - 1) * 4) + OutputPair + 1;

  Serial.print(F("notifyDccAccTurnoutBoard: "));
  Serial.print(Addr,DEC) ;
  Serial.print(',');
  Serial.print(Direction,DEC) ;
  Serial.print(',');
  Serial.println(OutputPower, HEX) ;

  processTurnoutCommand(Addr, Direction, OutputPower);
};


// Make FactoryDefaultCVIndex non-zero and equal to num CV's to be reset 
// to flag to the loop() function that a reset to Factory Defaults needs to be done

void notifyCVResetFactoryDefault()
 {
  FactoryDefaultCVIndex = sizeof(FactoryDefaultCVs)/sizeof(CVPair);
  Serial.print("FactoryDefaultCVIndex : ");
  Serial.println(FactoryDefaultCVIndex);
 }


#ifdef  NOTIFY_DCC_MSG
void notifyDccMsg( DCC_MSG * Msg)
{
  Serial.print("notifyDccMsg: ") ;
  for(uint8_t i = 0; i < Msg->Size; i++)
  {
    Serial.print(Msg->Data[i], HEX);
    Serial.write(' ');
  }
  Serial.println();
}
#endif





#endif
