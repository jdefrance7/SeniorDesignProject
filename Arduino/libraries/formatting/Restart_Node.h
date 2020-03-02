#ifndef RESTART_NODE_H
#define RESTART_NODE_H

// DSDL Constants
#include "dsdl.h"

// UAVCAN Formatting
#include <canard.h>

#define RESTART_NODE_REQUEST_SIZE           (40 / 8)
#define RESTART_NODE_MESSAGE_SIZE           1
#define RESTART_NODE_DATA_TYPE_SIGNATURE    0x569e05394a3017f0
#define RESTART_NODE_DATA_TYPE_ID           5

#define MAGIC_NUMBER          0xACCE551B1E

#ifdef __cplusplus
extern "C" {
#endif

int8_t restart_node(
  uint8_t buffer[],
  uint16_t offset,
  bool ok
);

#ifdef __cplusplus
}
#endif

/* RestartNode DSDL

Full name: uavcan.protocol.RestartNode
Default data type ID: 5

#
# Restart the node.
#
# Some nodes may require restart before the new configuration will be applied.
#
# The request should be rejected if magic_number does not equal MAGIC_NUMBER.
#

uint40 MAGIC_NUMBER = 0xACCE551B1E
uint40 magic_number

---

bool ok

*/

#endif // RESTART_NODE_H
