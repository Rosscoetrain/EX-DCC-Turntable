/*
 *
 * @file Led.h
 * 
 */

#ifndef LED_H
#define LED_H

#include <Arduino.h>

enum LED_State
{  
  LED_STATE_OFF = 0,
  LED_STATE_ON,
  LED_STATE_SLOW,
  LED_STATE_FAST,
};

class LED
{
  private:

  LED_State _state;
  uint8_t _pin;
  uint8_t _ledOutput = 0;
  
  uint16_t _millisStart;


  public:


  void init(uint8_t pin);
  void state(LED_State state);
  void process();


};

#endif
