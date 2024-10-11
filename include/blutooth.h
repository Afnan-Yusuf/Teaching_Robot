#pragma once
#include <Arduino.h>
#include "BluetoothSerial.h"
#include "ultrasonic.h"
#include <Ramp.h>

ramp myramp;

extern bool obstacledetected;

char incomingData = 's';
char prevdata;
BluetoothSerial BT;

int speed = 200;
int minspeed = 50;
int acceltime = 1750;
int deceltime = 500;
int stopspeed = 0;
int t;
void recvWithEndMarker();

void initbt()
{
    BT.begin("Teaching Robot");
}

void btcontrol()
{
    

    t = myramp.update();
    Serial.print(t);
    Serial.print("\t");
    Serial.print(incomingData);
    Serial.print("\t");
    Serial.print(prevdata);
    Serial.print("\t");
    Serial.println(obstacledetected);
    recvWithEndMarker();
    if (obstacledetected == false)
    {
        
        switch (incomingData)
        {
        case 'f': // Forward
           t = 0;
            myramp.go(speed, acceltime, LINEAR, ONCEFORWARD);
            fd(t);
            prevdata = incomingData;
            incomingData = 0;
            break;

        case 'l': // Left turn
            t = 0;
            myramp.go(speed, acceltime, LINEAR, ONCEFORWARD);
            lt(t);
            prevdata = incomingData;
            incomingData = 0;
            break;

        case 'r': // Right turn
            t = 0;
            myramp.go(speed, acceltime, LINEAR, ONCEFORWARD);
            rt(t);
            prevdata = incomingData;
            incomingData = 0;
            break;

        case 'b': // Backward
            t = 0;
            myramp.go(speed, acceltime, LINEAR, ONCEFORWARD);
            bk(t);
            prevdata = incomingData;
            incomingData = 0;
            break;

        case 's': // Stop
            t = 0;
            myramp.go(stopspeed, deceltime, LINEAR, ONCEFORWARD);
            if (prevdata == 'f')
            {
                fd(t);
            }
            else if (prevdata == 'l')
            {
                lt(t);
            }
            else if (prevdata == 'r')
            {
                rt(t);
            }
            else if (prevdata == 'b')
            {
                bk(t);
            }
            prevdata = prevdata;
            incomingData = 0;
            break;

        default: // If no valid command is received
            // Optionally handle unexpected input
            break;
        }
    }else if (obstacledetected == true){
        if(incomingData == 'b'){
             t = 0;
            myramp.go(speed, acceltime, LINEAR, ONCEFORWARD);
            bk(t);
            prevdata = incomingData;
            incomingData = 0;
        }else{
            incomingData = 's';
            prevdata = 's';
            t = 0;
            StopMot();
        }
        
        

        
    }
    
        // StopMot();  // Stop motors if an obstacle is detected
    
    if (!obstacledetected)
    {

        if (prevdata == 'f')
        {
            fd(t);
        }
        else if (prevdata == 'l')
        {
            lt(t);
        }
        else if (prevdata == 'r')
        {
            rt(t);
        }
        else if (prevdata == 'b')
        {
            bk(t);
        }else if (prevdata == 's'){
            if (prevdata == 'f')
            {
                fd(t);
            }
            else if (prevdata == 'l')
            {
                lt(t);
            }
            else if (prevdata == 'r')
            {
                rt(t);
            }
            else if (prevdata == 'b')
            {
                bk(t);
            }
        }
    }
    
    
   
}

void recvWithEndMarker()
{
    if (BT.available() > 0)
    {
        incomingData = BT.read();
        // btpidv(incomingData);
        //Serial.println(incomingData);
    }
}