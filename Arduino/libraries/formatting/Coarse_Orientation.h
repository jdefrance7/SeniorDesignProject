#ifndef COARSE_ORIENTATION_H
#define COARSE_ORIENTATION_H

// DSDL Constants
#include "dsdl.h"

// UAVCAN Formatting
#include <canard.h>

#define COARSE_ORIENTATION_MESSAGE_SIZE             (16 / 8)
#define COARSE_ORIENTATION_DATA_TYPE_SIGNATURE      0x271ba10b0dac9e52
#define COARSE_ORIENTATION_DATA_TYPE_ID             DATA_TYPE_NA

#define COARSE_ORIENTATION_ANGLE_MULTIPLIER         4.7746482927568605

#ifdef __cplusplus
extern "C" {
#endif

int8_t coarse_orientation(
  uint8_t buffer[],
  uint8_t offset,
  int8_t roll,
  int8_t pitch,
  int8_t yaw,
  bool orientation_defined
);

#ifdef __cplusplus
}
#endif


/* CoarseOrientation DSDL

Full name: uavcan.CoarseOrientation

#
# Nested type.
# Coarse, low-resolution 3D orientation represented as fixed axes in 16 bit.
#
# Roll, pitch, yaw angles in radians should be multiplied by
# ANGLE_MULTIPLIER in order to convert them to the coarse representation.
#
# ANGLE_MULTIPLIER = NORM / PI
#
# Where NORM is 12, because it:
#  - Fits the maximum range of a signed 5 bit integer
#  - Allows to exactly represent the following angles:
#    0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180, and negatives
#

float32 ANGLE_MULTIPLIER = 4.7746482927568605

int5[3] fixed_axis_roll_pitch_yaw

bool orientation_defined    # False if the orientation is actually not defined

*/

#endif // COARSE_ORIENTATION_H
