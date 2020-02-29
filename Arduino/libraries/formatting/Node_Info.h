#ifndef NODE_INFO_H
#define NODE_INFO_H

// DSDL Constants
#include "dsdl.h"

// UAVCAN Formatting
#include <canard.h>

// Other Standard Data Types
#include "Node_Status.h"
#include "Software_Version.h"
#include "Hardware_Version.h"

#define NODE_INFO_REQUEST_SIZE            0
#define NODE_INFO_MESSAGE_SIZE            (3015 / 8)
#define NODE_INFO_DATA_TYPE_SIGNATURE     0xee468a8121c46a9e
#define NODE_INFO_DATA_TYPE_ID            1

int8_t node_info(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t status[NODE_STATUS_MESSAGE_SIZE],
  uint8_t software_version[SOFTWARE_VERSION_MESSAGE_SIZE],
  uint8_t hardware_version[HARDWARE_VERSION_MESSAGE_SIZE],
  uint8_t name[]
);

/* GetNodeInfo DSDL

Full name: uavcan.protocol.GetNodeInfo
Default data type ID: 1

#
# Full node info request.
# Note that all fields of the response section are byte-aligned.
#

---

#
# Current node status
#
NodeStatus status

#
# Version information shall not be changed while the node is running.
#
SoftwareVersion software_version
HardwareVersion hardware_version

#
# Human readable non-empty ASCII node name.
# Node name shall not be changed while the node is running.
# Empty string is not a valid node name.
# Allowed characters are: a-z (lowercase ASCII letters) 0-9 (decimal digits) . (dot) - (dash) _ (underscore).
# Node name is a reversed internet domain name (like Java packages), e.g. "com.manufacturer.project.product".
#
uint8[<=80] name

*/

#endif // NODE_INFO_H
