/*
 * @file Ttmover.h
 * 
 * A class to control the Marklin 7186 Turntable
 * 
 */

#ifndef TTMOVER_H
#define TTMOVER_H

#include <Arduino.h>
#include <AccelStepper.h>

#include "defines.h"

#define TT_MOVER_MAX_TRACKS  NUM_TRACKS + 5

enum TT_State
{  
  TT_IDLE = 0,
  TT_MOVE,
  TT_MOVING,
  TT_STOP,
};

enum LED_State
{  
  LED_STATE_OFF = 0,
  LED_STATE_ON,
  LED_STATE_SLOW,
  LED_STATE_FAST,
};



class TtMover : public AccelStepper
{
  private:
    TT_State state = TT_IDLE;
    uint16_t commandQueue[TT_MOVER_MAX_TRACKS + 1];
    uint16_t thisCommand, newCommand, lastCommand;
    byte target, track, lastTrack;
    byte direction, lastDirection;
    uint16_t Addr;

    uint16_t startMs;
    uint16_t interval = 200;                    // time between checking for change of solenoid state

    uint8_t ledState = 0;
    uint8_t _pin = LED_PIN;
    uint8_t _ledOutput = 0;
  
    uint16_t _millisStart;

    uint16_t trackOneForward;
    uint16_t trackOneBackward;
    uint16_t fullTurnSteps;
    uint16_t trackAngle;

    bool lastIsRunningState = false;

    uint16_t lastStep;
    uint16_t lastTarget;
    uint8_t homed;

    bool phaseSwitch = false;
    uint16_t phaseSwitchStartSteps = 0;
    uint16_t phaseSwitchStopSteps = 0;

    bool debug = false;

  public:
    TtMover();
    void init(uint16_t interval);
    uint16_t addCommand(uint16_t command);
    TT_State process(void);

    void processLed();

    void setDebug(bool d);

    void setTrackOne(uint8_t mf, uint8_t lf, uint8_t mb, uint8_t lb);
    void setFullTurnSteps(uint8_t m, uint8_t l);
    void setTrackAngle(uint8_t m, uint8_t l);

    uint16_t calibrate();

  private:
    void setupPins();

    void setupStepper();
    void moveToHome();

    void setPhase(bool phase);

};


#endif
