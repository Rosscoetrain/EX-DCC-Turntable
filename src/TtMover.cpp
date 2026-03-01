/*
 * TtMover.cpp
 * 
 * A class to control a DCC turntable
 * 
 */

//#include "defines.h"
//#include "variables.h"

#include "TtMover.h"


#define TT_MOVER_SLOT_EMPTY 511

void TtMover::init(uint16_t interval, AccelStepper& stp, LED& Led)
 {
  this->interval = interval;
  this->state = TT_IDLE;
  this->_stepper = stp;
//  this->turnoutpositions = tp;
  this->_led = Led;

  for (int j = 0; j < TT_MOVER_MAX_TRACKS +  1; j++)
   {
    this->commandQueue[j].Addr = TT_MOVER_SLOT_EMPTY;
   }

}

/*
void TtMover::initTurnoutPositions(TurnoutPosition *tp)
 {
  this->turnoutpositions = tp;
 }
*/


uint16_t TtMover::addCommand(uint16_t Addr, uint8_t Direction, uint8_t OutputPower)
{

  for(uint8_t i = 0; i < TT_MOVER_MAX_TRACKS; i++)
  {
    if(this->commandQueue[i].Addr == Addr)
    {
      return i;
    }

    else if(commandQueue[i].Addr == TT_MOVER_SLOT_EMPTY)
    {
      this->commandQueue[i].Addr = Addr;
      this->commandQueue[i].Direction = Direction;
      this->commandQueue[i].OutputPower = OutputPower;

      this->process();
      return i;
    }
  }  

  return TT_MOVER_SLOT_EMPTY;
}

TT_State TtMover::process(void)
{
  switch(this->state)
  {

/*
 * 
 */

    case TT_IDLE:                             // check to see if there is a command then set state to move
    if(this->commandQueue[0].Addr != TT_MOVER_SLOT_EMPTY)
    {
      this->thisCommand = this->commandQueue[0];

      this->state = TT_MOVE;
    }
    
    break;

/*
 * 
 */

    case TT_MOVE:                             // start the move

      processTurnoutCommand(this->thisCommand.Addr, this->thisCommand.Direction, this->thisCommand.OutputPower);

      this->state = TT_MOVING;
    break;

/*
 * 
 */

    case TT_MOVING:                           // continue moving until the target track is reached
      if (this->_stepper.isRunning())
       {
        this->state = TT_MOVING;
       }
      else
       {
        this->state = TT_IDLE;
       }
    break;

/*
 * 
 */

    case TT_STOP:                             // at the target track turn of the solenoid and return to TT_IDLE move next command in queue to begining of queue

      this->state = TT_IDLE;
    break;
  }

return this->state;

}


void TtMover::processTurnoutCommand(uint16_t Addr, uint8_t Direction, uint8_t OutputPower)
 {

  for (uint8_t i = 0; i < MAX_TURNOUT_POSITIONS ; i++)
  {
    if ((Addr == turnoutPositions[i].dccAddress) && ((Addr != lastAddr) || (Direction != lastDirection)) && OutputPower)
    {
      lastAddr = Addr ;
      lastDirection = Direction ;

/*
      if (debug)
       {
        MYSERIAL.print(F("Moving to "));
        MYSERIAL.print(Direction ? F("Front") : F("Back"));
        MYSERIAL.print(F(" Position: "));
        MYSERIAL.print(i, DEC);
        MYSERIAL.print(F(" @ Step: "));
       }
*/

#ifdef TMC2209_ENABLE_PIN
      this->_stepper.enableOutputs();
#endif

      int newStep;
      if(Direction)
        newStep = turnoutPositions[i].positionFront;
      else
        newStep = turnoutPositions[i].positionBack;
/*
      if (debug)
       {
        MYSERIAL.print(newStep, DEC);
        MYSERIAL.print(F("  Last Step: "));
        MYSERIAL.print(lastStep, DEC);
       }
*/
      int diffStep = newStep - lastStep;
/*
      if (debug)
       {
        MYSERIAL.print(F("  Diff Step: "));
        MYSERIAL.print(diffStep, DEC);
       }
*/

#if defined ALWAYS_MOVE_POSITIVE
/*
      if (debug)
       {
        MYSERIAL.print(F("  Positive"));
       }
*/
      if(diffStep < 0)
        diffStep += FULL_TURN_STEPS;
        
#elif defined ALWAYS_MOVE_NEGATIVE
/*
      if (debug)
       {
        MYSERIAL.print(F("  Negative"));
       }
*/
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

/*
    if (debug)
     {
      Serial.print(F("Setting phase switch flag to: "));
      Serial.println(phaseSwitch);
     }
*/

    setPhase(phaseSwitch);

/*
      if (debug)
       {
        MYSERIAL.print(F("  Move: "));
        MYSERIAL.println(diffStep, DEC);
       }
*/

      this->_stepper.move(diffStep);

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


// Function to set phase.
void TtMover::setPhase(uint8_t phase) {
#if RELAY_ACTIVE_STATE == HIGH
  digitalWrite(RELAY_PIN, phase);
#elif RELAY_ACTIVE_STATE == LOW
  digitalWrite(RELAY_PIN, !phase);
#endif
}


