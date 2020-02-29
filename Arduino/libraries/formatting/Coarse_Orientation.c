#include "Coarse_Orientation.h"

int8_t coarse_orientation(
  uint8_t buffer[],
  uint8_t offset,
  int8_t roll,
  int8_t pitch,
  int8_t yaw,
  bool orientation_defined
)
{
  if((offset + COARSE_ORIENTATION_MESSAGE_SIZE) > sizeof(buffer))
  {
    return -1;
  }

  canardEncodeScalar(buffer, offset, INT5, &roll);
  offset += INT5;

  canardEncodeScalar(buffer, offset, INT5, &pitch);
  offset += INT5;

  canardEncodeScalar(buffer, offset, INT5, &yaw);
  offset += INT5;

  canardEncodeScalar(buffer, offset, BOOL, &orientation_defined);
  offset += BOOL;

  return 0;
}
