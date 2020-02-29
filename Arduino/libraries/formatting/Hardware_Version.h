#ifndef HARDWARE_VERSION_H
#define HARDWARE_VERSION_H

// DSDL Constants
#include "dsdl.h"

// UAVCAN Formatting
#include <canard.h>

#define HARDWARE_VERSION_MESSAGE_SIZE             (2192 / 8)
#define HARDWARE_VERSION_DATA_TYPE_SIGNATURE      0x0ad5c4c933f4a0c4
#define HARDWARE_VERSION_DATA_TYPE_ID             DATA_TYPE_NA

#define HARDWARE_VERSION_UNIQUE_ID_SIZE                     16
#define HARDWARE_VERSION_CERTIFICATE_OF_AUTHENTICITY_SIZE   255

int8_t hardware_version(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t major,
  uint8_t minor,
  uint8_t unique_id[],
  uint8_t certificate_of_authenticity[]
);

/* HardwareVersion DSDL

Full name: uavcan.protocol.HardwareVersion

#
# Nested type.
# Generic hardware version information.
# These values should remain unchanged for the device's lifetime.
#

#
# Hardware version code.
#
uint8 major
uint8 minor

#
# Unique ID is a 128 bit long sequence that is globally unique for each node.
# All zeros is not a valid UID.
# If filled with zeros, assume that the value is undefined.
#
uint8[16] unique_id

#
# Certificate of authenticity (COA) of the hardware, 255 bytes max.
#
uint8[<=255] certificate_of_authenticity

*/

#endif // HARDWARE_VERSION_H
