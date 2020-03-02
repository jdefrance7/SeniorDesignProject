#ifndef NODE_TIMESTAMP_H
#define NODE_TIMESTAMP_H

// DSDL Constants
#include "dsdl.h"

// UAVCAN Formatting
#include <canard.h>

#define NODE_TIMESTAMP_MESSAGE_SIZE           (56 / 8)
#define NODE_TIMESTAMP_DATA_TYPE_SIGNATURE    0x05bd0b5c81087e0d
#define NODE_TIMESTAMP_DATA_TYPE_ID           DATA_TYPE_NA

#define TIMESTAMP_UNKNOWN    0

#ifdef __cplusplus
extern "C" {
#endif


int8_t node_timestamp(
  uint8_t buffer[],
  uint8_t offset,
  uint64_t usec
);

#ifdef __cplusplus
}
#endif

/* Timestamp DSDL

Full name: uavcan.Timestamp

#
# Global timestamp in microseconds, 7 bytes.
#
# Use this data type for timestamp fields in messages, like follows:
#   uavcan.Timestamp timestamp
#

uint56 UNKNOWN = 0
truncated uint56 usec     # Microseconds

*/

#endif // NODE_TIMESTAMP_H
