/*
  demo_RotEnc_using_callback.ino
*/

#include <RotEnc.h>

RotEnc re(2, 3);

void setup() {
  Serial.begin(9600);
  while (!Serial) { }
  delay(500);
  Serial.println("*** demo_RotEnc_using_callback ***");
  
  re.attachCallback_RotatedInCCW(on_rotenc_rotated_ccw);
  re.attachCallback_RotatedInCW(on_rotenc_rotated_cw);
}

void loop() {
  re.poll();
}

void on_rotenc_rotated_ccw() {
  Serial.println("CCW");
}

void on_rotenc_rotated_cw() {
  Serial.println("CW");
}
