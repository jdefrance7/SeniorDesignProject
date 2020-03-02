#ifndef NXP_FXOS_FXAS_H
#define NXP_FXOS_FXAS_H

#if defined(SENSOR_NXP_FXOS_FXAS)

#include <Adafruit_FXAS21002C.h>
#include <Adafruit_FXOS8700.h>

#include <Adafruit_Sensor_Calibration.h>
#include <Adafruit_AHRS.h>

extern Adafruit_FXOS8700 fxos;
extern Adafruit_FXAS21002C fxas;

extern Adafruit_Sensor *accelerometer;
extern Adafruit_Sensor *gyroscope;
extern Adafruit_Sensor *magnetometer;

bool init_sensors(void);

void setup_sensors(void);

#endif

#endif // NXP_FXOS_FXAS_H
