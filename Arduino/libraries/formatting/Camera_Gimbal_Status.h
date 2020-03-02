#ifndef CAMERA_GIMBAL_STATUS_H
#define CAMERA_GIMBAL_STATUS_H

// DSDL Constants
#include "dsdl.h"

// UAVCAN Formatting
#include <canard.h>

#define CAMERA_GIMBAL_STATUS_MESSAGE_SIZE           (228 / 8)
#define CAMERA_GIMBAL_STATUS_DATA_TYPE_SIGNATURE    0xb9f127865be0d61e
#define CAMERA_GIMBAL_STATUS_DATA_TYPE_ID           1044

#define CAMERA_MODE_ANGULAR_VELOCITY          0
#define CAMERA_MODE_ORIENTATION_FIXED_FRAME   1
#define CAMERA_MODE_ORIENTATION_BODY_FRAME    2
#define CAMERA_MODE_GEO_POI                   4

#define CAMERA_ORIENTATION_SIZE               4

#ifdef __cplusplus
extern "C" {
#endif

int8_t camera_gimbal_status(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t gimbal_id,
  uint8_t camera_mode,
  float16_t camera_orientation_x,
  float16_t camera_orientation_y,
  float16_t camera_orientation_z,
  float16_t camera_orientation_w
);

#ifdef __cplusplus
}
#endif


/* camera_gimbal.Status DSDL

Full name: uavcan.equipment.camera_gimbal.Status
Default data type ID: 1044

#
# Generic gimbal status.
#

uint8 gimbal_id

Mode mode

#
# Camera axis orientation in body frame (not in fixed frame).
# Please refer to the UAVCAN coordinate frame conventions.
#
float16[4] camera_orientation_in_body_frame_xyzw
float16[<=9] camera_orientation_in_body_frame_covariance   # +inf for non-existent axes

*/

/* camera_gimbal_Mode DSDL

Full name: uavcan.equipment.camera_gimbal.Mode

#
# Gimbal operating mode
#

uint8 COMMAND_MODE_ANGULAR_VELOCITY        = 0
uint8 COMMAND_MODE_ORIENTATION_FIXED_FRAME = 1
uint8 COMMAND_MODE_ORIENTATION_BODY_FRAME  = 2
uint8 COMMAND_MODE_GEO_POI                 = 3
uint8 command_mode

*/

#endif // CAMERA_GIMBAL_STATUS_H
