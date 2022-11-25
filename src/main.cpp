#include <Arduino.h>

extern "C"{
#include <SCIMaster.h>
}

void setup() {
  SCIMasterInit((tsSCI_MASTER_CALLBACKS){NULL});
}

void loop() {
  // put your main code here, to run repeatedly:
}