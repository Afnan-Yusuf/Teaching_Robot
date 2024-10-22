#pragma once
#include <Arduino.h>
#define RXD2 16
#define TXD2 17

int modet = 0;  // This will store the last mode value
int mode = 0;   // 0 = walking; 1 = handshake; 2 = stop;

void initserial2() {
    Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void arduinointerface() {
    //if (mode != modet) {  // Only proceed if the mode has changed
        if (mode == 0) {
            Serial1.println('1');
        } else if (mode == 1) {
            Serial1.println('2');
        } else if (mode == 2) {
            Serial1.println('3');
        }

        modet = mode;  // Update the last mode to the current one
    //}

    //Serial.println(mode);  // Optional for debugging
}
