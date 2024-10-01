#include <Arduino.h>
#include <TaskScheduler.h>
#include "eyeblink.h"
#include "motorinit.h"
#include "blutooth.h"
#include "scheduler.h"
#include "ultrasonic.h"
#include "neopixel.h"
#include "arduinointerface.h"



void setup() {
  Serial.begin(115200);
  initbt();
  //initeyedisplay();
  InitMot();
  initsharpir();
  rgbledinit();
  //initserial2();

  ts.addTask(eyeblinktask);
  ts.addTask(bttask);
  ts.addTask(readsensortask);
  
  //eyeblinktask.enable();
  bttask.enable();
  readsensortask.enable();


}

void loop() {
  ts.execute();
 // arduinointerface();
}
