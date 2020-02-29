#ifndef DEBUG_KEY_VALUE_H
#define DEBUG_KEY_VALUE_H

// DSDL Constants
#include "dsdl.h"

// UAVCAN Formatting
#include <canard.h>

#define DEBUG_KEY_VALUE_MESSAGE_SIZE            (502 / 8)
#define DEBUG_KEY_VALUE_DATA_TYPE_SIGNATURE     0xe02f25d6e0c98ae0
#define DEBUG_KEY_VALUE_DATA_TYPE_ID            16370

#define DEBUG_KEY_MAX_SIZE  58

int8_t debug_key_value(
  uint8_t buffer[],
  uint16_t offset,
  float32_t value,
  uint8_t key[]
);

/* KeyValue DSDL

Full name: uavcan.protocol.debug.KeyValue
Default data type ID: 16370

#
# Generic named parameter (key/value pair).
#

#
# Integers are exactly representable in the range (-2^24, 2^24) which is (-16'777'216, 16'777'216).
#
float32 value

#
# Tail array optimization is enabled, so if key length does not exceed 3 characters, the whole
# message can fit into one CAN frame. The message always fits into one CAN FD frame.
#
uint8[<=58] key

*/

#endif // DEBUG_KEY_VALUE_H
