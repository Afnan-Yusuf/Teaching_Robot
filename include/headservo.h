#pragma once
#include <Arduino.h>

#include <ESP32Servo.h>

Servo myservo;
int pos = 0; 
int servoPin = 4;

void headservoinit(){
    
	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo.setPeriodHertz(50);    // standard 50 hz servo
	myservo.attach(servoPin, 1000, 2000);
}
void headservo(int angle){
    myservo.write(angle);
}
