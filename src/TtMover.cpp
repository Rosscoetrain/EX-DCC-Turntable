/*
 * TtMover.cpp
 * 
 * A class to control the Marklin 7186 turntable
 * 
 */

#include "defines.h"

#include "TtMover.h"

#define TT_MOVER_SLOT_EMPTY 511


//TtMover::TtMover() : AccelStepper(AccelStepper::DRIVER, TMC2209_STEP_PIN, TMC2209_DIRECTION_PIN, 0, 0, false){};
TtMover::TtMover(uint8_t interface, uint8_t pin1, uint8_t pin2,
      uint8_t pin3, uint8_t pin4, bool enable) : AccelStepper(interface, pin1, pin2, pin3, pin4, enable){};

void TtMover::init(uint16_t interval)
 {
  this->interval = interval;
  this->state = TT_IDLE;

//  this->_stepper = stepper;

  for (int j = 0; j < TT_MOVER_MAX_TRACKS +  1; j++)
   {
    this->commandQueue[j] = TT_MOVER_SLOT_EMPTY;
   }

  this->setupPins();
  this->setupStepper();
  this->moveToHome();
  this->addCommand(1);

}

uint16_t TtMover::addCommand(uint16_t command)
 {
  if (debug)
   {
    MYSERIAL.print(" TtMover::addCommand: "); MYSERIAL.println(command,DEC);
   }

  for(uint8_t i = 0; i < TT_MOVER_MAX_TRACKS; i++)
  {
    if(this->commandQueue[i] == command)
    {
     if (debug)
      {
       MYSERIAL.print(" Already in Index: "); MYSERIAL.println(i,DEC);
      }
     return i;
    }

    else if(commandQueue[i] == TT_MOVER_SLOT_EMPTY)
    {
     if (debug)
      {
        MYSERIAL.print(" commandQueue Index: "); MYSERIAL.println(i,DEC);
      }

      this->commandQueue[i] = command;
      this->process();
      return i;
    }
  }  

  return TT_MOVER_SLOT_EMPTY;
}

/*
 * process the move commands
 */

