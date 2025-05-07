/*
  RotEnc.h
*/

#ifndef __ROTENC_H_INCLUDED__
#define __ROTENC_H_INCLUDED__

#include "Arduino.h"

// macros
#define AUTO 0xff

#define PAUSE 0xff
#define CCW 0x00
#define CW  0x01

class RotEnc {
  private:
    const uint8_t pin_A; // pin number for A-phase
    const uint8_t pin_B; // pin number for B-phase

    const uint8_t direction; // direction [CCW, CW]. "CW" means B == off when A : off -> on.

    const uint8_t mode; // mode of input [INPUT, INPUT_PULLUP] for A and B

    uint8_t vol_prev, vol_curr; // [LOW, HIGH] of pin_A in prevoius and current polling
    volatile uint8_t vol_B; // [LOW, HIGH] of pin_B at a just timing of chaning pin_A voltage 

    uint32_t time_paralyze; // msec. of paralyzing for debouncing

    volatile boolean isParalyzing; // true in paralyzing for debouncing
    volatile unsigned long ms_paralyzed; // starting time of paralyzing

    boolean is_rotated_attached; // true if callback_Rotated is attached
    void(* callback_rotated)(uint8_t); // callback function for rotated (*any direction)
    
    boolean is_rotated_CCW_attached; // true if callback_RotatedInCCW is attached
    void(* callback_rotated_CCW)(void); // callback function for rotated in CCW direction
    
    boolean is_rotated_CW_attached; // true if callback_RotatedInCW is attached
    void(* callback_rotated_CW)(void); // callback function for rotated in CW direction

    boolean is_ext_interrupt_attached; // true if external interrupt for A-phase is attached

    boolean is_rotated; // one-time variable for detect rotated
    uint8_t rotated_direction; // one-time variable for rotated direction

  public:
    // constructor
    RotEnc(uint8_t pA, uint8_t pB, uint8_t dir = CW, uint8_t m = INPUT_PULLUP, uint32_t t_p = 2);

    void attachCallback_Rotated(void(* func)(uint8_t));
    void attachCallback_RotatedInCCW(void(* func)(void));
    void attachCallback_RotatedInCW(void(* func)(void));

    void attachExtInterrupt(void(* func)(void));
    void detachExtInterrupt(void);
    
    void setTimeParalyze(uint32_t);

    uint8_t getRotatedDirection(void);

    boolean isRotated(void);
    boolean isRotatedInCCW(void);
    boolean isRotatedInCW(void);
    
    void poll(void);

    void detect(void);
};

#endif
