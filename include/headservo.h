#pragma once
#include <Arduino.h>
#include <ESP32Servo.h>
#include <Ramp.h>
//#include "ct6b.h"

extern int z;

int ang;

rampInt servoramp;
int finalpos;

unsigned long lastUpdateTime = 0;  // To store the last time the servo was updated
int targetAngle = 90;              // Target angle for the servo to move to
int currentPos = 90;               // Current position of the servo
bool returningTo90 = false;        // Flag to check if we are returning to 90 degrees
unsigned long targetHoldTime = 0;  // Time when the target angle was reached

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
    myservo.write(90);
}

void writeservo(int angle) {
    targetAngle = angle;           // Set the new target angle
    returningTo90 = false;         // Reset the flag for returning to 90 degrees
    lastUpdateTime = millis();     // Store the current time
}

void updateServo() {
    unsigned long currentTime = millis();
    int increment = 1;  // Step size for the servo movement
    unsigned long interval = 20;  // Time between steps (20 ms)

    // Move the servo gradually if the target position has not been reached
    if (!returningTo90 && currentPos != targetAngle) {
        if (currentTime - lastUpdateTime >= interval) {
            // Move the servo towards the target angle in steps
            if (currentPos < targetAngle) {
                currentPos += increment;
                if (currentPos > targetAngle) currentPos = targetAngle;  // Ensure it doesn't overshoot
            } else if (currentPos > targetAngle) {
                currentPos -= increment;
                if (currentPos < targetAngle) currentPos = targetAngle;  // Ensure it doesn't overshoot
            }
            
            // Write the new position to the servo
            myservo.write(currentPos);
            lastUpdateTime = currentTime;  // Update the last time we moved the servo
        }
    }

    // Check if the servo has reached the target angle and start counting the hold time
    if (currentPos == targetAngle && !returningTo90) {
        if (targetHoldTime == 0) {
            targetHoldTime = currentTime;  // Start timing the 2500ms hold
        }

        // Wait for 2500ms before returning to 90 degrees
        if (currentTime - targetHoldTime >= 2500) {
            returningTo90 = true;  // Start returning to 90 degrees
            targetAngle = 90;      // Set the target angle to 90
            targetHoldTime = 0;    // Reset the hold timer
        }
    }

    // Move the servo back to 90 degrees in the same manner
    if (returningTo90 && currentPos != 90) {
        if (currentTime - lastUpdateTime >= interval) {
            if (currentPos < 90) {
                currentPos += increment;
                if (currentPos > 90) currentPos = 90;
            } else if (currentPos > 90) {
                currentPos -= increment;
                if (currentPos < 90) currentPos = 90;
            }

            myservo.write(currentPos);
            lastUpdateTime = currentTime;  // Update the last time we moved the servo
        }
    }
}
