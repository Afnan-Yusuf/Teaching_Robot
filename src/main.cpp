#include <Arduino.h>
#include "eyeblink.h"
#include "motorinit.h"
#include "ultrasonic.h"
#include "blutooth.h"

void setup() {
  initbt();
  initeyedisplay();
  InitMot();
  initsharpir();
}

void loop() {
  eyeblink();
  fd(100);
  obstacledeteect();
}

