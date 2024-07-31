/*
  demo_RotEnc_external_interrupt.ino
*/

#include <RotEnc.h>

// this is for Arduino Leonardo. pin 7 is assigned to an external interrupt (INT.4)
RotEnc myRE(7, 8);

void setup() {
  Serial.begin(9600);
  while (!Serial) { }
  delay(500);
  Serial.println("*** demo_RotEnc_external_interrupt ***");
  
  myRE.attachCallback_RotatedInCCW(on_rotenc_rotated_ccw);
  myRE.attachCallback_RotatedInCW(on_rotenc_rotated_cw);

  // attach external interrupt for FALLING og A-phase signal
  // check effectiveness by comment-out
  myRE.attachExtInterrupt([]{myRE.detect();});
}

void loop() {
  myRE.poll();
  delayMicroseconds(1000); // emulate very heavy processing
}

void on_rotenc_rotated_ccw() {
  Serial.println("CCW");
}

void on_rotenc_rotated_cw() {
  Serial.println("CW");
}
