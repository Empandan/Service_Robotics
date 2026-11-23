#include <Arduino.h>

// HC-SR04 style ultrasonic sensor test.
// Wiring: VCC -> 5V, GND -> GND, TRIG -> pin 2, ECHO -> pin 3
// (these pins don't clash with the motor/encoder pins in robot.h)
const int TRIG_PIN = 2;
const int ECHO_PIN = 3;

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);
  while (!Serial && millis() < 3000) {}  // Leonardo: wait for serial monitor
  Serial.println("Ultrasonic test");
}

void loop() {
  // 10 us trigger pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Echo pulse width in us (30 ms timeout ~ 5 m)
  unsigned long us = pulseIn(ECHO_PIN, HIGH, 30000UL);

  if (us == 0) {
    Serial.println("No echo");
  } else {
    float cm = us * 0.0343f / 2.0f;
    Serial.print(cm, 1);
    Serial.println(" cm");
  }
  delay(100);
}
