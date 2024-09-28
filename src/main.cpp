#include <Arduino.h>
#include <TaskScheduler.h>
#include "eyeblink.h"
#include "motorinit.h"
#include "blutooth.h"
#include "scheduler.h"
#include "ultrasonic.h"
#include "neopixel.h"



void setup() {
  Serial.begin(115200);
  initbt();
  initeyedisplay();
  InitMot();
  initsharpir();
  void rgbledinit();

  ts.addTask(eyeblinktask);
  ts.addTask(bttask);
  ts.addTask(readsensortask);
  
  //eyeblinktask.enable();
  bttask.enable();
  readsensortask.enable();
  eyeblinktask.enable();


}

void loop() {
  ts.execute();
}
