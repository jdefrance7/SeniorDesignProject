#ifndef NXP_FXOS_FXAS_H
#define NXP_FXOS_FXAS_H

/* Select Filter */
// #define FILTER_NXP_SENSOR_FUSION
#define FILTER_MADGWICK
// #define FILTER_MAHONY

/* Define Filter Update Rate */
#define FILTER_UPDATE_RATE_HZ 100

#include "imu_constants.h"

#include <Adafruit_FXAS21002C.h>
#include <Adafruit_FXOS8700.h>

#include <Adafruit_Sensor_Calibration.h>
#include <Adafruit_AHRS.h>

extern Adafruit_FXOS8700 fxos;
extern Adafruit_FXAS21002C fxas;

extern Adafruit_Sensor *accelerometer;
extern Adafruit_Sensor *gyroscope;
extern Adafruit_Sensor *magnetometer;

// Initialization Function
bool init_imu(void);

// Update Function
int16_t update_imu(void);

// Data Aquisition
float orientation(uint8_t rotational_axis);
float quaternion(uint8_t rotational_axis);
float angularVelocity(uint8_t rotational_axis);
float linearAcceleration(uint8_t linear_axis);
float magneticField(uint8_t linear_axis);
float temperature(uint8_t temperature_scale);

#if defined(FILTER_NXP_SENSOR_FUSION)
  extern Adafruit_NXPSensorFusion filter;
#elif defined(FILTER_MADGWICK)
  extern Adafruit_Madgwick filter;
#elif defined(FILTER_MAHONY)
  extern Adafruit_Mahony filter;
#endif

#if defined(ADAFRUIT_SENSOR_CALIBRATION_USE_EEPROM)
  extern Adafruit_Sensor_Calibration_EEPROM cal;
#else
  extern Adafruit_Sensor_Calibration_SDFat cal;
#endif

#endif // NXP_FXOS_FXAS_H
