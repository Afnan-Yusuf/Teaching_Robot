#pragma once
#include <Arduino.h>
#include <ESP32Servo.h>
#include <Ramp.h>
#include "scheduler.h"
//#include "ct6b.h"

extern int z;

ramp servoramp;
int finalpos;

Servo myservo;
int pos = 0; 
int servoPin = 4;
int increment = 1;          // Increment value for servo position
const long interval = 20;   // Update interval for smooth movement (20ms)
unsigned long pmn1 = 0; // Store the last time the servo was updated
enum SweepDirection { SWEEPING_UP, SWEEPING_DOWN };
SweepDirection currentDirection = SWEEPING_UP; // Direction for sweeping
void writeservo(int angle);

// Moving average filter variables
const int filterSize = 50;  // Size of the moving average filter (adjust as needed)
int zValues[filterSize];    // Array to store past `z` values
int zIndex = 0;             // Index to track the current position in the buffer

void headservoinit() {
    // Allow allocation of all timers
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    myservo.setPeriodHertz(50);    // Standard 50 Hz servo
    myservo.attach(servoPin, 1000, 2000);

    // Initialize the moving average filter buffer with zeros
    for (int i = 0; i < filterSize; i++) {
        zValues[i] = 0;
    }
}

int filterZValue(int rawZ) {
    // Add the new `z` value to the buffer
    zValues[zIndex] = rawZ;
    zIndex = (zIndex + 1) % filterSize;

    // Compute the average of the buffer
    int sum = 0;
    for (int i = 0; i < filterSize; i++) {
        sum += zValues[i];
    }

    return sum / filterSize;
}

void headservoSweep() {
    // Apply the moving average filter to the `z` value
    int filteredZ = filterZValue(z);

    // Write the filtered value to the servo
    myservo.write(filteredZ);
}

void writeservo(int angle){
    finalpos = myramp.update();
    myramp.go(angle, 100, LINEAR, ONCEFORWARD);
    myservo.write(finalpos);
}
