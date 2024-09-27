#include <Arduino.h>
#include <TaskScheduler.h>
#include "eyeblink.h"
#include "motorinit.h"
#include "blutooth.h"
#include "scheduler.h"
#include "ultrasonic.h"



void setup() {
  Serial.begin(115200);
  initbt();
  initeyedisplay();
  InitMot();
  initsharpir();

  ts.addTask(eyeblinktask);
  ts.addTask(bttask);
  
  eyeblinktask.enable();
  bttask.enable();


}

void loop() {
  ts.execute();
}
