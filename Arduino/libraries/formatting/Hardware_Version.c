#include "Hardware_Version.h"

int8_t hardware_version(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t major,
  uint8_t minor,
  uint8_t unique_id[],
  uint8_t certificate_of_authenticity[]
)
{
  if((offset + HARDWARE_VERSION_MESSAGE_SIZE) > sizeof(buffer))
  {
    return -1;
  }

  canardEncodeScalar(buffer, offset, UINT8, &major);
  offset += UINT8;

  canardEncodeScalar(buffer, offset, UINT8, &minor);
  offset += UINT8;

  for(int n = 0; n < sizeof(unique_id); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &unique_id[n]);
    offset += UINT8;
  }
  for(int n = 0; n < HARDWARE_VERSION_UNIQUE_ID_SIZE-sizeof(unique_id); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, NULL);
    offset += UINT8;
  }

  for(int n = 0; n < sizeof(certificate_of_authenticity); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &certificate_of_authenticity[n]);
    offset += UINT8;
  }
  for(int n = 0; n < HARDWARE_VERSION_CERTIFICATE_OF_AUTHENTICITY_SIZE-sizeof(certificate_of_authenticity); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, NULL);
    offset += UINT8;
  }

  return 0;
}
