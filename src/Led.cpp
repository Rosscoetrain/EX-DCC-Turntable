/*
 *
 * Led.cpp 2026 Ross Scanlon 
 * 
 * class to process a led on output pin
 * 
 * call .process in main loop
 *
 */
#include <Arduino.h>

#include "Led.h"

#include "defines.h"

// 
void LED::init(uint8_t pin)
 {
  _pin = pin;
  pinMode(_pin, OUTPUT);
 }


// this processes the led depending on state set by Led::state

void LED::process()
 {
  if (_state == LED_STATE_OFF)
   {
    digitalWrite(_pin, LOW);
    return;
   }
  if (_state == LED_STATE_ON)
   {
    digitalWrite(_pin, HIGH);
    return;
   }
  uint16_t currentMillis = millis();
  if ((_state == LED_STATE_SLOW) && (currentMillis - _millisStart >= LED_SLOW))
   {
    _ledOutput = !_ledOutput;
    _millisStart = currentMillis;
   }
  
  if ((_state == LED_STATE_FAST) && (currentMillis - _millisStart >= LED_FAST))
   {
    _ledOutput = !_ledOutput;
    _millisStart = currentMillis;
   }

  digitalWrite(_pin, _ledOutput);
 }




void LED::state(LED_State state)
 {
  _state = state;
  if ((_state == LED_STATE_FAST) || (_state == LED_STATE_SLOW))
   {
    _millisStart = 0;  // millis();
   }
//  MYSERIAL.print(F("set Led State : "));
//  MYSERIAL.println(_state);
 }

