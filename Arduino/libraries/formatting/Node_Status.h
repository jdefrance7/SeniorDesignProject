#ifndef NODE_STATUS_H
#define NODE_STATUS_H

// DSDL Constants
#include "dsdl.h"

// UAVCAN Formatting
#include <canard.h>

#define NODE_STATUS_MESSAGE_SIZE          (56 / 8)
#define NODE_STATUS_DATA_TYPE_SIGNATURE   0x0f0868d0c1a7c6f1
#define NODE_STATUS_DATA_TYPE_ID          341

#define NODE_HEALTH_OK                    0
#define NODE_HEALTH_WARNING               1
#define NODE_HEALTH_ERROR                 2
#define NODE_HEALTH_CRITICAL              3

#define NODE_MODE_OPERATIONAL             0
#define NODE_MODE_INITIALIZATION          1
#define NODE_MODE_MAINTENANCE             2
#define NODE_MODE_SOFTWARE_UPDATE         3
#define NODE_MODE_OFFLINE                 7

#define MAX_BROADCASTING_PERIOD_MS        1000
#define MIN_BROADCASTING_PERIOD_MS        2

#define OFFLINE_TIMEOUT_MS                3000

#ifdef __cplusplus
extern "C" {
#endif

int8_t node_status(
  uint8_t buffer[],
  uint16_t offset,
  uint32_t uptime_sec,
  uint8_t health,
  uint8_t mode
);

#ifdef __cplusplus
}
#endif

/* NodeStatus DSDL

Full name: uavcan.protocol.NodeStatus
Default data type ID: 341

#
# Abstract node status information.
#
# Any UAVCAN node is required to publish this message periodically.
#

#
# Publication period may vary within these limits.
# It is NOT recommended to change it at run time.
#
uint16 MAX_BROADCASTING_PERIOD_MS = 1000
uint16 MIN_BROADCASTING_PERIOD_MS = 2

#
# If a node fails to publish this message in this amount of time, it should be considered offline.
#
uint16 OFFLINE_TIMEOUT_MS = 3000

#
# Uptime counter should never overflow.
# Other nodes may detect that a remote node has restarted when this value goes backwards.
#
uint32 uptime_sec

#
# Abstract node health.
#
uint2 HEALTH_OK         = 0     # The node is functioning properly.
uint2 HEALTH_WARNING    = 1     # A critical parameter went out of range or the node encountered a minor failure.
uint2 HEALTH_ERROR      = 2     # The node encountered a major failure.
uint2 HEALTH_CRITICAL   = 3     # The node suffered a fatal malfunction.
uint2 health

#
# Current mode.
#
# Mode OFFLINE can be actually reported by the node to explicitly inform other network
# participants that the sending node is about to shutdown. In this case other nodes will not
# have to wait OFFLINE_TIMEOUT_MS before they detect that the node is no longer available.
#
# Reserved values can be used in future revisions of the specification.
#
uint3 MODE_OPERATIONAL      = 0         # Node is performing its main functions.
uint3 MODE_INITIALIZATION   = 1         # Node is initializing; this mode is entered immediately after startup.
uint3 MODE_MAINTENANCE      = 2         # Node is under maintenance.
uint3 MODE_SOFTWARE_UPDATE  = 3         # Node is in the process of updating its software.
uint3 MODE_OFFLINE          = 7         # Node is no longer available.
uint3 mode

#
# Not used currently, keep zero when publishing, ignore when receiving.
#
uint3 sub_mode

#
# Optional, vendor-specific node status code, e.g. a fault code or a status bitmask.
#
uint16 vendor_specific_status_code

*/

#endif // NODE_STATUS_H
