#include <Arduino.h>

const byte interruptPin = 33;
const byte channelAmount = 6;
const unsigned long blankTime = 4000;
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
}

void runonct6b()
{
  unsigned long safeValues[channelAmount];

  // Disable interrupts to safely read shared values
  noInterrupts();
  for (byte i = 0; i < channelAmount; i++)
  {
    safeValues[i] = rawValues[i];
  }
  interrupts();

  // Print the safe values only if they are within the valid range
  for (byte i = 0; i < channelAmount; i++)
  {
    if (safeValues[i] >= minPulseTime && safeValues[i] <= maxPulseTime)
    {
      // Serial.print(safeValues[i]);
    }
    else
    {
      // Serial.print("Invalid"); // Show "Invalid" for any out-of-range value
    }
    // Serial.print("\t");
  }

  // Serial.println();
  delay(3);

  x = map(safeValues[0], ch1min, ch2max, 510, 0) - 255;
  y = map(safeValues[1], ch1min, ch1max, 0, 510) - 255;
  z = map(safeValues[3], ch3min, ch3max, 180, 0);
  z > 180 ? z = 180 : z = z;
  z < 0 ? z = 0 : z = z;
  motleftspeed = x + y;
  motrightspeed = x - y;
  // Serial.println(safeValues[3]);
  // writeservo(z);

  motrightspeed > maxspeed ? motrightspeed = maxspeed : motrightspeed = motrightspeed;
  motleftspeed > maxspeed ? motleftspeed = maxspeed : motleftspeed = motleftspeed;
  motrightspeed < slow ? motrightspeed = slow : motrightspeed = motrightspeed;
  motleftspeed < slow ? motleftspeed = slow : motleftspeed = motleftspeed;
  if(safeValues[2] > 1200){
    if (x > 25 || x < -25 || y > 25 || y < -25)
    {
      if (motleftspeed > 0)
      {
        leftmotbackward(abs(motleftspeed));
        // Serial.println("yhn");
      }
      else if (motleftspeed < 0)
      {

        leftmotforward(abs(motleftspeed));
      }

      if (motrightspeed > 0)
      {
        rightmotforward(abs(motrightspeed));
      }
      else if (motrightspeed < 0)
      {
        rightmotbackward(abs(motrightspeed));
      }
    }
    else
    {
      StopMot();
    }
  }else{
    StopMot();
    }

  Serial.print(x);
  Serial.print("\t");
  Serial.print(y);
  Serial.print("\t");
  Serial.print(motleftspeed);
  Serial.print("\t");
  Serial.print(motrightspeed);
  Serial.print("\t");
  Serial.println(z);
}
