#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 6
#define LED_COUNT 16
Adafruit_NeoPixel rgbled(LED_COUNT, LED_PIN, NEO_RGBW + NEO_KHZ800);
void rgbledinit()
{
    rgbled.begin();
}

void rgbledcontrol(){
    rgbled.setPixelColor(1, rgbled.Color(55, 5, 55));
    rgbled.setBrightness(100);
}
