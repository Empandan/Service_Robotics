#include "robot.h"

const float KP = 1.0;
const unsigned long TIMEOUT_MS = 10000;

static volatile long leftCount = 0;
static volatile long rightCount = 0;

static void leftISR() {
  if (digitalRead(L_ENC_A) == digitalRead(L_ENC_B)) leftCount++;
  else leftCount--;
}

static void rightISR() {
  if (digitalRead(R_ENC_A) == digitalRead(R_ENC_B)) rightCount++;
  else rightCount--;
}

void robotBegin() {
  pinMode(L_FWD, OUTPUT);
  pinMode(L_BWD, OUTPUT);
  pinMode(R_FWD, OUTPUT);
  pinMode(R_BWD, OUTPUT);
  stopMotors();

  pinMode(L_ENC_A, INPUT_PULLUP);
  pinMode(L_ENC_B, INPUT_PULLUP);
  pinMode(R_ENC_A, INPUT_PULLUP);
  pinMode(R_ENC_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(L_ENC_A), leftISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(R_ENC_A), rightISR, CHANGE);
}

static void setMotor(int fwdPin, int bwdPin, int speed) {
  speed = constrain(speed, -255, 255);
  if (speed >= 0) {
    analogWrite(fwdPin, speed);
    analogWrite(bwdPin, 0);
  } else {
    analogWrite(fwdPin, 0);
    analogWrite(bwdPin, -speed);
  }
}

void setMotors(int left, int right) {
  setMotor(L_FWD, L_BWD, left);
  setMotor(R_FWD, R_BWD, right);
}

void stopMotors() {
  setMotors(0, 0);
}

void readCounts(long &left, long &right) {
  noInterrupts();
  left = leftCount;
  right = rightCount;
  interrupts();
}

void resetCounts() {
  noInterrupts();
  leftCount = 0;
  rightCount = 0;
  interrupts();
}

// dirL and dirR are +1 or -1. Both wheels travel target counts,
// slowing whichever wheel is ahead so their distances stay equal.
static void move(int dirL, int dirR, long target, int speed) {
  resetCounts();
  unsigned long start = millis();
  long l = 0, r = 0;
  while ((dirL * l + dirR * r) / 2 < target && millis() - start < TIMEOUT_MS) {
    readCounts(l, r);
    int correction = KP * (dirL * l - dirR * r);
    setMotors(dirL * constrain(speed - correction, 0, 255),
              dirR * constrain(speed + correction, 0, 255));
    delay(10);
  }
  stopMotors();
  delay(500);
}

// On the spot, each wheel moves along a circle the size of the wheel spacing
static long turnCounts(float degrees) {
  return degrees / 360.0 * PI * WHEEL_SPACING_MM * COUNTS_PER_MM;
}

void forward(float mm, int speed) {
  move(1, 1, mm * COUNTS_PER_MM, speed);
}

void backward(float mm, int speed) {
  move(-1, -1, mm * COUNTS_PER_MM, speed);
}

void turnLeft(float degrees, int speed) {
  move(-1, 1, turnCounts(degrees), speed);
}

void turnRight(float degrees, int speed) {
  move(1, -1, turnCounts(degrees), speed);
}
