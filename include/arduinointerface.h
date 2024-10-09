#pragma once
#include <Arduino.h>
#define RXD2 16
#define TXD2 17

int mode = 0;// 0 = walking; 1 = handshake; 2 = stop;

void initserial2()
{
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void arduinointerface(){
    if(mode == 0){
        Serial2.println('w');
    }else if(mode == 1){
        Serial2.println('h');
    }else if(mode == 2){
        Serial2.println('s');
    }
    delay(10);
}