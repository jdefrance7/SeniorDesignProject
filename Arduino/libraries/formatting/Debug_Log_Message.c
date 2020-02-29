#include "Debug_Log_Message.h"

int8_t debug_log_message(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t level,
  uint8_t source[],
  uint8_t text[]
)
{
  if((offset + DEBUG_LOG_MESSAGE_MESSAGE_SIZE) > sizeof(buffer))
  {
    return -1;
  }

  canardEncodeScalar(buffer, offset, UINT8, &level);
  offset += UINT8;

  for(int n = 0; n < sizeof(source); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &source[n]);
    offset += UINT8;
  }
  for(int n = 0; n < DEBUG_SOURCE_SIZE-sizeof(source); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, NULL);
    offset += UINT8;
  }

  for(int n = 0; n < sizeof(text); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &text[n]);
    offset += UINT8;
  }
  for(int n = 0; n < DEBUG_TEXT_SIZE-sizeof(text); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, NULL);
    offset += UINT8;
  }

  return 0;
}
