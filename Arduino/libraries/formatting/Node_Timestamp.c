#include "Node_Timestamp.h"

int8_t node_timestamp(
  uint8_t buffer[],
  uint8_t offset,
  uint64_t usec
)
{
  if((offset + NODE_TIMESTAMP_MESSAGE_SIZE) > sizeof(buffer))
  {
    return -1;
  }

  canardEncodeScalar(buffer, offset, UINT56, &usec);
  offset += UINT56;

  return 0;
}
