/*
  demo_RotEnc_using_getter.ino
*/

#include <RotEnc.h>

RotEnc re(2, 3);

void setup() {
  Serial.begin(9600);
  while (!Serial) { }
  delay(500);
  Serial.println("*** demo_RotEnc_using_getter ***");
}

void loop() {
  re.poll();

  switch (re.getRotatedDirection()) {
    case CCW:
      Serial.println("CCW");
      break;
    case CW:
      Serial.println("CW");
      break;
    case PAUSE:
      break;
  }

  /* Following code works the same as above.
  if (re.isRotatedInCCW()) {
    Serial.println("CCW.");
  } else if (re.isRotatedInCW()) {
    Serial.println("CW.");
  }
  */
}