TT_State TtMover::process(void)
 {
  switch(this->state)
   {

/*
 * check to see if there is a command then set state to move
 */

    case TT_IDLE:
      if(this->commandQueue[0] != TT_MOVER_SLOT_EMPTY)
       {
        this->thisCommand = this->commandQueue[0];

        if (debug)
         {
          MYSERIAL.print("TT_IDLE thiscommand: ");MYSERIAL.println(this->thisCommand);
         }
        this->state = TT_MOVE;
       }
      break;

/*
 * start the mover
 */

    case TT_MOVE:

      if (debug)
       {
        MYSERIAL.print("TT_MOVE thisCommand = ");MYSERIAL.println(this->thisCommand);
       }

      this->direction = this->thisCommand - (int ( this->thisCommand / 10 ) * 10);

      this->Addr = (this->thisCommand - this->direction) / 10 + 1;

      if (debug)
       {
        MYSERIAL.print("TT_MOVE Addr = ");MYSERIAL.println(this->Addr);
        MYSERIAL.print("TT_MOVE direction = ");MYSERIAL.println(this->direction);
       }


  //GoTo commands
      if (( CMD_GOTO_1_CW <= this->thisCommand ) && ( this->thisCommand <= CMD_GOTO_48_ACW ))
       {
        this->lastCommand = this->thisCommand;
        this->lastDirection = this->direction;

        if (this->debug)
         {
          MYSERIAL.print(F("Moving to "));
          MYSERIAL.print(this->direction ? F("Front") : F("Back"));
          MYSERIAL.print(F(" Position: "));
          MYSERIAL.print(this->Addr, DEC);
          MYSERIAL.print(F(" @ Step: "));
         }

        this->enableOutputs();

        uint16_t angleSteps = this->fullTurnSteps / this->numOfTracks;

        uint16_t newStep;
        if (this->direction)
         {
          newStep = this->trackOneForward + ((Addr - 1) * angleSteps);
         }
        else
         {
          newStep = this->trackOneBackward + ((Addr - 1) * angleSteps);
         }

        if (this->debug)
         {
          MYSERIAL.print(newStep, DEC);
          MYSERIAL.print(F("  Last Step: "));
          MYSERIAL.print(lastStep, DEC);
         }

        int diffStep = newStep - lastStep;
        if (this->debug)
         {
          MYSERIAL.print(F("  Diff Step: "));
          MYSERIAL.println(diffStep, DEC);
         }

#if defined ALWAYS_MOVE_POSITIVE
        if (this->debug)
         {
          MYSERIAL.print(F("  Positive"));
         }
        if(diffStep < 0)
         {
          diffStep += FULL_TURN_STEPS;
         }
        
#elif defined ALWAYS_MOVE_NEGATIVE
        if (this->debug)
         {
          MYSERIAL.print(F("  Negative"));
         }
        if(diffStep > 0)
         {
          diffStep -= FULL_TURN_STEPS;
         }
#else
        if(diffStep > HALF_TURN_STEPS)
         {
          diffStep = diffStep - FULL_TURN_STEPS;
         }
        
        else
         {
          if(diffStep < -HALF_TURN_STEPS)
           {
            diffStep = diffStep + FULL_TURN_STEPS;
           }
         }
#endif


#if PHASE_SWITCHING == AUTO
        if ((newStep >= 0 && newStep < phaseSwitchStartSteps) || (newStep <= fullTurnSteps && newStep >= phaseSwitchStopSteps))
         {
          this->phaseSwitch = false;
         }
        else
         {
          this->phaseSwitch = true;
         }
#endif
        if (this->debug)
         {
          MYSERIAL.print(F("Setting phase switch flag to: "));
          MYSERIAL.println(this->phaseSwitch);
         }
        this->setPhase(phaseSwitch);

        if (this->debug)
         {
          MYSERIAL.print(F("  Move: "));
          MYSERIAL.println(diffStep, DEC);
         }
        this->move(diffStep);
        this->lastStep = newStep;

        MYSERIAL.println(F("Ready to move"));
        MYSERIAL.print(F("targetPosition : "));
        MYSERIAL.println(this->targetPosition());

        delay(2000);

      this->state = TT_MOVING;
      this->startMs = millis();
     }

// on off commands

    if ((this->thisCommand >= CMD_LED_OFF ) && ( this->thisCommand <= CMD_EXT4_ON ))
     {
      switch (this->thisCommand)
       {
        case CMD_LED_ON:                         // 245 0 close
          this->ledState = LED_STATE_ON;
          MYSERIAL.println(F("Led on"));
          break;
        case CMD_LED_OFF:                        // 245 1 throw
          MYSERIAL.println(F("Led off"));
          this->ledState = LED_STATE_OFF;
          break;
        case CMD_LED_SLOW:                       // 246 0 close
          MYSERIAL.println(F("Led slow"));
          this->ledState = LED_STATE_SLOW;
          break;
        case CMD_LED_FAST:                       // 246 1 throw
          MYSERIAL.println(F("Led fast"));
          this->ledState = LED_STATE_FAST;
          break;
        case CMD_ACC_ON:                         // 
          digitalWrite(ACC_PIN, HIGH);
          break;
        case CMD_ACC_OFF:                         // 
          digitalWrite(ACC_PIN, LOW);
          break;
        case CMD_EXT1_ON:                         // 
          digitalWrite(EXT1_PIN, HIGH);
          break;
        case CMD_EXT1_OFF:                         // 
          digitalWrite(EXT1_PIN, LOW);
          break;
        case CMD_EXT2_ON:                         // 
          digitalWrite(EXT2_PIN, HIGH);
          break;
        case CMD_EXT2_OFF:                         // 
          digitalWrite(EXT2_PIN, LOW);
          break;
#ifndef DUAL_MOTOR
          case CMD_EXT3_ON:                         // 
          digitalWrite(EXT3_PIN, HIGH);
          break;
        case CMD_EXT3_OFF:                         // 
          digitalWrite(EXT3_PIN, LOW);
          break;
        case CMD_EXT4_ON:                         // 
          digitalWrite(EXT4_PIN, HIGH);
          break;
        case CMD_EXT4_OFF:                         // 
          digitalWrite(EXT4_PIN, LOW);
          break;
#endif
        default:
          break;
       }
      this->state = TT_STOP;
     }

    if (debug)
     {
      MYSERIAL.print(" this->state: "); MYSERIAL.println(this->state);
      MYSERIAL.print(" this->startMs: "); MYSERIAL.println(this->startMs);
     }
    break;

/*
 * continue moving until the target track is reached 
 */

    case TT_MOVING:

      if (debug)
       {
//        MYSERIAL.println("TT_MOVING: ");
//        MYSERIAL.print("interval: ");MYSERIAL.println(this->interval);
       }

      if ( ( this->thisCommand >= CMD_GOTO_1_CW ) && ( this->thisCommand <= CMD_GOTO_48_ACW ) )
       {
        if ( ( millis() - this->startMs ) > this->interval)
         {
// Process the Stepper Library
          this->run();

          if(this->isRunning() != lastIsRunningState)
           {
            lastIsRunningState = this->isRunning();
            if(!lastIsRunningState)
             {
              this->disableOutputs();
              MYSERIAL.println(F("Disable Stepper Outputs"));
              this->state = TT_STOP;
              this->startMs = millis();
             }
           }
         }
       }

      break;

/*
 * at the target track return to TT_IDLE move next command in queue to begining of queue
 */

    case TT_STOP:

      if (this->debug)
       {
        MYSERIAL.println("TT_STOP: ");
       }

      for (int i = 0; i < TT_MOVER_MAX_TRACKS; i++)
       {
        this->commandQueue[i] = this->commandQueue[i + 1];
       }
      this->commandQueue[TT_MOVER_MAX_TRACKS + 1] = TT_MOVER_SLOT_EMPTY;

      this->state = TT_IDLE;
      break;
   }
  return this->state;
 }

