#pragma once
#include <Arduino.h>
void obstacledetect();
int thresh = 20;
#define SENSORLEFT_PIN 34  // Example: GPIO34 for left sensor
#define SENSORRIGHT_PIN 35  // Example: GPIO35 for right sensor


bool obstacledetected = false;

void obstacledetect() {
  // Read the analog values from both sensors
  int leftAnalogValue = analogRead(SENSORLEFT_PIN);  
  int rightAnalogValue = analogRead(SENSORRIGHT_PIN);  

  // Convert ADC values to voltage (ESP32 ADC resolution is 12 bits: 0-4095)
  float leftVoltage = leftAnalogValue * (3.3 / 4095.0);  
  float rightVoltage = rightAnalogValue * (3.3 / 4095.0);  

  // Calculate distance for both sensors
  float leftDistance = 25;//12.08 * pow(leftVoltage, -1.058);
  float rightDistance = 25;//12.08 * pow(rightVoltage, -1.058);
  if((leftDistance < thresh) || (rightDistance < thresh)){
    obstacledetected = true;
  }else{
    obstacledetected = false;
  }

  // Print the distances for debugging
  //Serial.print("Left Distance: ");
  // Serial.print(leftDistance);
  // Serial.print(" cm, Right Distance: ");
  // Serial.println(rightDistance);
}
void initsharpir() {
  Serial.begin(115200);  // Start serial communication
  pinMode(SENSORLEFT_PIN, INPUT);   // Set left sensor pin as input
  pinMode(SENSORRIGHT_PIN, INPUT);  // Set right sensor pin as input
}