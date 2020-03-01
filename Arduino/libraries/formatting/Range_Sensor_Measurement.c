#include "Range_Sensor_Measurement.h"

int8_t range_sensor(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t usec[],
  uint8_t sensor_id,
  uint8_t coarse_orientation[],
  float16_t field_of_view,
  uint5_t sensor_type,
  float16_t range
)
{
  if((offset + RANGE_SENSOR_MEASUREMENT_MESSAGE_SIZE) > sizeof(buffer))
  {
    return -1;
  }

  for(int n = 0; n < sizeof(usec); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &usec[n]);
    offset += UINT8;
  }
  for(int n = 0; n < NODE_TIMESTAMP_MESSAGE_SIZE - sizeof(usec); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, NULL);
    offset += UINT8;
  }

  canardEncodeScalar(buffer, offset, UINT8, sensor_id);
  offset += UINT8;

  for(int n = 0; n < sizeof(coarse_orientation); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &coarse_orientation[n]);
    offset += UINT8;
  }
  for(int n = 0; n < COARSE_OREINTATION_MESSAGE_SIZE - sizeof(coarse_orientation); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, NULL);
    offset += UINT8;
  }

  canardEncodeScalar(buffer, offset, FLOAT16, &field_of_view);
  offset += FLOAT16;

  canardEncodeScalar(buffer, offset, UINT5, &sensor_type);
  offset += UINT5;

  canardEncodeScalar(buffer, offset, FLOAT16, &range);
  offset += FLOAT16;

  return 0;
}
