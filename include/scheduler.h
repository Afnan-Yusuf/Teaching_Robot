
#pragma once
#include <Arduino.h>
#include <TaskScheduler.h>
#include "eyeblink.h"
#include "motorinit.h"
#include "ultrasonic.h"
#include "blutooth.h"
#include "headservo.h"
Scheduler ts; 
Task eyeblinktask(TASK_IMMEDIATE, TASK_FOREVER, &eyeblink, &ts, false);
Task bttask(TASK_IMMEDIATE, TASK_FOREVER, &btcontrol, &ts, false, NULL, &StopMot);
Task readsensortask(TASK_IMMEDIATE, TASK_FOREVER, &obstacledetect, &ts, false);
Task initmottask(TASK_IMMEDIATE, TASK_ONCE, &InitMot, &ts, false);
Task servosweeptask(TASK_IMMEDIATE, TASK_FOREVER, &headservoSweep, &ts, false);