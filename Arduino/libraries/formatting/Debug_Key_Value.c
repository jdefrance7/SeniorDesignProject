#include "Debug_Key_Value.h"

int8_t debug_key_value(
  uint8_t buffer[],
  uint16_t offset,
  float32_t value,
  uint8_t key[]
)
{
  if((offset + sizeof(buffer)) > DEBUG_KEY_VALUE_MESSAGE_SIZE)
  {
    return -1;
  }

  if(sizeof(key) > DEBUG_KEY_MAX_SIZE)
  {
    return -1;
  }

  canardEncodeScalar(buffer, offset, FLOAT32, &value);
  offset += FLOAT32;

  for(int n = 0; n < sizeof(key); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &key[n]);
    offset += UINT8;
  }
  for(int n = 0; DEBUG_KEY_VALUE_MESSAGE_SIZE-sizeof(buffer); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, NULL);
    offset += UINT8;
  }

  return 0;
}
