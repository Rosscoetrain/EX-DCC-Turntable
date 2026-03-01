/*
 * @file Ttmover.h
 * 
 * A class to control a DCC Turntable
 * 
 */

#ifndef TTMOVER_H
#define TTMOVER_H

#include <Arduino.h>
#include <AccelStepper.h>
#include <NmraDcc.h>

#include "defines.h"
#include "variables.h"

#include "Led.h"
#include "TtMover_defines.h"


enum TT_State
{  
  TT_IDLE = 0,
  TT_MOVE,
  TT_MOVING,
  TT_STOP,
};

struct TtMoverCommand
{
  uint16_t Addr;
  uint8_t Direction;
  uint8_t OutputPower;
};


class TtMover
{
  private:
    TT_State state = TT_IDLE;

    TtMoverCommand commandQueue[TT_MOVER_MAX_TRACKS + 1];
    TtMoverCommand thisCommand, newCommand;
    byte target, track, lastTrack;
    byte direction;
    String CommandName;

    uint16_t startMs;
    uint16_t interval = 200;                    // time between checking for change of solenoid state

    AccelStepper *stepper;
//    TurnoutPosition *turnoutpositions;
    LED *led;


  public:
    void init(uint16_t interval, AccelStepper *stp, LED *Led);
    uint16_t addCommand(uint16_t Addr, uint8_t Direction, uint8_t OutputPower);
    TT_State process(void);
//    void initTurnoutPositions(TurnoutPosition *tp);

  private:
    void processTurnoutCommand(uint16_t Addr, uint8_t Direction, uint8_t OutputPower);
    void setPhase(uint8_t phase);

};


#endif
