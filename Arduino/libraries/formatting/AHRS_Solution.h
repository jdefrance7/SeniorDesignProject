#ifndef AHRS_SOLUTION_H
#define AHRS_SOLUTION_H

// DSDL Constants
#include "dsdl.h"

// UAVCAN Formatting
#include <canard.h>

// Other Standard Data Types
#include "Node_Timestamp.h"

#define AHRS_SOLUTION_MESSAGE_SIZE           (668 / 8)
#define AHRS_SOLUTION_DATA_TYPE_SIGNATURE    0x72a63a3c6f41fa9b
#define AHRS_SOLUTION_DATA_TYPE_ID           1000

int8_t ahrs_solution(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t timestamp[],
  float16_t orientation_x,
  float16_t orientation_y,
  float16_t orientation_z,
  float16_t orientation_w,
  float16_t angular_velocity_x,
  float16_t angular_velocity_y,
  float16_t angular_velocity_z,
  float16_t linear_acceleration_x,
  float16_t linear_acceleration_y,
  float16_t linear_acceleration_z
);

/* ahrs.Solution DSDL

Full name: uavcan.equipment.ahrs.Solution
Default data type ID: 1000

#
# Inertial data and orientation in body frame.
#

uavcan.Timestamp timestamp

#
# Normalized quaternion
#
float16[4] orientation_xyzw
void4
float16[<=9] orientation_covariance

#
# rad/sec
#
float16[3] angular_velocity
void4
float16[<=9] angular_velocity_covariance

#
# m/s^2
#
float16[3] linear_acceleration
float16[<=9] linear_acceleration_covariance

*/

#endif // AHRS_SOLUTION_H
