/*
 *
 * @file functions.h
 *
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "defines.h"
#include "Led.h"


// Function to set phase.
void setPhase(uint8_t phase) {
#if RELAY_ACTIVE_STATE == HIGH
  digitalWrite(RELAY_PIN, phase);
#elif RELAY_ACTIVE_STATE == LOW
  digitalWrite(RELAY_PIN, !phase);
#endif
}


void processTurnoutCommand(uint16_t Addr, uint8_t Direction, uint8_t OutputPower)
{
  if (debug)
   {
    MYSERIAL.print(F("processTurnoutCommand: "));
    MYSERIAL.print(F(" DCC address :"));
    MYSERIAL.println(Addr);
   }

  for (uint8_t i = 0; i < MAX_TURNOUT_POSITIONS ; i++)
  {
    if ((Addr == turnoutPositions[i].dccAddress) && ((Addr != lastAddr) || (Direction != lastDirection)) && OutputPower)
    {
      lastAddr = Addr ;
      lastDirection = Direction ;

      if (debug)
       {
        MYSERIAL.print(F("Moving to "));
        MYSERIAL.print(Direction ? F("Front") : F("Back"));
        MYSERIAL.print(F(" Position: "));
        MYSERIAL.print(i, DEC);
        MYSERIAL.print(F(" @ Step: "));
       }

#ifdef TMC2209_ENABLE_PIN
      stepper.enableOutputs();
#endif

      int newStep;
      if(Direction)
        newStep = turnoutPositions[i].positionFront;
      else
        newStep = turnoutPositions[i].positionBack;
      if (debug)
       {
        MYSERIAL.print(newStep, DEC);
        MYSERIAL.print(F("  Last Step: "));
        MYSERIAL.print(lastStep, DEC);
       }

      int diffStep = newStep - lastStep;
      if (debug)
       {
        MYSERIAL.print(F("  Diff Step: "));
        MYSERIAL.print(diffStep, DEC);
       }

#if defined ALWAYS_MOVE_POSITIVE
      if (debug)
       {
        MYSERIAL.print(F("  Positive"));
       }
      if(diffStep < 0)
        diffStep += FULL_TURN_STEPS;
        
#elif defined ALWAYS_MOVE_NEGATIVE
      if (debug)
       {
        MYSERIAL.print(F("  Negative"));
       }
      if(diffStep > 0)
        diffStep -= FULL_TURN_STEPS;
#else
      if(diffStep > HALF_TURN_STEPS)
        diffStep = diffStep - FULL_TURN_STEPS;
        
      else if(diffStep < -HALF_TURN_STEPS)
        diffStep = diffStep + FULL_TURN_STEPS;
#endif


#if PHASE_SWITCHING == AUTO
    if ((newStep >= 0 && newStep < phaseSwitchStartSteps) || (newStep <= fullTurnSteps && newStep >= phaseSwitchStopSteps))
     {
      phaseSwitch = 0;
     }
    else
     {
      phaseSwitch = 1;
     }
#endif
    if (debug)
     {
      Serial.print(F("Setting phase switch flag to: "));
      Serial.println(phaseSwitch);
     }
    setPhase(phaseSwitch);



      if (debug)
       {
        MYSERIAL.print(F("  Move: "));
        MYSERIAL.println(diffStep, DEC);
       }
      stepper.move(diffStep);

      lastStep = newStep;
      break;
    }
  }

//  uint16_t addr = turnoutPositions[MAX_TURNOUT_POSITIONS - 1].dccAddress;
  uint16_t addr = baseAddress + MAX_TURNOUT_POSITIONS;

  if ((Addr >= addr) && (Addr < addr + EXTRA_DCC_ADDRESSES))
   {
    if (debug)
     {
      MYSERIAL.print(F("Addr : "));
      MYSERIAL.println(Addr);
     }

// this is for led, acc and ext processing.
    switch (Addr - addr)
    {
    case 1:
      /* led off */
      Led.state(LED_STATE_OFF);
      break;
    
    case 2:
      /* led on */
      Led.state(LED_STATE_ON);
      break;
    
    case 3:
      /* led slow flash */
      Led.state(LED_STATE_SLOW);
      break;
    
    case 4:
      /* led fast flash */
      Led.state(LED_STATE_FAST);
      break;
    
    case 5:
      /* acc on */
      digitalWrite(ACC_PIN, HIGH);
      break;
    
    case 6:
      /* acc off */
      digitalWrite(ACC_PIN, LOW);
      break;
    
    case 7:
      /* ext1 on */
      digitalWrite(EXT1_PIN, HIGH);
      break;
    
    case 8:
      /* ext1 off */
      digitalWrite(EXT1_PIN, LOW);
      break;
    
    case 9:
      /* ext2 on */
      digitalWrite(EXT2_PIN, HIGH);
      break;
    
    case 10:
      /* ext2 off */
      digitalWrite(EXT2_PIN, LOW);
      break;
    
    case 11:
      /* ext3 on */
      digitalWrite(EXT3_PIN, HIGH);
      break;
    
    case 12:
      /* ext3 off */
      digitalWrite(EXT3_PIN, LOW);
      break;
    
    case 13:
      /* ext4 on */
      digitalWrite(EXT4_PIN, HIGH);
      break;
    
    case 14:
      /* ext4 off */
      digitalWrite(EXT4_PIN, LOW);
      break;
    
    default:
      break;
    }
   }
}

