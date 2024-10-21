#pragma once
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

// Create the display object for a 128x64 OLED using I2C
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE, /* clock=*/22, /* data=*/21);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C mdisp(U8G2_R0, /* reset=*/U8X8_PIN_NONE, /* clock=*/22, /* data=*/21);

int blinkDelay = 200;  // Time for eye to remain open
int blinkCloseDuration = 2500;  // Time for eye to remain closed
unsigned long mouthAnimationDelay = 300;  // Interval for mouth animation

// Timing variables for non-blocking delay
unsigned long previousMillis = 0;
unsigned long mouthPreviousMillis = 0;
int blinkState = 0;  // 0 for open eye, 1 for closed eye
int mouthState = 0;  // 0 for closed mouth, 1 for open mouth

// Function to draw an open eye
void drawEyeOpen() {
    u8g2.clearBuffer();
  
    // Draw eye outline (maximize size with thicker lines)
    u8g2.setDrawColor(1);  // Set draw color to white
  
    // Draw pupil (larger pupil)
    u8g2.drawDisc(64, 32, 40); // Larger pupil at center

    u8g2.sendBuffer();
}

// Function to draw a closed eye
void drawEyeClosed() {
    u8g2.clearBuffer();
  
    // Draw closed eye (thicker horizontal line)
    u8g2.drawLine(14, 32, 114, 32);  // Maximize length of closed eye
    u8g2.drawLine(15, 33, 113, 33);  // Thicken the line for the closed eye
  
    u8g2.sendBuffer();
}

// Function to draw a robotic mouth animation on the second display
void mouthanimation() {
    unsigned long currentMillis = millis();

    if (currentMillis - mouthPreviousMillis >= mouthAnimationDelay) {
        mouthPreviousMillis = currentMillis;
        mdisp.clearBuffer();
        
        if (mouthState == 0) {
            // Draw closed mouth (a horizontal line)
            mdisp.drawLine(20, 48, 108, 48);
            mdisp.drawLine(21, 49, 107, 49);
            mouthState = 1;  // Switch to open mouth
        } else {
            // Draw open mouth (a rectangle simulating an open mouth)
            mdisp.drawFrame(20, 40, 88, 20);  // Outer rectangle
            mdisp.drawBox(28, 44, 72, 12);    // Inner black box to create a "mouth" effect
            mouthState = 0;  // Switch to closed mouth
        }
        
        mdisp.sendBuffer();
    }
}

void initeyedisplay() {
    // Initialize the display
    u8g2.begin();
    mdisp.setI2CAddress(0x3D << 1);
    mdisp.begin();
}

void eyeblink() {
    unsigned long currentMillis = millis();

    if (blinkState == 0 && currentMillis - previousMillis >= blinkDelay) {
        // Open eye for blinkDelay duration
        drawEyeOpen();
        blinkState = 1;  // Switch to closed eye
        previousMillis = currentMillis;
    } 
    else if (blinkState == 1 && currentMillis - previousMillis >= blinkCloseDuration) {
        // Close eye for blinkCloseDuration
        drawEyeClosed();
        blinkState = 0;  // Switch back to open eye
        previousMillis = currentMillis;
    }
    mouthanimation();
}
