/*
  RotEnc.cpp
*/

#include "Arduino.h"
#include "RotEnc.h"

// constructor
RotEnc::RotEnc(uint8_t pA, uint8_t pB, uint8_t dir, uint8_t m, uint8_t v, uint32_t t_p)
    :pin_A(pA),pin_B(pB),direction(dir),mode(m),value(v),time_paralyze(t_p) {

  // initialize times
  isParalyzing = false;

  // initialize callbackes
  // all callback functtions are not attached.
  is_rotated_attached = false;
  is_rotated_CCW_attached = false;
  is_rotated_CW_attached = false;

  // all one-time variables are not detected.
  is_rotated = false;
  rotated_direction = PAUSE; 

  // decide polarity and assign pin mode
  switch (value) {
    case LOW:
    case HIGH:
      vol_on = value;
      break;
    default: // AUTO
      vol_on = (mode == INPUT)?HIGH:LOW;
      break;
  }
  vol_off = !vol_on;

  // assign pins
  pinMode(pin_A, mode);
  pinMode(pin_B, mode);

  // initialize values of pin position
  vol_prev = vol_off;
}

// public member functions
// attach callback funtions
void RotEnc::attachCallback_Rotated(void(* func)(uint8_t)) {
  callback_rotated = func;
  is_rotated_attached = true;
}

void RotEnc::attachCallback_RotatedInCCW(void(* func)(void)) {
  callback_rotated_CCW = func;
  is_rotated_CCW_attached = true;
}

void RotEnc::attachCallback_RotatedInCW(void(* func)(void)) {
  callback_rotated_CW = func;
  is_rotated_CW_attached = true;
}

// setter for time constants
void RotEnc::setTimeParalyze(uint32_t t) {time_paralyze = t;}

// getter for one-time variables and status
uint8_t RotEnc::getRotatedDirection() {return rotated_direction;}

boolean RotEnc::isRotated() {return is_rotated;}
boolean RotEnc::isRotatedInCCW() {return (rotated_direction == CCW);}
boolean RotEnc::isRotatedInCW() {return (rotated_direction == CW);}


// polling rotary encoder status and callback
void RotEnc::poll() {
  // all one-time variables set to false;
  is_rotated = false;
  rotated_direction = PAUSE;

  // poll status of switch
  if (isParalyzing) { // in paralyzing
    if (millis() - ms_paralyzed > time_paralyze) { // time over paralyzing
      vol_curr = digitalRead(pin_A);
      if (vol_prev != vol_curr) { // current value of pin_A is different from before paralyzing
        if (vol_curr == vol_on) { // detect "A : off -> on"
          is_rotated = true;
          rotated_direction = (vol_B == vol_off)?direction:!direction;
          if (is_rotated_attached) callback_rotated(rotated_direction);
          switch (rotated_direction) {
            case CCW: if (is_rotated_CCW_attached) callback_rotated_CCW(); break;
            case CW : if (is_rotated_CW_attached)  callback_rotated_CW();  break;
          }
        }
        vol_prev = vol_curr;
      }
      isParalyzing = false;
    }
  } else { // not in paralyzing
    if (vol_prev != digitalRead(pin_A)) { // pin_A voltage change from previous polling
      isParalyzing = true; // move into paralyze
      ms_paralyzed = millis(); // update starting time of paralyzing
      vol_B = digitalRead(pin_B); // restore voltage on pin_B
    }
  }
}
