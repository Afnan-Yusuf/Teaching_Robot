#pragma once
#include <Arduino.h>
#include <ESP32Servo.h>

Servo myservo;
int pos = 0; 
int servoPin = 4;
int increment = 1;          // Increment value for servo position
const long interval = 20;   // Update interval for smooth movement (20ms)
unsigned long pmn1 = 0; // Store the last time the servo was updated
enum SweepDirection { SWEEPING_UP, SWEEPING_DOWN };
SweepDirection currentDirection = SWEEPING_UP; // Direction for sweeping

void headservoinit() {
    // Allow allocation of all timers
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    myservo.setPeriodHertz(50);    // Standard 50 Hz servo
    myservo.attach(servoPin, 1000, 2000);
}

void headservoSweep() {
    unsigned long currentMillis = millis();

    // Check if it's time to update the servo position
    if (currentMillis - pmn1 >= interval) {
        pmn1 = currentMillis; // Save the last update time

        // Update the servo position based on the current direction
        if (currentDirection == SWEEPING_UP) {
            pos += increment; // Increase position
            if (pos >= 180) { // Check if we've reached the max position
                pos = 180;
                currentDirection = SWEEPING_DOWN; // Change direction
            }
        } else { // SWEEPING_DOWN
            pos -= increment; // Decrease position
            if (pos <= 0) { // Check if we've reached the min position
                pos = 0;
                currentDirection = SWEEPING_UP; // Change direction
            }
        }

        myservo.write(pos); // Update servo position
    }
}


