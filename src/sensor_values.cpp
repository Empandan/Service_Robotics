#include <Arduino.h>
#include <QTRSensors.h>
#include "robot.h"

QTRSensors qtr;

const uint8_t SensorCount = 8;
const uint8_t sensorPins[SensorCount] = {A0, A1, A2, A3, A4, A5, A6, A11};
uint16_t sensorValues[SensorCount];

void printValues(const char *label, const uint16_t *values) {
    Serial.print(label);
    for (int i = 0; i < SensorCount; i++) {
        Serial.print(values[i]);
        Serial.print("\t");
    }
    Serial.println();
}

void setup() {
    robotBegin();
    Serial.begin(115200);
    while (!Serial) {}

    qtr.setTypeAnalog();
    qtr.setSensorPins(sensorPins, SensorCount);

    // LED_BUILTIN is pin 13, which drives the left motor, so progress goes over serial
    Serial.println("Calibrating in 2 s, slide the sensor back and forth over the line");
    delay(2000);
    for (int i = 0; i < 400; i++) {
        qtr.calibrate();
    }
    Serial.println("Calibration done");
    printValues("min:\t", qtr.calibrationOn.minimum);
    printValues("max:\t", qtr.calibrationOn.maximum);
    delay(2000);
}

void loop() {
    uint16_t position = qtr.readLineBlack(sensorValues);

    for (int i = 0; i < SensorCount; i++) {
        Serial.print(sensorValues[i]);
        Serial.print("\t");
    }

    Serial.print("Position: ");
    Serial.println(position);

    delay(100);
}