void setupPins()
 {
//  pinMode(LED_PIN, OUTPUT);
  Led.init(LED_PIN);
  pinMode(ACC_PIN, OUTPUT);
  pinMode(EXT1_PIN, OUTPUT);
  pinMode(EXT2_PIN, OUTPUT);
  pinMode(EXT3_PIN, OUTPUT);
  pinMode(EXT4_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

//  digitalWrite(LED_PIN, LOW);
  Led.state(LED_STATE_OFF);
  digitalWrite(ACC_PIN, LOW);
  digitalWrite(EXT1_PIN, LOW);
  digitalWrite(EXT2_PIN, LOW);
  digitalWrite(EXT3_PIN, LOW);
  digitalWrite(EXT4_PIN, LOW);

// setPhase and RELAY_PIN to off
  setPhase(0);

#if HOME_SENSOR_ACTIVE_STATE == LOW
  pinMode(HOME_SENSOR_PIN, INPUT_PULLUP);
#elif HOME_SENSOR_ACTIVE_STATE == HIGH
  pinMode(HOME_SENSOR_PIN, INPUT);
#endif

#if TURNTABLE_EX_MODE == TRAVERSER
// Configure limit sensor pin in traverser mode
#if LIMIT_SENSOR_ACTIVE_STATE == LOW
  pinMode(LIMIT_SENSOR_PIN, INPUT_PULLUP);
#elif LIMIT_SENSOR_ACTIVE_STATE == HIGH
  pinMode(LIMIT_SENSOR_PIN, INPUT);
#endif
#endif


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
  MYSERIAL.println(F("Finding Home Sensor...."));

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
    MYSERIAL.println(F("Found Home Position - Setting Current Position to 0"));
    return true;
  }
  else
    MYSERIAL.println(F("Home Position NOT FOUND - Check Sensor Hardware"));

  return false;  
}

