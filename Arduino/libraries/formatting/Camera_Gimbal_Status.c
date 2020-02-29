#include "Camera_Gimbal_Status.h"

int8_t camera_gimbal_status(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t gimbal_id,
  uint8_t camera_mode,
  float16_t camera_orientation_x,
  float16_t camera_orientation_y,
  float16_t camera_orientation_z,
  float16_t camera_orientation_w
)
{
  if((offset + CAMERA_GIMBAL_STATUS_MESSAGE_SIZE) > sizeof(buffer))
  {
    return -1;
  }

  canardEncodeScalar(buffer, offset, UINT8, &gimbal_id);
  offset += UINT8;

  canardEncodeScalar(buffer, offset, UINT8, &camera_mode);
  offset += UINT8;

  canardEncodeScalar(buffer, offset, FLOAT16, &camera_orientation_x);
  offset += UINT16;

  canardEncodeScalar(buffer, offset, FLOAT16, &camera_orientation_y);
  offset += UINT16;

  canardEncodeScalar(buffer, offset, FLOAT16, &camera_orientation_z);
  offset += UINT16;

  canardEncodeScalar(buffer, offset, FLOAT16, &camera_orientation_w);
  offset += UINT16;

  for(int n = 0; n < 9; n++)
  {
    canardEncodeScalar(buffer, offset, FLOAT16, NULL);
    offset += FLOAT16;
  }

  return 0;
}
