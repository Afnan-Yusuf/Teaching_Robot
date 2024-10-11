#pragma once
#include <Arduino.h>

// Define constants
void obstacledetect();
int thresh = 30;
#define SENSORLEFT_PIN 34  // Example: GPIO34 for left sensor
#define SENSORRIGHT_PIN 35  // Example: GPIO35 for right sensor

bool obstacledetected = false;

// Filter settings
const int sampleSize = 5;  // Number of samples for moving average
float leftSamples[sampleSize] = {0};  // Array to store recent left sensor samples
float rightSamples[sampleSize] = {0};  // Array to store recent right sensor samples
int sampleIndex = 0;  // Index to store the next sample in the array

// Helper function to calculate moving average
float calculateAverage(float samples[], int size) {
  float sum = 0;
  for (int i = 0; i < size; i++) {
    sum += samples[i];
  }
  return sum / size;
}

void obstacledetect() {
  // Read the analog values from both sensors
  int leftAnalogValue = analogRead(SENSORLEFT_PIN);  
  int rightAnalogValue = analogRead(SENSORRIGHT_PIN);  

  // Convert ADC values to voltage (ESP32 ADC resolution is 12 bits: 0-4095)
  float leftVoltage = leftAnalogValue * (3.3 / 4095.0);  
  float rightVoltage = rightAnalogValue * (3.3 / 4095.0);  

  // Calculate distance for both sensors using the provided formula
  float leftDistance = 12.08 * pow(leftVoltage, -1.058);
  float rightDistance = 12.08 * pow(rightVoltage, -1.058);

  // Store the latest sensor readings in the sample arrays
  leftSamples[sampleIndex] = leftDistance;
  rightSamples[sampleIndex] = rightDistance;

  // Update the index, wrap around if necessary
  sampleIndex = (sampleIndex + 1) % sampleSize;

  // Calculate the average of the last few readings to smooth the data
  float leftAvgDistance = calculateAverage(leftSamples, sampleSize);
  float rightAvgDistance = calculateAverage(rightSamples, sampleSize);

  // Check if the filtered distances are below the threshold
  if ((leftAvgDistance < thresh) || (rightAvgDistance < thresh)) {
    obstacledetected = true;
  } else {
    obstacledetected = false;
  }

  // Optional: Debugging output
  // Serial.print("Left Distance (avg): ");
  // Serial.print(leftAvgDistance);
  // Serial.print(" cm, Right Distance (avg): ");
  // Serial.println(rightAvgDistance);
}

void initsharpir() {
  pinMode(SENSORLEFT_PIN, INPUT);   // Set left sensor pin as input
  pinMode(SENSORRIGHT_PIN, INPUT);  // Set right sensor pin as input
}
