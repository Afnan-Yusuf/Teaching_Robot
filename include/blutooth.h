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

    switch (incomingData)
    {
    case 'f': // Forward
        if (obstacledetected == false)
        {
            t = 0;
            myramp.go(speed, acceltime, LINEAR, ONCEFORWARD);
            fd(t);
            prevdata = incomingData;
            incomingData = 0;
        }
        break;

    case 'l': // Left turn
        if (obstacledetected == false)
        {
            t = 0;
            myramp.go(speed, acceltime, LINEAR, ONCEFORWARD);
            lt(t);
            prevdata = incomingData;
            incomingData = 0;
        }
        break;

    case 'r': // Right turn
        if (obstacledetected == false)
        {
            t = 0;
            myramp.go(speed, acceltime, LINEAR, ONCEFORWARD);
            rt(t);
            prevdata = incomingData;
            incomingData = 0;
        }
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
    if (obstacledetected == true)
    {

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
    }

    // StopMot();  // Stop motors if an obstacle is detected

    

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
        else if (prevdata == 's')
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
            }
        }
    
}

void recvWithEndMarker()
{
    if (BT.available() > 0)
    {
        incomingData = BT.read();
        // btpidv(incomingData);
        // Serial.println(incomingData);
    }
}