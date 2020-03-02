#ifndef BNO055_H
#define BNO055_H

#if defined(SENSOR_BNO055)

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

extern Adafruit_BNO055 bno;

bool init_sensors(void);

void setup_sensors(void);

#endif

#endif // BNO055_H
