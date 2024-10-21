#pragma once
#include <Arduino.h>
#define RXD2 16
#define TXD2 17

int modet = 0;

int mode = 0;// 0 = walking; 1 = handshake; 2 = stop;

void initserial2()
{
    pinMode(RXD2, OUTPUT);
    pinMode(TXD2, OUTPUT);
}

void arduinointerface(){
    if(mode == 0){
        digitalWrite(RXD2, 0);
        digitalWrite(TXD2, 0);
    }else if(mode == 1){
        digitalWrite(RXD2, 0);
        digitalWrite(TXD2, 1);
    }else if(mode == 2){
        digitalWrite(RXD2, 1);
        digitalWrite(TXD2, 1);
    }

    //Serial.println(mode);
}