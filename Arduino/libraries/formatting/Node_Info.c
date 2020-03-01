#include "Node_Info.h"

int8_t node_info(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t status[],
  uint8_t software_version[],
  uint8_t hardware_version[],
  uint8_t name[]
)
{
  if((offset + NODE_INFO_MESSAGE_SIZE) > sizeof(buffer))
  {
    return -1;
  }

  for(int n = 0; n < sizeof(status); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &status[n]);
    offset += UINT8;
  }
  for(int n = 0; n < NODE_STATUS_MESSAGE_SIZE - sizeof(status); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, NULL);
    offset += UINT8;
  }

  for(int n = 0; n < sizeof(software_version); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &software_version[n]);
    offset += UINT8;
  }
  for(int n = 0; n < SOFTWARE_VERSION_MESSAGE_SIZE-sizeof(software_version); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, NULL);
    offset += UINT8;
  }

  for(int n = 0; n < sizeof(hardware_version); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &hardware_version[n]);
    offset += UINT8;
  }
  for(int n = 0; n < HARDWARE_VERSION_MESSAGE_SIZE-sizeof(hardware_version); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, NULL);
    offset += UINT8;
  }

  for(int n = 0; n < sizeof(name); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &name[n]);
    offset += UINT8;
  }

  return 0;
}
