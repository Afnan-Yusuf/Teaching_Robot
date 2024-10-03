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
    unsigned long currentMillis = millis();
    
    // Check if the time has passed to update the servo position
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        // Update servo position
        pos += increment;
        
        // Reverse direction if reaching 0 or 180 degrees
        if (pos <= 0 || pos >= 180) {
            increment = -increment;
        }

        // Write the new position to the servo
        myservo.write(pos);
    }
}
