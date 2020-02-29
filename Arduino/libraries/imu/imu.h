#ifndef IMU_H
#define IMU_H

//##############################################################################
// IMU CONFIGS /////////////////////////////////////////////////////////////////
//##############################################################################

/* Select Sensor */

// #define SENSOR_BNO055
// #define SENSOR_LSM9DS1
// #define SENSOR_NXP_FXOS_FXAS

/* Select Filter from below (only for LSM9DS1 & NXP_FXOS_FXAS) */

// #define FILTER_NXP_SENSOR_FUSION
// #define FILTER_MADGWICK
// #define FILTER_MAHONY

/* Select Filter Update Rate (only for LSM9DS1 & NXP_FXOS_FXAS) */

// #define FILTER_UPDATE_RATE_HZ 100

//##############################################################################
// CONSTANTS ///////////////////////////////////////////////////////////////////
//##############################################################################

// Linear Axis Designators
#define X_AXIS      1
#define Y_AXIS      2
#define Z_AXIS      3
#define W_AXIS      4

// Rotational Axis Designators
#define ROLL_AXIS   1
#define PICTH_AXIS  2
#define YAW_AXIS    3

// Temperature Scales
#define TEMPERATURE_CELSIUS   1
#define TEMPERATURE_KELVIN    2

//##############################################################################
// USER FUNCTIONS //////////////////////////////////////////////////////////////
//##############################################################################

// Hardware Support Functions
bool init_imu(void);
bool update_imu(void);

// Data Aquisition
float16_t orientation(uint8_t rotational_axis);
float16_t quaternion(uint8_t rotational_axis);
float16_t angularVelocity(uint8_t rotational_axis);
float16_t linearAcceleration(uint8_t linear_axis);
float16_t magneticField(uint8_t linear_axis);
float16_t temperature(uint8_t temperature_scale);

//##############################################################################
// SUPPORTING LIBRARIES ////////////////////////////////////////////////////////
//##############################################################################

// General Arduino Library
#include <Arduino.h>

// Supporting Libraries & Objects
#if defined(SENSOR_LSM9DS1 || SENSOR_NXP_FXOS_FXAS)

  #include <Adafruit_Sensor_Calibration.h>
  #include <Adafruit_AHRS.h>

  Adafruit_Sensor *accelerometer, *gyroscope, *magnetometer;

#endif // Supporting Libraries & Objects

//##############################################################################
// SENSOR OBJECTS //////////////////////////////////////////////////////////////
//##############################################################################

// Sensor Object
#if defined(SENSOR_BNO055)

  #include "BNO055.h"

#elif defined(SENSOR_LSM9DS1)

  #include "LSM9DS1.h"

#elif defined(SENSOR_NXP_FXOS_FXAS)

  #include "NXP_FXOS_FXAS.h"

#endif // Sensor Object

//##############################################################################
// FILTER OBJECTS //////////////////////////////////////////////////////////////
//##############################################################################

// Filter Object
#if defined(SENSOR_LSM9DS1 | SENSOR_NXP_FXOS_FXAS)

  #if defined(FILTER_NXP_SENSOR_FUSION)

    Adafruit_NXPSensorFusion filter;

  #elif defined(FILTER_MADGWICK)

    Adafruit_Madgwick filter;

  #elif defined(FILTER_MAHONY)

    Adafruit_Mahony filter;

  #endif

#endif // Filter Object

//##############################################################################
// CALIBRATION OBJECTS /////////////////////////////////////////////////////////
//##############################################################################

// Calibration Object
#if defined(SENSOR_LSM9DS1 | SENSOR_NXP_FXOS_FXAS)

  #if defined(ADAFRUIT_SENSOR_CALIBRATION_USE_EEPROM)

    Adafruit_Sensor_Calibration_EEPROM cal;

  #else

    Adafruit_Sensor_Calibration_SDFat cal;

  #endif

#endif // Calibration Object

//##############################################################################
// END /////////////////////////////////////////////////////////////////////////
//##############################################################################

#endif // IMU_H
