#include "AHRS_Solution.h"

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
)
{
  if((offset+AHRS_SOLUTION_MESSAGE_SIZE) > sizeof(buffer))
  {
    return -1;
  }

  if(sizeof(timestamp) > NODE_TIMESTAMP_MESSAGE_SIZE)
  {
    return -1;
  }

  for(int n = 0; n < sizeof(timestamp); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &timestamp[n]);
    offset += 8;
  }
  for(int n = 0; n < NODE_TIMESTAMP_MESSAGE_SIZE-sizeof(timestamp); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, NULL);
    offset += 8;
  }

  canardEncodeScalar(buffer, offset, FLOAT16, &orientation_x);
  offset += FLOAT16;

  canardEncodeScalar(buffer, offset, FLOAT16, &orientation_y);
  offset += FLOAT16;

  canardEncodeScalar(buffer, offset, FLOAT16, &orientation_z);
  offset += FLOAT16;

  canardEncodeScalar(buffer, offset, FLOAT16, &orientation_w);
  offset += FLOAT16;

  canardEncodeScalar(buffer, offset, VOID4, NULL);
  offset += VOID4;

  for(int n = 0; n < 9; n++)
  {
    canardEncodeScalar(buffer, offset, FLOAT16, NULL);
    offset += FLOAT16;
  }

  canardEncodeScalar(buffer, offset, FLOAT16, &angular_velocity_x);
  offset += FLOAT16;

  canardEncodeScalar(buffer, offset, FLOAT16, &angular_velocity_y);
  offset += FLOAT16;

  canardEncodeScalar(buffer, offset, FLOAT16, &angular_velocity_z);
  offset += FLOAT16;

  canardEncodeScalar(buffer, offset, VOID4, NULL);
  offest += 4;

  for(int n = 0; n < 9; n++)
  {
    canardEncodeScalar(buffer, offset, FLOAT16, NULL);
    offset += FLOAT16;
  }

  canardEncodeScalar(buffer, offset, FLOAT16, &linear_acceleration_x);
  offset += FLOAT16;

  canardEncodeScalar(buffer, offset, FLOAT16, &linear_acceleration_y);
  offset += FLOAT16;

  canardEncodeScalar(buffer, offset, FLOAT16, &linear_acceleration_z);
  offset += FLOAT16;

  for(int n = 0; n < 9; n++)
  {
    canardEncodeScalar(buffer, offset, FLOAT16, NULL);
    offset += FLOAT16;
  }

  return 0;
}
