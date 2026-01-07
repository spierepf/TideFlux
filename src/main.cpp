#include <Arduino.h>

#include "wifi.hpp"

#ifndef SERIAL_SPEED
#define SERIAL_SPEED 9600
#endif

void setup() {
  // put your setup code here, to run once:
  Serial.begin(SERIAL_SPEED);

  wifi::setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  wifi::loop();
}