void setupDCCDecoder()
{
  MYSERIAL.println(F("Setting up DCC Decorder..."));

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



void(* resetFunc) (void) = 0;                     // declare reset function at address 0


// R soft reset
void serialCommandR()
 {
  MYSERIAL.println("Resetting");
  delay(2000);
  resetFunc();
 }


// M command to move
void serialCommandM(long steps) {
  if (stepper.isRunning()) {
    MYSERIAL.println(F("Stepper is running, ignoring <M>"));
    return;
  }
  if (steps < 0) {
    MYSERIAL.println(F("Cannot provide a negative step count"));
  } else if (steps > 32767) {
    MYSERIAL.println(F("Step count too large, refer to the documentation for large step counts > 32767"));
  } else {
    MYSERIAL.print(F("Test move "));
    MYSERIAL.print(steps);
    MYSERIAL.print(F(" steps, activity ID "));
    MYSERIAL.println(testActivity);
//    testStepsMSB = steps >> 8;
//    testStepsLSB = steps & 0xFF;
//    testCommandSent = true;
//    receiveEvent(3);

    uint8_t addr = (steps / (FULL_TURN_STEPS/24)) + POSITION_01_DCC_ADDRESS;

    MYSERIAL.print(F(" addr "));
    MYSERIAL.println(addr);

    processTurnoutCommand(addr, 1, 1);
  }
}

// reset to factory default CVs
void serialCommandS()
 {
  MYSERIAL.println(F("Reset factory default CVs"));
//  Dcc.setCV(8, 8);
  notifyCVResetFactoryDefault();
 }


// calibrate turntable
void serialCommandC()
 {
  if (stepper.isRunning())
   {
    MYSERIAL.println(F("Stepper is running, ignoring <C>"));
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


// toggle debug
void serialCommandD()
 {
  debug = !debug;
  MYSERIAL.println("Debug changed");
 }


void serialCommandE()
 {
  MYSERIAL.println("Erasing EEPROM...");
  
  // Loop through all EEPROM addresses and write 0 to each
  for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0xFF);
  }
  
  MYSERIAL.println("EEPROM erased.");
  // You can add a delay or turn on an LED to signal completion
//  pinMode(13, OUTPUT);
//  digitalWrite(13, HIGH); // LED on when done

  for (int i = 0; i < EEPROM.length(); i++) {
    MYSERIAL.print("i : ");
    MYSERIAL.print(i);
    MYSERIAL.print(" : 0x");
    MYSERIAL.println(EEPROM.read(i), HEX);
   }
 }


/*
void serialCommand()
 {

 }
*/


void showCVs()
 {
  MYSERIAL.println(F("CVs are:"));
  MYSERIAL.print(F("CV"));
  MYSERIAL.print(CV_ACCESSORY_DECODER_ADDRESS_LSB);
  MYSERIAL.print(F(" = "));
  MYSERIAL.println(Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_LSB));
  MYSERIAL.print(F("CV"));
  MYSERIAL.print(CV_ACCESSORY_DECODER_ADDRESS_MSB);
  MYSERIAL.print(F(" = "));
  MYSERIAL.println(Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_MSB));
  MYSERIAL.print(F("CV"));
  MYSERIAL.print(8);
  MYSERIAL.print(F(" = "));
  MYSERIAL.println(Dcc.getCV(8));
  MYSERIAL.print(F("CV"));
  MYSERIAL.print(CV_29_CONFIG);
  MYSERIAL.print(F(" = "));
  MYSERIAL.println(Dcc.getCV(CV_29_CONFIG));

  for (int i = 0; i < 12; i++)
   {
    MYSERIAL.print(F("CV : "));
    MYSERIAL.print(CV_USER_ADDRESS + i);
    MYSERIAL.print(F(" = "));
    MYSERIAL.println(Dcc.getCV(CV_USER_ADDRESS + i));
   }

  for (int i = 0; i < MAX_TURNOUT_POSITIONS; i++)
   {
    MYSERIAL.print(F("CV : "));
    MYSERIAL.print(CV_USER_ADDRESS + (4 * i) + 12);
    MYSERIAL.print(F(" = "));
    MYSERIAL.print(Dcc.getCV(CV_USER_ADDRESS + (4 * i) + 12));
    MYSERIAL.print(F(" CV : "));
    MYSERIAL.print(CV_USER_ADDRESS + (4 * i) + 13);
    MYSERIAL.print(F(" = "));
    MYSERIAL.print(Dcc.getCV(CV_USER_ADDRESS + (4 * i) + 13));
    MYSERIAL.print(F(" CV : "));
    MYSERIAL.print(CV_USER_ADDRESS + (4 * i) + 14);
    MYSERIAL.print(F(" = "));
    MYSERIAL.print(Dcc.getCV(CV_USER_ADDRESS + (4 * i) + 14));
    MYSERIAL.print(F(" CV : "));
    MYSERIAL.print(CV_USER_ADDRESS + (4 * i) + 15);
    MYSERIAL.print(F(" = "));
    MYSERIAL.println(Dcc.getCV(CV_USER_ADDRESS + (4 * i) + 15));
   }

 }