void TtMover::setHomeLimitPin(uint8_t home, uint8_t limit)
 {
  this->homePin = home;
  this->limitPin = limit;
 }


void TtMover::setupPins()
 {
#if (HOME_SENSOR_ACTIVE_STATE == LOW)
  pinMode(this->homePin, INPUT_PULLUP);
#else
  pinMode(this->homePin, INPUT);
#endif

#if TURNTABLE_EX_MODE == TRAVERSER
// Configure limit sensor pin in traverser mode
#if LIMIT_SENSOR_ACTIVE_STATE == LOW
//  pinMode(this->limitPin, INPUT_PULLUP);
#elif LIMIT_SENSOR_ACTIVE_STATE == HIGH
//  pinMode(this->limitPin, INPUT);
#endif
#endif

  pinMode(LED_PIN, OUTPUT);
  pinMode(ACC_PIN, OUTPUT);
  pinMode(EXT1_PIN, OUTPUT);
  pinMode(EXT2_PIN, OUTPUT);
#ifndef DUAL_MOTOR
  pinMode(EXT3_PIN, OUTPUT);
  pinMode(EXT4_PIN, OUTPUT);
#endif
  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(ACC_PIN, LOW);
  digitalWrite(EXT1_PIN, LOW);
  digitalWrite(EXT2_PIN, LOW);
#ifndef DUAL_MOTOR
  digitalWrite(EXT3_PIN, LOW);
  digitalWrite(EXT4_PIN, LOW);
#endif
// setPhase and RELAY_PIN to off
//  setPhase(0);

 }


void TtMover::setInvert(bool d = false, bool s = false, bool e = false)
 {
  this->dirInvert = d;
  this->stepInvert = s;
  this->enableInvert = e;

 }


void TtMover::setupStepper()
 {
  this->setEnablePin(TMC2209_ENABLE_PIN);
  this->setPinsInverted(this->dirInvert, this->stepInvert, this-enableInvert);

  this->enableOutputs();

  this->setMaxSpeed(STEPPER_MAX_SPEED);
  this->setAcceleration(STEPPER_ACCELERATION);

 }

void TtMover::moveToHome()
 {
  this->enableOutputs();
  if (this->debug)
   {
    MYSERIAL.println("Finding home ...");
    MYSERIAL.print("Home sensor : ");
    MYSERIAL.println(digitalRead(this->homePin));
   }


  if (digitalRead(this->homePin) == HOME_SENSOR_ACTIVE_STATE)
   {
    MYSERIAL.println("Move off home");
    this->moveTo(HOME_SENSITIVITY);
    while (this->isRunning())
     {
      this->run();
     }
   }
  

  if (digitalRead(this->homePin) != HOME_SENSOR_ACTIVE_STATE)
   {

    MYSERIAL.print(F("Homing "));
#if TURNTABLE_EX_MODE == TURNTABLE
    MYSERIAL.print(F("turntable"));
    this->moveTo(SANITY_STEPS);
#elif TURNTABLE_EX_MODE == TRAVERSER
    MYSERIAL.print(F("traverser"));
    this->moveTo(-SANITY_STEPS);
#endif
    MYSERIAL.println(F(" ..."));
   }

  while (digitalRead(this->homePin) != HOME_SENSOR_ACTIVE_STATE)
    this->run();

  this->disableOutputs();
  this->lastStep = 0;
  this->setCurrentPosition(0);

  if (debug)
   {
    MYSERIAL.print(F("lastStep : "));
    MYSERIAL.println(lastStep);
   }
 }


