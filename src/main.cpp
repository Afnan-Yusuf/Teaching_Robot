#include <Arduino.h>
#include <TaskScheduler.h>
#include "eyeblink.h"
#include "motorinit.h"
#include "blutooth.h"
#include "scheduler.h"
#include "ultrasonic.h"
#include "neopixel.h"
#include "arduinointerface.h"
#include "headservo.h"
#include "ct6b.h"



void setup() {
  Serial.begin(115200);
  initbt();
  initeyedisplay();
  //InitMot();
  initsharpir();
  headservoinit();
  rgbledinit();
  //initserial2();
  initct6b();

  ts.addTask(eyeblinktask);
  ts.addTask(bttask);
  ts.addTask(readsensortask);
  
  eyeblinktask.enable();
  //bttask.enable();
  readsensortask.enable();
  initmottask.enableDelayed(2000);
  //servosweeptask.enable();


}

void loop() {
  ts.execute();
 // arduinointerface();
  rgbledcontrol();
  runonct6b();
}
