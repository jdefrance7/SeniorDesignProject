#include "Restart_Node.h"

int8_t restart_node(
  uint8_t buffer[],
  uint16_t offset,
  bool ok
)
{
  if((offset + RESTART_NODE_MESSAGE_SIZE) > sizeof(buffer))
  {
    return -1;
  }
  
  canardEncodeScalar(buffer, offset, BOOK, &ok);
  offset += BOOL;

  return 0;
}
