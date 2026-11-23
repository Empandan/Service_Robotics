#pragma once

#include <Arduino.h>

const int L_FWD = 11;
const int L_BWD = 13;
const int R_FWD = 5;
const int R_BWD = 6;

const int L_ENC_A = 1;
const int L_ENC_B = 0;
const int R_ENC_A = 7;
const int R_ENC_B = 8;

const float COUNTS_PER_TURN = 1920.0;
const float WHEEL_DIAMETER_MM = 70.0;
const float WHEEL_SPACING_MM = 156.5;
const float COUNTS_PER_MM = COUNTS_PER_TURN / (PI * WHEEL_DIAMETER_MM);

const int DRIVE_SPEED = 150;
const int TURN_SPEED = 120;

void robotBegin();

// Positive speed = forward, negative = backward, range -255..255
void setMotors(int left, int right);
void stopMotors();

void readCounts(long &left, long &right);
void resetCounts();

// Each move resets the counts, so readCounts() afterwards gives that move's counts
void forward(float mm, int speed = DRIVE_SPEED);
void backward(float mm, int speed = DRIVE_SPEED);
void turnLeft(float degrees, int speed = TURN_SPEED);
void turnRight(float degrees, int speed = TURN_SPEED);
