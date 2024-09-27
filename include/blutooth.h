#pragma once
#include <Arduino.h>
#include "BluetoothSerial.h"
char incomingData;
BluetoothSerial BT;
int speed = 200;
void recvWithEndMarker();
void initbt()
{
    BT.begin("Teaching Robot");
}
void btcontrol()
{
    recvWithEndMarker();    
    if(incomingData == 'f'){
        fd(speed);
    }
    if(incomingData == 'l'){
        lt(speed);
    }
    if(incomingData == 'r'){
        rt(speed);
    }
    if(incomingData == 'b'){
        bk(speed);
    }
    if(incomingData == 's'){
        StopMot();
    }
    
}
 void recvWithEndMarker(){
        if (BT.available() > 0)
        {
            incomingData = BT.read();
            //btpidv(incomingData);
        }
       // Serial.println(incomingData);
    }
