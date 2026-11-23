#include <Arduino.h>
#include "robot.h"

const float SIDE_MM = 300.0;

void printCounts(const char *name) {
  long l, r;
  readCounts(l, r);
  Serial.print(name);
  Serial.print("   L: ");
  Serial.print(l);
  Serial.print("   R: ");
  Serial.println(r);
}

void setup() {
  robotBegin();
  Serial.begin(115200);
  delay(3000);

  for (int i = 0; i < 4; i++) {
    forward(SIDE_MM);
    printCounts("forward");
    turnLeft(90);
    printCounts("turn left");
  }
}

void loop() {}
