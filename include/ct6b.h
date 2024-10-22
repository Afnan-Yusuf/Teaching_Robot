#include <Arduino.h>
#include "arduinointerface.h"
extern int mode;

const byte interruptPin = 33;
const byte channelAmount = 6;
const unsigned long blankTime = 3000;
const unsigned long failsafeTimeout = 50000;
const unsigned long minPulseTime = 800;  // Minimum valid pulse width (in microseconds)
const unsigned long maxPulseTime = 2100; // Maximum valid pulse width (in microseconds)

volatile unsigned long lastPulseTime = 0;
volatile unsigned long rawValues[channelAmount];
volatile byte currentChannel = 0;

const int ch2max = 1965;
const int ch2min = 1160;
const int ch1max = 2000;
const int ch1min = 1100;
const int ch3max = 1900;
const int ch3min = 1055;

int maxspeed = 255;
int mimumnspeed = (-190);
int slow = -255;

int motleftspeed = 0;
int motrightspeed = 0;

int x, y, z;

// Moving Average Filter Variables
const int fz = 50;               // Size of the moving average window
unsigned long filterBuffer[fz];  // Buffer to store previous values
int filterIndex = 0;                    // Current index in the buffer
unsigned long sum = 0;                  // Sum of the values in the buffer

// Interrupt handler for PPM
void IRAM_ATTR handlePPMInterrupt()
{
    unsigned long currentMicros = micros();
    unsigned long timeSinceLastPulse = currentMicros - lastPulseTime;
    lastPulseTime = currentMicros;

    if (timeSinceLastPulse > blankTime)
    {
        currentChannel = 0;
    }
    else
    {
        if (currentChannel < channelAmount)
        {
            // Store the pulse duration only if it's within a valid range
            if (timeSinceLastPulse >= minPulseTime && timeSinceLastPulse <= maxPulseTime)
            {
                rawValues[currentChannel] = timeSinceLastPulse;
            }
            currentChannel++;
        }
    }
}

void initct6b()
{
    pinMode(interruptPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(interruptPin), handlePPMInterrupt, RISING);

    // Initialize the moving average filter buffer with zeroes
    for (int i = 0; i < fz; i++) {
        filterBuffer[i] = 0;
    }
}

unsigned long applyMovingAverage(unsigned long newValue) {
    // Subtract the oldest value from the sum
    sum -= filterBuffer[filterIndex];
    
    // Add the new value to the sum
    filterBuffer[filterIndex] = newValue;
    sum += newValue;
    
    // Move to the next index in the buffer
    filterIndex = (filterIndex + 1) % fz;
    
    // Return the average of the values in the buffer
    return sum / fz;
}

void runonct6b()
{
    unsigned long safeValues[channelAmount];

    // Disable interrupts to safely read shared values
    //noInterrupts();
    for (byte i = 0; i < channelAmount; i++)
    {
        safeValues[i] = rawValues[i];
    }
    interrupts();

    // Apply the moving average filter to safeValues[5]
    safeValues[5] = applyMovingAverage(safeValues[5]);

    // Example: use filtered safeValues[5] to determine mode
    if (safeValues[5] > 1000 && safeValues[5] < 1300) {
        mode = 1;
    } else if (safeValues[5] > 1300 && safeValues[5] < 1700) {
        mode = 2;
    } else if (safeValues[5] > 1700) {
        mode = 0;
    }
    Serial.println(mode);

    // Continue with your logic (motor control, etc.)
    x = map(safeValues[0], ch1min, ch2max, 510, 0) - 255;
    y = map(safeValues[1], ch1min, ch1max, 0, 510) - 255;
    z = map(safeValues[3], ch3min, ch3max, 180, 0);
    z = constrain(z, 0, 180);
    
    motleftspeed = x + y;
    motrightspeed = x - y;

    motrightspeed = constrain(motrightspeed, slow, maxspeed);
    motleftspeed = constrain(motleftspeed, slow, maxspeed);

    // Motor control logic
    if (safeValues[2] > 1700) {
        if (abs(x) > 25 || abs(y) > 25) {
            if (motleftspeed > 0) {
                leftmotbackward(abs(motleftspeed));
            } else if (motleftspeed < 0) {
                leftmotforward(abs(motleftspeed));
            }

            if (motrightspeed > 0) {
                rightmotforward(abs(motrightspeed));
            } else if (motrightspeed < 0) {
                rightmotbackward(abs(motrightspeed));
            }
        } else {
            StopMot();
        }
    } else {
        StopMot();
    }
}
