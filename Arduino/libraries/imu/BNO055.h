#ifndef BNO055_H
#define BNO055_H

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

bool init_sensors(void) {
  if(!bno.begin()) {
    return false;
  }
  bno.setExtCrystalUse(true);

  return true;
}

void setup_sensors(void) {}

#endif // BNO055_H
