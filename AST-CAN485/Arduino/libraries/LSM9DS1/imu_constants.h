/**
 * @file imu_constants.h
 *
 * IMU constants.
 *
 * @author Joe DeFrance
 */


#ifndef IMU_CONSTANTS_H
#define IMU_CONSTANTS_H

// Linear Axis
#define X_AXIS      0
#define Y_AXIS      1
#define Z_AXIS      2
#define W_AXIS      3

// Rotational Axis
#define ROLL_AXIS   0
#define PITCH_AXIS  1
#define YAW_AXIS    2

// Temperature Scale
#define TEMPERATURE_CELSIUS   0
#define TEMPERATURE_KELVIN    1

// Return Values
#define IMU_SUCCESS             0
#define ERR_SENSOR_FAILURE      -1
#define ERR_CALIBRATION_BEGIN   -2
#define ERR_CALIBRATION_LOAD    -3

#endif // IMU_CONSTANTS_H
