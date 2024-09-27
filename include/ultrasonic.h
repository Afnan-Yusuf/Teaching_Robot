#pragma once
#include <Arduino.h>

#define SENSOR_PIN 34  // Example: GPIO34

void initsharpir() {
  Serial.begin(115200);  // Start serial communication
  pinMode(SENSOR_PIN, INPUT);
}

void obstacledeteect() {
  int analogValue = analogRead(SENSOR_PIN);  // Read the analog value from the sensor
  float voltage = analogValue * (3.3 / 4095.0);  // Convert ADC value to voltage (ESP32 ADC resolution is 12 bits: 0-4095)
  float distance = 12.08 * pow(voltage, -1.058);
}
