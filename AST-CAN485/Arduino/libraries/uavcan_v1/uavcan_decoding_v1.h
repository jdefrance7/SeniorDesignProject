#ifndef UAVCAN_DECODING_H
#define UAVCAN_DECODING_H

//------------------------------------------------------------------------------

/*
  Support functions to handle decoding serialized bytes to UAVCAN data types.
*/

//------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <libcanard_v1.h>

#include "uavcan_dsdl_sizes_v1.h"
#include "uavcan_data_types_v1.h"

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------

// Functions return new bit_offset

// --- OR ---

#define DECODING_BUFFER_OVERFLOW -1

//------------------------------------------------------------------------------

// Heartbeat

// Full name: uavcan.node.Heartbeat

int32_t decode_heartbeat(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  Heartbeat* heartbeat
);

//------------------------------------------------------------------------------

// SynchronizedTimestamp

// Full name: uavcan.time.SynchronizedTimestamp

int32_t decode_synchronized_timestamp(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  SynchronizedTimestamp* timestamp
);

//------------------------------------------------------------------------------

// Quaternion

// Full name: uavcan.si.unit.angle.Quaternion

int32_t decode_quaternion(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  Quaternion* quaternion
);

//------------------------------------------------------------------------------

// ID

// Full name: uavcan.node.ID

int32_t decode_id(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  ID* id
);

//------------------------------------------------------------------------------

// Version

// Full name: uavcan.node.Version

int32_t decode_version(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  Version* version
);

//------------------------------------------------------------------------------

// GetInfo

// Full name: uavcan.node.GetInfo

int32_t decode_get_info(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  GetInfo* get_info
);

//------------------------------------------------------------------------------

// Severity

// Full name: : uavcan.diagnostic.Severity

int32_t decode_severity(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  Severity* severity
);

//------------------------------------------------------------------------------

// Record

// Full name: uavcan.diagnostic.Record

int32_t decode_record(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  Record* record
);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------------

#endif // UAVCAN_DECODING_H
