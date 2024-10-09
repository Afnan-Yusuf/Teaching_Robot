#pragma once
#include <Arduino.h>
#include "BluetoothSerial.h"
#include "ultrasonic.h"
#include <Ramp.h>

ramp myramp;


extern bool obstacledetected;

char incomingData;
BluetoothSerial BT;

int speed = 200;
int minspeed = 50;
int acceltime = 1750;
int deceltime = 750;
int stopspeed = 0;
int t;
void recvWithEndMarker();

void initbt() {
    BT.begin("Teaching Robot");

}

void btcontrol() {
    recvWithEndMarker();
    
    
     t = myramp.update();
     //Serial.println(t);
    if (!obstacledetected) {
        switch (incomingData) {
            case 'f': // Forward
                t = 0;
                myramp.go(speed, acceltime, LINEAR, ONCEFORWARD);
                fd(t);
                incomingData = 0;
                break;

            case 'l': // Left turn
                t = 0;
                myramp.go(speed, acceltime, LINEAR, ONCEFORWARD);
                lt(t);
                incomingData = 0;
                break;

            case 'r': // Right turn
                t = 0;
                myramp.go(speed, acceltime, LINEAR, ONCEFORWARD);
                rt(t);
                incomingData = 0;
                break;

            case 'b': // Backward
                t = 0;
                myramp.go(speed, acceltime, LINEAR, ONCEFORWARD);
                bk(t);
                incomingData = 0;
                break;

            case 's': // Stop
                t = 0;
                myramp.go(stopspeed, deceltime, LINEAR, ONCEFORWARD);
                fd(t);
                incomingData = 0;
                break;

            default: // If no valid command is received
                // Optionally handle unexpected input
                break;
        }
    } else {
        if(incomingData == 'b'){
            t = 0;
            myramp.go(speed, acceltime, LINEAR, ONCEFORWARD);
            bk(t);
            incomingData = 0;
        }
        StopMot();  // Stop motors if an obstacle is detected
    }
    
}


void recvWithEndMarker() {
    if (BT.available() > 0) {
        incomingData = BT.read();
        //btpidv(incomingData);
        Serial.println(incomingData);
    }
}