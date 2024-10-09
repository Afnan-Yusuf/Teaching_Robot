#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 23
#define LED_COUNT 5
#define COLOR_CHANGE_INTERVAL 20 // Define the non-blocking delay interval

Adafruit_NeoPixel rgbled(LED_COUNT, LED_PIN, NEO_RGBW + NEO_KHZ800);

// Variables to keep track of time
unsigned long previousMillis = 0;

void rgbledinit()
{
  rgbled.begin();
  rgbled.show(); // Initialize all pixels to 'off'
}

void rgbledcontrol() {
  static uint32_t colors[LED_COUNT] = {0};
  static uint8_t hue[LED_COUNT] = {0};

  unsigned long currentMillis = millis(); // Get the current time

  // Check if enough time has passed to update the colors
  if (currentMillis - previousMillis >= COLOR_CHANGE_INTERVAL) {
    // Save the last time the colors were updated
    previousMillis = currentMillis;

    // Update the LED colors
    for (int i = 0; i < LED_COUNT; i++) {
      // Use the current hue to set the color
      colors[i] = rgbled.ColorHSV(hue[i] * 256, 255, 255);

      // Set the LED color
      rgbled.setPixelColor(i, colors[i]);

      // Increment the hue, wrapping around at 256
      hue[i] = (hue[i] + 1) % 256;
    }

    // Show the updated colors on the strip
    rgbled.show();
  }
}
