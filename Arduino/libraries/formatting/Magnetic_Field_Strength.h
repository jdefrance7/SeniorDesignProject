#ifndef MAGNETIC_FIELD_STRENGTH_H
#define MAGNETIC_FIELD_STRENGTH_H

// DSDL Constants
#include "dsdl.h"

// UAVCAN Formatting
#include <canard.h>

#define MAGNETIC_FIELD_STRENGTH_MESSAGE_SIZE           (204 / 8)
#define MAGNETIC_FIELD_STRENGTH_DATA_TYPE_SIGNATURE    0xb6ac0c442430297e
#define MAGNETIC_FIELD_STRENGTH_DATA_TYPE_ID           1002

#ifdef __cplusplus
extern "C" {
#endif

int8_t magnetic_field_strength(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t sensor_id,
  float16_t magnetic_field_x,
  float16_t magnetic_field_y,
  float16_t magnetic_field_z
);

#ifdef __cplusplus
}
#endif

/* MagneticFieldStrength2 DSDL

Full name: uavcan.equipment.ahrs.MagneticFieldStrength2
Default data type ID: 1002

#
# Magnetic field readings, in Gauss, in body frame.
# SI units are avoided because of float16 range limitations.
#

uint8 sensor_id

float16[3] magnetic_field_ga
float16[<=9] magnetic_field_covariance

*/

#endif // MAGNETIC_FIELD_STRENGTH_H
