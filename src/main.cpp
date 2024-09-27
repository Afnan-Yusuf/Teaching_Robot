#include <Arduino.h>
#include "eyeblink.h"
#include "motorinit.h"
#include "ultrasonic.h"
#include "blutooth.h"
#include <TaskScheduler.h>

Scheduler ts; 
Task eyeblinktask(TASK_IMMEDIATE, TASK_FOREVER, &eyeblink, &ts, false);
Task bttask(TASK_IMMEDIATE, TASK_FOREVER, &btcontrol, &ts, false);
Task readsensortask(TASK_IMMEDIATE, TASK_FOREVER, &obstacledetect, &ts, false);


void setup() {
  Serial.begin(115200);
  initbt();
  initeyedisplay();
  InitMot();
  initsharpir();

  ts.addTask(eyeblinktask);
  ts.addTask(bttask);
  //eyeblinktask.enable();
  bttask.enable();


}

void loop() {
  ts.execute();
  //obstacledeteect();
}
