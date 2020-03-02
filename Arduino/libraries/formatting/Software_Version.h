#ifndef SOFTWARE_VERSION_H
#define SOFTWARE_VERSION_H

// DSDL Constants
#include "dsdl.h"

// UAVCAN Formatting
#include <canard.h>

#define SOFTWARE_VERSION_MESSAGE_SIZE             (120 / 8)
#define SOFTWARE_VERSION_DATA_TYPE_SIGNATURE      0xdd46fd376527fea1
#define SOFTWARE_VERSION_DATA_TYPE_ID             DATA_TYPE_NA

#ifdef __cplusplus
extern "C" {
#endif

int8_t software_version(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t major,
  uint8_t minor,
  uint8_t optional_field_flags,
  uint32_t vcs_commit,
  uint64_t image_crc
);

#ifdef __cplusplus
}
#endif

/* SoftwareVersion DSDL

Full name: uavcan.protocol.SoftwareVersion

#
# Nested type.
# Generic software version information.
#

#
# Primary version numbers.
# If both fields are set to zero, the version is considered unknown.
#
uint8 major
uint8 minor

#
# This mask indicates which optional fields (see below) are set.
#
uint8 OPTIONAL_FIELD_FLAG_VCS_COMMIT = 1
uint8 OPTIONAL_FIELD_FLAG_IMAGE_CRC  = 2
uint8 optional_field_flags

#
# VCS commit hash or revision number, e.g. git short commit hash. Optional.
#
uint32 vcs_commit

#
# The value of an arbitrary hash function applied to the firmware image.
# This field is used to detect whether the firmware running on the node is EXACTLY THE SAME
# as a certain specific revision. This field provides the absolute identity guarantee, unlike
# the version fields above, which can be the same for different builds of the firmware.
#
# The exact hash function and the methods of its application are implementation defined.
# However, implementations are recommended to adhere to the following guidelines,
# fully or partially:
#
#   - The hash function should be CRC-64-WE, the same that is used for computing DSDL signatures.
#
#   - The hash function should be applied to the entire application image padded to 8 bytes.
#
#   - If the computed image CRC is stored within the firmware image itself, the value of
#     the hash function becomes ill-defined, because it becomes recursively dependent on itself.
#     In order to circumvent this issue, while computing or checking the CRC, its value stored
#     within the image should be zeroed out.
#
uint64 image_crc

*/

#endif // SOFTWARE_VERSION_H
