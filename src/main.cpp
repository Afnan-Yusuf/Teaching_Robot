#include <Arduino.h>
#include "eyeblink.h"
#include "motorinit.h"

void setup() {
  initeyedisplay();
  InitMot();
}

void loop() {
  eyeblink();
  fd(100);
}

