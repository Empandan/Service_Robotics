#include <Arduino.h>
#include <QTRSensors.h>
#include "robot.h"

QTRSensors qtr;

const uint8_t SensorCount = 8;
const uint8_t sensorPins[SensorCount] = {A0, A1, A2, A3, A4, A5, A6, A11};
uint16_t sensorValues[SensorCount];

// Set to false if sensor 1 is on the robot's right side
const bool SENSOR_1_ON_LEFT = false;

const int BASE_SPEED = 130;
const float KP = 0.04;
const float KD = 0.2;

const int SWEEP_SPEED = 100;
const uint16_t LINE_SEEN = 300;
const unsigned long LINE_LOST_MS = 300;
const unsigned long RUN_MS = 30000;
const unsigned long SWEEP_TIMEOUT_MS = 5000;

const char *stopReason = "";

// dir = 1 turns left, -1 turns right, calibrating the sensor while turning
void sweep(int dir, float degrees) {
    long target = degrees / 360.0 * PI * WHEEL_SPACING_MM * COUNTS_PER_MM;
    resetCounts();
    setMotors(-dir * SWEEP_SPEED, dir * SWEEP_SPEED);
    unsigned long start = millis();
    long l = 0, r = 0;
    while ((abs(l) + abs(r)) / 2 < target && millis() - start < SWEEP_TIMEOUT_MS) {
        qtr.calibrate();
        readCounts(l, r);
    }
    stopMotors();
    delay(200);
}

bool lineSeen() {
    for (int i = 0; i < SensorCount; i++) {
        if (sensorValues[i] > LINE_SEEN) return true;
    }
    return false;
}

void followLine() {
    unsigned long start = millis();
    unsigned long lastSeen = millis();
    unsigned long lastPrint = 0;
    int lastError = 0;

    while (true) {
        if (millis() - start > RUN_MS) {
            stopReason = "time up";
            break;
        }

        uint16_t position = qtr.readLineBlack(sensorValues);
        if (lineSeen()) {
            lastSeen = millis();
        } else if (millis() - lastSeen > LINE_LOST_MS) {
            stopReason = "line lost";
            break;
        }

        // Negative error means the line is to the left
        int error = (int)position - 3500;
        if (!SENSOR_1_ON_LEFT) error = -error;

        int correction = KP * error + KD * (error - lastError);
        lastError = error;
        int left = BASE_SPEED + correction;
        int right = BASE_SPEED - correction;
        setMotors(left, right);

        if (millis() - lastPrint >= 100) {
            lastPrint = millis();
            Serial.print("pos: ");
            Serial.print(position);
            Serial.print("   err: ");
            Serial.print(error);
            Serial.print("   L: ");
            Serial.print(left);
            Serial.print("   R: ");
            Serial.println(right);
        }
    }

    stopMotors();
}

void setup() {
    robotBegin();
    Serial.begin(115200);

    qtr.setTypeAnalog();
    qtr.setSensorPins(sensorPins, SensorCount);

    delay(3000);

    // Swing the sensor across the line both ways and end facing forward again
    sweep(1, 45);
    sweep(-1, 90);
    sweep(1, 90);
    sweep(-1, 90);
    sweep(1, 45);
    delay(500);

    followLine();
}

void loop() {
    Serial.print("stopped: ");
    Serial.println(stopReason);
    delay(1000);
}
