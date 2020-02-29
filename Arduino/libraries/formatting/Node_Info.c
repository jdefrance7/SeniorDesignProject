#include "Node_Info.h"

int8_t node_info(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t status[NODE_STATUS_MESSAGE_SIZE],
  uint8_t software_version[SOFTWARE_VERSION_MESSAGE_SIZE],
  uint8_t hardware_version[HARDWARE_VERSION_MESSAGE_SIZE],
  uint8_t name[]
)
{
  if((offset + NODE_INFO_MESSAGE_SIZE) > sizeof(buffer))
  {
    return -1;
  }

  for(int n = 0; n < NODE_STATUS_MESSAGE_SIZE; n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &status[n]);
    offset += UINT8;
  }

  for(int n = 0; n < SOFTWARE_VERSION_MESSAGE_SIZE; n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &software_version[n]);
    offset += UINT8;
  }

  for(int n = 0; n < HARDWARE_VERSION_MESSAGE_SIZE; n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &hardware_version[n]);
    offset += UINT8;
  }

  for(int n = 0; n < sizeof(name); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &name[n]);
    offset += UINT8;
  }

  return 0;
}
