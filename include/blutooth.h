#pragma once
#include <Arduino.h>
#include "BluetoothSerial.h"

BluetoothSerial BT;
void initbt(){
    BT.begin("LFIR");
}