void TtMover::processLed()

 {
  if (this->ledState == LED_STATE_OFF)
   {
    digitalWrite(_pin, LOW);
    return;
   }
  if (this->ledState == LED_STATE_ON)
   {
    digitalWrite(_pin, HIGH);
    return;
   }

  uint16_t currentMillis = millis();
  if ((this->ledState == LED_STATE_SLOW) && (currentMillis - _millisStart >= LED_SLOW))
   {
    _ledOutput = !_ledOutput;
    _millisStart = currentMillis;
   }
  
  if ((this->ledState == LED_STATE_FAST) && (currentMillis - _millisStart >= LED_FAST))
   {
    _ledOutput = !_ledOutput;
    _millisStart = currentMillis;
   }
  digitalWrite(_pin, _ledOutput);

 }


void TtMover::setDebug(bool d)
 {
  this->debug = d;
 }


void TtMover::setTrackOne(uint8_t mf, uint8_t lf, uint8_t mb, uint8_t lb)
 {
   this->trackOneForward = mf * 256 + lf;
   this->trackOneBackward = mb * 256 + lb;
 }

void TtMover::setFullTurnSteps(uint8_t m, uint8_t l)
 {
  this->fullTurnSteps = m * 256 + l;

  this->phaseSwitchStartSteps = this->fullTurnSteps / 360 * PHASE_SWITCH_ANGLE;
  this->phaseSwitchStopSteps = this->fullTurnSteps / 360 * (PHASE_SWITCH_ANGLE + 180);

  if (debug)
   {
    MYSERIAL.print(F("fullTurnSteps : "));
    MYSERIAL.println(this->fullTurnSteps);
    MYSERIAL.print(F("phaseSwitchStartSteps : "));
    MYSERIAL.println(this->phaseSwitchStartSteps);
    MYSERIAL.print(F("phaseSwitchStopSteps : "));
    MYSERIAL.println(this->phaseSwitchStopSteps);
   }
 }

uint16_t TtMover::getFullTurnSteps()
 {
  return this->fullTurnSteps;
 }

#if TURNTABLE_EX_MODE == TRAVERSER
void TtMover::setNumOfTracks(uint8_t m)
   {
    this->numOfTracks = m;
   }
#endif



void TtMover::setTrackAngle(uint8_t m, uint8_t l)
 {
#if TURNTABLE_EX_MODE == TURNTABLE
  this->trackAngle = m * 256 + l;
  this->numOfTracks = 360 * 10 / this->trackAngle;
#elif TURNTABLE_EX_MODE == TRAVERSER
  this->trackAngle = 0;
#endif

  if (this->debug)
   {
    MYSERIAL.print(F("Number of tracks : "));
    MYSERIAL.println(this->numOfTracks);
   }
 }


void TtMover::setPhase(bool phase)
 {
  #if RELAY_ACTIVE_STATE == HIGH
  digitalWrite(RELAY_PIN, phase);
#elif RELAY_ACTIVE_STATE == LOW
  digitalWrite(RELAY_PIN, !phase);
#endif
 }


// The calibration function is used to determine the number of steps required for a single 360 degree rotation,
// or, in traverser mode, the steps between the home and limit switches.
// This should only be trigged when either there are no stored steps in EEPROM, the stored steps are invalid,
// or the calibration command has been initiated by the CommandStation.
// Logic:
// - Move away from home if already homed and erase EEPROM.
// - Perform initial home rotation, set to 0 steps when homed.
// - Perform second home rotation, set steps to currentPosition().
// - Write steps to EEPROM.

