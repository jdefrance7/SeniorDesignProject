#include "Node_Status.h"

int8_t node_status(
  uint8_t buffer[],
  uint16_t offset,
  uint32_t uptime_sec,
  uint8_t health,
  uint8_t mode
)
{
  if((offset + NODE_STATUS_MESSAGE_SIZE) > sizeof(buffer))
  {
    return -1;
  }

  canardEncodeScalar(buffer, offset, UINT32, &uptime_sec);
  offset += UINT32;

  canardEncodeScalar(buffer, offset, UINT2, &health);
  offset += UINT2;

  canardEncodeScalar(buffer, offset, UINT3, &mode);
  offset += UINT3;

  canardEncodeScalar(buffer, offset, UINT3, NULL);
  offset += UINT3;

  canardEncodeScalar(buffer, offset, UINT16, NULL);
  offset += UINT16;

  return 0;
}
