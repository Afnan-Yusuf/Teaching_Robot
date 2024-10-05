#include <Arduino.h>

const byte interruptPin = 33;
const byte channelAmount = 6;
const unsigned long blankTime = 3750;
const unsigned long failsafeTimeout = 21000;
const unsigned long minPulseTime = 900;   // Minimum valid pulse width (in microseconds)
const unsigned long maxPulseTime = 2100;  // Maximum valid pulse width (in microseconds)

volatile unsigned long lastPulseTime = 0;
volatile unsigned long rawValues[channelAmount];
volatile byte currentChannel = 0;

void IRAM_ATTR handlePPMInterrupt() {
    unsigned long currentMicros = micros();
    unsigned long timeSinceLastPulse = currentMicros - lastPulseTime;
    lastPulseTime = currentMicros;

    if (timeSinceLastPulse > blankTime) {
        currentChannel = 0;
    } else {
        if (currentChannel < channelAmount) {
            // Store the pulse duration only if it's within a valid range
            if (timeSinceLastPulse >= minPulseTime && timeSinceLastPulse <= maxPulseTime) {
                rawValues[currentChannel] = timeSinceLastPulse;
            }
            currentChannel++;
        }
    }
}

void initct6b() {
    Serial.begin(115200);
    pinMode(interruptPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(interruptPin), handlePPMInterrupt, RISING);
}

void runonct6b() {
    unsigned long safeValues[channelAmount];
    
    // Disable interrupts to safely read shared values
    noInterrupts();
    for (byte i = 0; i < channelAmount; i++) {
        safeValues[i] = rawValues[i];
    }
    interrupts();

    // Print the safe values only if they are within the valid range
    for (byte i = 0; i < channelAmount; i++) {
        if (safeValues[i] >= minPulseTime && safeValues[i] <= maxPulseTime) {
            Serial.print(safeValues[i]);
        } else {
            Serial.print("Invalid");  // Show "Invalid" for any out-of-range value
        }
        Serial.print("\t");
    }

    Serial.println();
    delay(3);
}
