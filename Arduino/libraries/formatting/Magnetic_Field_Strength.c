#include "Magnetic_Field_Strenth.h"

int8_t magnetic_field_strength(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t sensor_id,
  float16_t magnetic_field_x,
  float16_t magnetic_field_y,
  float16_t magnetic_field_z
)
{
  if((offset + MAGNETIC_FIELD_STRENGTH_MESSAGE_SIZE) > sizeof(buffer))
  {
    return -1;
  }

  canardEncodeScalar(buffer, offset, UINT8,   &sensor_id);
  offset += UINT8;

  canardEncodeScalar(buffer, offset, FLAOT16, &magnetic_field_x);
  offset += FLOAT16;

  canardEncodeScalar(buffer, offset, FLOAT16, &magnetic_field_y);
  offset += FLOAT16;

  canardEncodeScalar(buffer, offset, FLOAT16, &magnetic_field_z);
  offset += FLOAT16;

  for(int n = 0; n < 9; n++)
  {
    canardEncodeScalar(buffer, offset, FLOAT16, NULL);
    offset += FLOAT16;
  }

  return 0;
}
