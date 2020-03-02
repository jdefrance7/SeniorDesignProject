#ifndef IMU_H
#define IMU_H

#include <stdint.h>

//##############################################################################
// IMU CONFIGS /////////////////////////////////////////////////////////////////
//##############################################################################

/* Select Sensor */

#define SENSOR_BNO055
// #define SENSOR_LSM9DS1
// #define SENSOR_NXP_FXOS_FXAS

/* Select Filter from below (only for LSM9DS1 & NXP_FXOS_FXAS) */

// #define FILTER_NXP_SENSOR_FUSION
// #define FILTER_MADGWICK
// #define FILTER_MAHONY

/* Select Filter Update Rate (only for LSM9DS1 & NXP_FXOS_FXAS) */

#define FILTER_UPDATE_RATE_HZ 100

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
#define PITCH_AXIS  2
#define YAW_AXIS    3

// Temperature Scales
#define TEMPERATURE_CELSIUS   1
#define TEMPERATURE_KELVIN    2

//##############################################################################
// SENSOR OBJECTS //////////////////////////////////////////////////////////////
//##############################################################################

#include "BNO055.h"
#include "LSM9DS1.h"
#include "NXP_FXOS_FXAS.h"

//##############################################################################
// USER FUNCTIONS //////////////////////////////////////////////////////////////
//##############################################################################

// Hardware Support Functions
bool init_imu(void);
int16_t update_imu(void);

// Data Aquisition
float orientation(uint8_t rotational_axis);
float quaternion(uint8_t rotational_axis);
float angularVelocity(uint8_t rotational_axis);
float linearAcceleration(uint8_t linear_axis);
float magneticField(uint8_t linear_axis);
float temperature(uint8_t temperature_scale);

#if defined(SENSOR_LSM9DS1) | defined(SENSOR_NXP_FXOS_FXAS)

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
  
#endif

//##############################################################################
// END /////////////////////////////////////////////////////////////////////////
//##############################################################################

#endif // IMU_H
