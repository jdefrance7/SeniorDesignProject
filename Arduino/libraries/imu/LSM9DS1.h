#ifndef LSM9DS1_H
#define LSM9DS1_H

#if defined(SENSOR_LSM9DS1)

#include <Adafruit_LSM9DS1.h>

#include <Adafruit_Sensor_Calibration.h>
#include <Adafruit_AHRS.h>

extern Adafruit_LSM9DS1 lsm9ds1;

extern Adafruit_Sensor *accelerometer;
extern Adafruit_Sensor *gyroscope;
extern Adafruit_Sensor *magnetometer;

bool init_sensors(void);

void setup_sensors(void);

#endif

#endif // LSM9DS1_H
