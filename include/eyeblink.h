#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Time management variables for non-blocking delays
unsigned long previousMillis = 0;  // Store the last time the eye state was updated
const long openEyeDuration = 3000; // Eye open duration in milliseconds
const long blinkStepDelay = 40;    // Blink animation delay per step in milliseconds

int blinkStep = 0;
bool eyeIsOpen = true;
bool isBlinking = false;

// Function prototypes
void drawEye(bool blink, float blinkProgress = 0);
void drawEllipse(Adafruit_SSD1306 &display, int16_t x0, int16_t y0, int16_t rx, int16_t ry, uint16_t color);

void initeyedisplay() {

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.display();
  delay(2000);
  display.clearDisplay();
}

void eyeblink() {
  unsigned long currentMillis = millis();

  if (eyeIsOpen && !isBlinking) {
    // Keep the eye open for 3000 ms
    if (currentMillis - previousMillis >= openEyeDuration) {
      previousMillis = currentMillis;
      isBlinking = true;  // Start blinking
      blinkStep = 0;      // Reset blink step
    }
    drawEye(false);  // Draw open eye
  } 
  else if (isBlinking) {
    // Manage blink animation
    if (currentMillis - previousMillis >= blinkStepDelay) {
      previousMillis = currentMillis;

      if (blinkStep <= 10) {
        drawEye(true, blinkStep * 0.1);  // Closing the eye
        blinkStep++;
      } else if (blinkStep <= 20) {
        drawEye(true, (20 - blinkStep) * 0.1);  // Opening the eye
        blinkStep++;
      } else {
        isBlinking = false;  // Stop blinking
        eyeIsOpen = true;    // Reset to open state
        previousMillis = currentMillis;  // Reset timer for next open period
      }
    }
  }
}

void drawEye(bool blink, float blinkProgress) {
  display.clearDisplay();
  
  // Increase the size of the eye
  int centerX = 64;  // Center of the display (X axis)
  int centerY = 32;  // Center of the display (Y axis)
  
  // New larger dimensions
  int width = 120;    // Increased width of the eye
  int height = 50;   // Increased height of the eye
  
  if (blink) {
    height = height * (1 - blinkProgress);  // Shrink eye height during blink
  }
  
  // Draw thicker outer eye outline by drawing multiple ellipses
  for (int i = 0; i < 3; i++) { // Loop to draw multiple ellipses for thickness (change 3 to higher for thicker line)
    drawEllipse(display, centerX, centerY, (width / 2) - i, (height / 2) - i, WHITE);
  }
  
  // Draw iris and pupil if eye is open enough
  if (height > 10) {
    // Increased iris size, making sure it fits in the eye
    int irisSize = min(height - 4, 30);  // Increased from 20 to 30 for a larger iris
    display.fillCircle(centerX, centerY, irisSize / 2, WHITE);  // Iris
    display.fillCircle(centerX, centerY, irisSize / 4, BLACK);  // Pupil
  }
  
  display.display();
}

// Standalone function to draw an ellipse
void drawEllipse(Adafruit_SSD1306 &display, int16_t x0, int16_t y0, int16_t rx, int16_t ry, uint16_t color) {
  int16_t x, y;
  int32_t rx2 = rx * rx;
  int32_t ry2 = ry * ry;
  int32_t fx2 = 4 * rx2;
  int32_t fy2 = 4 * ry2;
  int32_t s;

  for (x = 0, y = ry, s = 2 * ry2 + rx2 * (1 - 2 * ry); ry2 * x <= rx2 * y; x++) {
    display.drawPixel(x0 + x, y0 + y, color);
    display.drawPixel(x0 - x, y0 + y, color);
    display.drawPixel(x0 + x, y0 - y, color);
    display.drawPixel(x0 - x, y0 - y, color);
    if (s >= 0) {
      s += fx2 * (1 - y);
      y--;
    }
    s += ry2 * ((4 * x) + 6);
  }

  for (x = rx, y = 0, s = 2 * rx2 + ry2 * (1 - 2 * rx); rx2 * y <= ry2 * x; y++) {
    display.drawPixel(x0 + x, y0 + y, color);
    display.drawPixel(x0 - x, y0 + y, color);
    display.drawPixel(x0 + x, y0 - y, color);
    display.drawPixel(x0 - x, y0 - y, color);
    if (s >= 0) {
      s += fy2 * (1 - x);
      x--;
    }
    s += rx2 * ((4 * y) + 6);
  }
}