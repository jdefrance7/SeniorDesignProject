#include "Device_Temperature.h"

int8_t device_temperature(
  uint8_t buffer[],
  uint16_t offset,
  uint16_t device_id,
  float16_t temperature,
  uint8_t error_flags
)
{
  if((offset + DEVICE_TEMPERATURE_MESSAGE_SIZE) > sizeof(buffer))
  {
    return -1;
  }

  canardEncodeScalar(buffer, offset,  UINT16, &device_id);
  offset += UINT16;

  canardEncodeScalar(buffer, offset, FLOAT16, &temperature);
  offset += FLOAT16;

  canardEncodeScalar(buffer, offset,   UINT8, &error_flags);
  offset += UINT8;

  return 0;
}
