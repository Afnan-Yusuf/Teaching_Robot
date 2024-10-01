#pragma once
#include <Arduino.h>
#define RXD2 16
#define TXD2 17

void initserial2()
{
    Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
}

void arduinointerface(){
    Serial2.println("HI");
}