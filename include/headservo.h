#pragma once
#include <Arduino.h>
#include <ESP32Servo.h>

Servo myservo;
int pos = 0; 
int servoPin = 4;
int increment = 1;          // Increment value for servo position
 
const long interval = 20;   // Update interval for smooth movement (20ms)

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
    //normal servo sweep code
    for (pos = 0; pos <= 180; pos += increment) { // goes from 0 degrees to 180 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= increment) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
}
