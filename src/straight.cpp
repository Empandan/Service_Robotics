#include <Arduino.h>
#include "robot.h"

void setup() {
  robotBegin();
  Serial.begin(115200);
  delay(3000);

  forward(3 * PI * WHEEL_DIAMETER_MM);
}

void loop() {
  long l, r;
  readCounts(l, r);
  Serial.print("done   L: ");
  Serial.print(l);
  Serial.print("   R: ");
  Serial.print(r);
  Serial.print("   diff: ");
  Serial.println(l - r);
  delay(1000);
}