void doSerialCommand()
 {
  static bool serialInProgress = false;
  static byte serialIndex = 0;
  char startMarker = '<';
  char endMarker = '>';
  char serialChar;
  while (MYSERIAL.available() > 0 && newSerialData == false) {
    serialChar = MYSERIAL.read();
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
        serialCommandD();
        break;
      
      case 'E':
        serialCommandE();
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
        MYSERIAL.println("Commands are");
//        MYSERIAL.println(" C = Calibrate decoder");
        MYSERIAL.println(" D = Enable/Disable debug");
//        MYSERIAL.println(" E = Erase EPROM contents");
//        MYSERIAL.println(" H = Initiate Homing");
        MYSERIAL.println(" M = Move < steps activity>");
        MYSERIAL.println(" R = Reset decoder");
//        MYSERIAL.println(" T = Enter/Exit sensor test mode");
//        MYSERIAL.println(" V = Display the startup information");
        MYSERIAL.println(" A = Show CVs");
        MYSERIAL.println(" S = Reset CVs to default");

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
  baseAddress = (Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_MSB) * 256) + Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_LSB);
/*
  MYSERIAL.print(F("MSB = "));
  MYSERIAL.println(Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_MSB));
  MYSERIAL.print(F("LSB = "));
  MYSERIAL.println(Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_LSB));
*/
  MYSERIAL.print(F("baseAddress = "));
  MYSERIAL.println(baseAddress);

  for (int i = 0; i < MAX_TURNOUT_POSITIONS; i++)
   {
    turnoutPositions[i].dccAddress = baseAddress + i;
    turnoutPositions[i].positionFront = getPosition(i, false);
    turnoutPositions[i].positionBack = getPosition(i, true);
   }

  fullTurnSteps = (Dcc.getCV(CV_USER_ADDRESS + 2) * 256) + Dcc.getCV(CV_USER_ADDRESS + 1);
  phaseSwitchStartSteps = fullTurnSteps * 45 / 360;
  phaseSwitchStopSteps = fullTurnSteps - (fullTurnSteps * 45 / 360);

/*
  MYSERIAL.print(F("fullTurnSteps : "));
  MYSERIAL.println(fullTurnSteps);
  MYSERIAL.print(F("phaseSwitchStartSteps : "));
  MYSERIAL.println(phaseSwitchStartSteps);
  MYSERIAL.print(F("phaseSwithchStopSteps : "));
  MYSERIAL.println(phaseSwitchStopSteps);
*/
 }


// This function is called from the Library whenever a normal DCC Turnout Packet is received
void notifyDccAccTurnoutBoard (uint16_t BoardAddr, uint8_t OutputPair, uint8_t Direction, uint8_t OutputPower)
{
  uint16_t Addr = ((BoardAddr - 1) * 4) + OutputPair + 1;
  if (debug)
   {
    MYSERIAL.print(F("notifyDccAccTurnoutBoard: "));
    MYSERIAL.print(Addr,DEC) ;
    MYSERIAL.print(',');
    MYSERIAL.print(Direction,DEC) ;
    MYSERIAL.print(',');
    MYSERIAL.println(OutputPower, HEX) ;
   }
  processTurnoutCommand(Addr, Direction, OutputPower);
};


// Make FactoryDefaultCVIndex non-zero and equal to num CV's to be reset 
// to flag to the loop() function that a reset to Factory Defaults needs to be done

void notifyCVResetFactoryDefault()
 {
  FactoryDefaultCVIndex = sizeof(FactoryDefaultCVs)/sizeof(CVPair);
  MYSERIAL.print("FactoryDefaultCVIndex : ");
  MYSERIAL.println(FactoryDefaultCVIndex);
 }

void notifyCVChange(uint16_t CV, uint8_t Value)
 {
  if (debug)
   {
    MYSERIAL.print("notifyCVChange: CV: ") ;
    MYSERIAL.print(CV,DEC) ;
    MYSERIAL.print(" Value: ") ;
    MYSERIAL.println(Value, DEC) ;
   }
  Value = Value;  // Silence Compiler Warnings...

  if ((CV == CV_ACCESSORY_DECODER_ADDRESS_MSB) || (CV == CV_ACCESSORY_DECODER_ADDRESS_LSB))
   {
    MYSERIAL.println("resetting decoder");
    delay(1000);
    resetFunc();
   }
		
 }



#ifdef  NOTIFY_DCC_MSG
void notifyDccMsg( DCC_MSG * Msg)
{
  MYSERIAL.print("notifyDccMsg: ") ;
  for(uint8_t i = 0; i < Msg->Size; i++)
  {
    MYSERIAL.print(Msg->Data[i], HEX);
    MYSERIAL.write(' ');
  }
  MYSERIAL.println();
}
#endif

#endif
