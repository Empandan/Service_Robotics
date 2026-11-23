#include <Arduino.h>
#include "robot.h"

void step(const char *name, int left, int right) {
  Serial.println(name);
  setMotors(left, right);
  delay(1000);
  stopMotors();
  delay(1000);
}

void setup() {
  robotBegin();
  Serial.begin(115200);
}

void loop() {
  step("right forward", 0, 150);
  step("left forward", 150, 0);
  step("right backward", 0, -150);
  step("left backward", -150, 0);
}