uint16_t TtMover::calibrate()
 {
  this->setPhase(0);

  uint16_t tempSteps = 0;

  bool homed = false;

  this->enableOutputs();

#if TURNTABLE_EX_MODE == TURNTABLE
  if (digitalRead(this->homePin) == HOME_SENSOR_ACTIVE_STATE)
   {
    MYSERIAL.println(F("Homed"));
    homed = true;
    this->setCurrentPosition(0);
   }

  if (!homed)
   {
    if (digitalRead(this->homePin) != HOME_SENSOR_ACTIVE_STATE)
     {
      MYSERIAL.println(F("Homing ..."));
     	this->moveTo(SANITY_STEPS);
     }

    while ((digitalRead(this->homePin) != HOME_SENSOR_ACTIVE_STATE) && (this->currentPosition() <= SANITY_STEPS))
     {
      this->run();
     }

    if ((digitalRead(this->homePin) != HOME_SENSOR_ACTIVE_STATE) && (this->currentPosition() >= SANITY_STEPS))
     {
      MYSERIAL.println(F("Calibration incomplete"));
      MYSERIAL.println(F("Home sensor not found"));
      MYSERIAL.println(F("Full revolution steps unchanged"));
      this->disableOutputs();
      this->setCurrentPosition(0);
      return 0;
     }

    if (digitalRead(this->homePin) == HOME_SENSOR_ACTIVE_STATE)
     {
      MYSERIAL.println(F("Homed"));
      homed = true;
      this->setCurrentPosition(0);
     }
   }

  MYSERIAL.println(F("Counting full steps"));

  this->moveTo(HOME_SENSITIVITY);
  while(digitalRead(this->homePin) == HOME_SENSOR_ACTIVE_STATE)
   {
    this->run();
   }

 	this->moveTo(SANITY_STEPS);
  while ((digitalRead(this->homePin) != HOME_SENSOR_ACTIVE_STATE) && (this->currentPosition() <= SANITY_STEPS))
   {
    this->run();
    tempSteps = this->currentPosition();
    if (debug)
     {
      MYSERIAL.print(".");
     }
   }

  if (digitalRead(this->homePin) == HOME_SENSOR_ACTIVE_STATE)
   {
    MYSERIAL.println(F("Calibration complete"));
    MYSERIAL.print(F("Full revolution steps : "));
    MYSERIAL.println(tempSteps);
    homed = true;
    this->disableOutputs();
    this->setCurrentPosition(0);
   }
  else
   {
    MYSERIAL.println(F("Calibration incomplete"));
    MYSERIAL.println(F("Home sensor not found"));
    MYSERIAL.println(F("Full revolution steps unchanged"));
    homed = false;
    tempSteps = 0;
    this->disableOutputs();
    this->setCurrentPosition(0);
   }

  return tempSteps;
#endif

#if TURNTABLE_EX_MODE == TRAVERSER
  if (digitalRead(this->homePin) == HOME_SENSOR_ACTIVE_STATE)
   {
    MYSERIAL.println(F("Homed"));
    homed = true;
    this->setCurrentPosition(0);
   }

  if (!homed)
   {
    if (digitalRead(this->homePin) != HOME_SENSOR_ACTIVE_STATE)
     {
      MYSERIAL.println(F("Homing ..."));
     	this->moveTo(-SANITY_STEPS);
     }

    while ((digitalRead(this->homePin) != HOME_SENSOR_ACTIVE_STATE) && (this->currentPosition() >= -SANITY_STEPS))
     {
      this->run();
     }

    if ((digitalRead(this->homePin) != HOME_SENSOR_ACTIVE_STATE) && (this->currentPosition() >= -SANITY_STEPS))
     {
      MYSERIAL.println(F("Calibration incomplete"));
      MYSERIAL.println(F("Home sensor not found"));
      MYSERIAL.println(F("Full revolution steps unchanged"));
      this->disableOutputs();
      this->setCurrentPosition(0);
      return 0;
     }

    if (digitalRead(this->homePin) == HOME_SENSOR_ACTIVE_STATE)
     {
      MYSERIAL.println(F("Homed"));
      homed = true;
      this->setCurrentPosition(0);
     }
   }

  MYSERIAL.println(F("Counting full steps"));

  this->moveTo(HOME_SENSITIVITY);
  while(digitalRead(this->homePin) == HOME_SENSOR_ACTIVE_STATE)
   {
    this->run();
   }

 	this->moveTo(SANITY_STEPS);
  while ((digitalRead(LIMIT_SENSOR_PIN) != LIMIT_SENSOR_ACTIVE_STATE) && (this->currentPosition() <= SANITY_STEPS))
   {
    this->run();
    tempSteps = this->currentPosition();
    if (debug)
     {
      MYSERIAL.print(".");
     }
   }

  if (digitalRead(LIMIT_SENSOR_PIN) == LIMIT_SENSOR_ACTIVE_STATE)
   {
    MYSERIAL.println(F("Calibration complete"));
    MYSERIAL.print(F("Full steps : "));
    MYSERIAL.println(tempSteps);
    homed = true;
    this->disableOutputs();
    this->setCurrentPosition(0);
   }
  else
   {
    MYSERIAL.println(F("Calibration incomplete"));
    MYSERIAL.println(F("Limit sensor not found"));
    MYSERIAL.println(F("Full steps unchanged"));
    homed = false;
    tempSteps = 0;
    this->disableOutputs();
    this->setCurrentPosition(0);
   }

  return tempSteps;

#endif

 }

