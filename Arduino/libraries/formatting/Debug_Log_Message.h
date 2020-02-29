#ifndef DEBUG_LOG_MESSAGE_H
#define DEBUG_LOG_MESSAGE_H

// DSDL Constants
#include "dsdl.h"

// UAVCAN Formatting
#include <canard.h>

#define DEBUG_LOG_MESSAGE_MESSAGE_SIZE          (983 / 8)
#define DEBUG_LOG_MESSAGE_DATA_TYPE_SIGNATURE   0xd654a48e0c049d75
#define DEBUG_LOG_MESSAGE_DATA_TYPE_ID          16383

#define LOG_LEVEL_DEBUG     0
#define LOG_LEVEL_INFO      1
#define LOG_LEVEL_WARNING   2
#define LOG_LEVEL_ERROR     3

#define DEBUG_SOURCE_SIZE   31
#define DEBUG_TEXT_SIZE     90

int8_t debug_log_message(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t level,
  uint8_t source[],
  uint8_t text[]
);

/* LogMessage DSDL

Full name: uavcan.protocol.debug.LogMessage
Default data type ID: 16383

#
# Generic log message.
# All items are byte aligned.
#

LogLevel level
uint8[<=31] source
uint8[<=90] text

*/

/* LogLevel DSDL

Full name: uavcan.protocol.debug.LogLevel

#
# Log message severity
#

uint3 DEBUG    = 0
uint3 INFO     = 1
uint3 WARNING  = 2
uint3 ERROR    = 3
uint3 value

*/

#endif // DEBUG_LOG_MESSAGE_H
