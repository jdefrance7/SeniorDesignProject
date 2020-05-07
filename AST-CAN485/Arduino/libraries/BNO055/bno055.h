/**
 * @file bno055.h
 *
 * Standardized interface for the Adafruit BNO055.
 *
 * @author Joe DeFrance
 */

#ifndef BNO055_H
#define BNO055_H

/* Define Filter Update Rate */
#define FILTER_UPDATE_RATE_HZ 1

#include "imu_constants.h"

#include <Adafruit_BNO055.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <utility/imumaths.h>

// Sensor Instance
extern Adafruit_BNO055 bno;

// Initialization Function
int init_imu(void);

// Update Function
int update_imu(void);

// Data Aquisition
float orientation(uint8_t rotational_axis);
float quaternion(uint8_t rotational_axis);
float angularVelocity(uint8_t rotational_axis);
float linearAcceleration(uint8_t linear_axis);
float magneticField(uint8_t linear_axis);
float temperature(uint8_t temperature_scale);

#endif // BNO055_H
