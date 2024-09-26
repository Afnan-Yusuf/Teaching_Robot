#include <Arduino.h>
#include "eyeblink.h"

// put function declarations here:
int myFunction(int, int);

void setup() {
  initeyedisplay();
}

void loop() {
  eyeblink();
}

