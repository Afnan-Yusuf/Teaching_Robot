#pragma once
#include <Arduino.h>
#include "BluetoothSerial.h"
#include "ultrasonic.h"

extern bool obstacledetected;

char incomingData;
BluetoothSerial BT;

int speed = 200;
int minspeed = 50;
int acceltime = 2000;
int deceltime = 2000;

void recvWithEndMarker();

void initbt() {
    BT.begin("Teaching Robot");
}

void btcontrol() {
    recvWithEndMarker();
    
    if (obstacledetected == false) {
        if (incomingData == 'f') {
            fd(speed);
        }
        if (incomingData == 'l') {
            lt(speed);
        }
        if (incomingData == 'r') {
            rt(speed);
        }
    } else {
        StopMot();
    }
    
    if (incomingData == 's') {
        StopMot();
    }
    if (incomingData == 'b') {
        bk(speed);
    }
}

void recvWithEndMarker() {
    if (BT.available() > 0) {
        incomingData = BT.read();
        //btpidv(incomingData);
        Serial.println(incomingData);
    }
}