#include <Arduino.h>
#include "robot.h"

void setup() {
  robotBegin();
  Serial.begin(115200);
}

void loop() {
  // Send 'r' to zero both counts
  if (Serial.available() && Serial.read() == 'r') {
    resetCounts();
  }

  long l, r;
  readCounts(l, r);
  Serial.print("L: ");
  Serial.print(l);
  Serial.print("   R: ");
  Serial.println(r);
  delay(200);
}
