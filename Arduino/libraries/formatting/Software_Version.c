#include "Software_Version.h"

int8_t software_version(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t major,
  uint8_t minor,
  uint8_t optional_field_flags,
  uint32_t vcs_commit,
  uint64_t image_crc
)
{
  if((offset + SOFTWARE_VERSION_MESSAGE_SIZE) > sizeof(buffer))
  {
    return -1;
  }

  canardEncodeScalar(buffer, offset, UINT8, &major);
  offset += UINT8;

  canardEncodeScalar(buffer, offset, UINT8, &minor);
  offset += UINT8;

  canardEncodeScalar(buffer, offset, UINT8, &optional_field_flags);
  offset += UINT8;

  canardEncodeScalar(buffer, offset, UINT32, &vcs_commit);
  offset += UINT32;

  canardEncodeScalar(buffer, offset, UINT64, &image_crc);
  offset += UINT64;

  return 0;
}
