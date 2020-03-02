#include "Data_Type_Info.h"

int8_t data_type_info(
  uint8_t buffer[],
  uint16_t offset,
  uint64_t signature,
  uint16_t id,
  uint8_t kind,
  uint8_t flags,
  uint8_t name[]
)
{
  if((offset + DATA_TYPE_INFO_MESSAGE_SIZE) > sizeof(buffer))
  {
    return -1;
  }

  canardEncodeScalar(buffer, offset, UINT16, &id);
  offset += UINT16;

  canardEncodeScalar(buffer, offset, UINT8, &kind);
  offset += UINT8;

  canardEncodeScalar(buffer, offset, UINT8, &flags);
  offset += UINT8;

  for(int n = 0; n < sizeof(name); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &name[n]);
    offset += UINT8;
  }

  for(int n = 0; n < DATA_TYPE_INFO_NAME_SIZE-sizeof(name); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, NULL);
    offset += UINT8;
  }

  return 0;
}
