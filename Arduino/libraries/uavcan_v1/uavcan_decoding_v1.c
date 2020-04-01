#include "uavcan_decoding_v1.h"

//------------------------------------------------------------------------------

// Heartbeat

// Full name: uavcan.node.Heartbeat

int32_t decode_heartbeat(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  Heartbeat* heartbeat
)
{
  if((bit_offset + HEARTBEAT_DATA_TYPE_SIZE) >= (buffer_size * 8))
  {
    return DECODING_BUFFER_OVERFLOW;
  }

  // Serial debugging
  #if defined(SERIAL_DEBUG)

  #endif // SERIAL_DEBUG

  heartbeat->uptime = canardDSDLGetU64(buffer, buffer_size, bit_offset, UINT56);
  bit_offset += UINT56;

  heartbeat->health = canardDSDLGetU8(buffer, buffer_size, bit_offset, UINT2);
  bit_offset += UINT2;

  heartbeat->mode = canardDSDLGetU8(buffer, buffer_size, bit_offset, UINT3);
  bit_offset += UINT3;

  heartbeat->vendor_specific_status_code = canardDSDLGetU32(buffer, buffer_size, bit_offset, UINT19);
  bit_offset += UINT19;

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// SynchronizedTimestamp

// Full name: uavcan.time.SynchronizedTimestamp

int32_t decode_synchronized_timestamp(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  SynchronizedTimestamp* timestamp
)
{
  if((bit_offset + SYNCHRONIZED_TIMESTAMP_DATA_TYPE_SIZE) >= (buffer_size * 8))
  {
    return DECODING_BUFFER_OVERFLOW;
  }

  *timestamp = canardDSDLGetU64(buffer, buffer_size, bit_offset, UINT56);
  bit_offset += UINT56;

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// Quaternion

// Full name: uavcan.si.unit.angle.Quaternion

int32_t decode_quaternion(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  Quaternion* quaternion
)
{
  if((bit_offset + QUATERNION_DATA_TYPE_SIZE) >= (buffer_size * 8))
  {
    return DECODING_BUFFER_OVERFLOW;
  }

  quaternion->wxyz[QW_INDEX] = canardDSDLGetF32(buffer, buffer_size, bit_offset);
  bit_offset += FLOAT32;

  quaternion->wxyz[QX_INDEX] = canardDSDLGetF32(buffer, buffer_size, bit_offset);
  bit_offset += FLOAT32;

  quaternion->wxyz[QY_INDEX] = canardDSDLGetF32(buffer, buffer_size, bit_offset);
  bit_offset += FLOAT32;

  quaternion->wxyz[QZ_INDEX] = canardDSDLGetF32(buffer, buffer_size, bit_offset);
  bit_offset += FLOAT32;

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// ID

// Full name: uavcan.node.ID

int32_t decode_id(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  ID* id
)
{
  if((bit_offset + ID_DATA_TYPE_SIZE) >= (buffer_size * 8))
  {
    return DECODING_BUFFER_OVERFLOW;
  }

  *id = canardDSDLGetU16(buffer, buffer_size, bit_offset, UINT16);
  bit_offset += UINT16;

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// Version

// Full name: uavcan.node.Version

int32_t decode_version(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  Version* version
)
{
  if((bit_offset + VERSION_DATA_TYPE_SIZE) >= (buffer_size * 8))
  {
    return DECODING_BUFFER_OVERFLOW;
  }

  version->major = canardDSDLGetU8(buffer, buffer_size, bit_offset, UINT8);
  bit_offset += UINT8;

  version->minor = canardDSDLGetU8(buffer, buffer_size, bit_offset, UINT8);
  bit_offset += UINT8;

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// GetInfo

// Full name: uavcan.node.GetInfo

int32_t decode_get_info(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  GetInfo* get_info
)
{
  if((bit_offset + GET_INFO_DATA_TYPE_SIZE) >= (buffer_size * 8))
  {
    return DECODING_BUFFER_OVERFLOW;
  }

  decode_version(buffer, buffer_size, bit_offset, &(get_info->protocol_version));
  bit_offset += VERSION_DATA_TYPE_SIZE;

  decode_version(buffer, buffer_size, bit_offset, &(get_info->hardware_version));
  bit_offset += VERSION_DATA_TYPE_SIZE;

  decode_version(buffer, buffer_size, bit_offset, &(get_info->software_version));
  bit_offset += VERSION_DATA_TYPE_SIZE;

  get_info->software_vcs_revision_id = canardDSDLGetU64(buffer, buffer_size, bit_offset, UINT64);
  bit_offset += UINT64;

  for(int n = 0; n < GET_INFO_UNIQUE_ID_SIZE; n++)
  {
    get_info->unique_id[n] = canardDSDLGetU8(buffer, buffer_size, bit_offset, UINT8);
    bit_offset += UINT8;
  }

  for(int n = 0; n < GET_INFO_NAME_SIZE; n++)
  {
    get_info->name[n] = canardDSDLGetU8(buffer, buffer_size, bit_offset, UINT8);
    bit_offset += UINT8;

    if((char)get_info->name[n] == '\0')
    {
      break;
    }
  }

  get_info->software_image_crc = canardDSDLGetU64(buffer, buffer_size, bit_offset, UINT64);
  bit_offset += UINT64;

  for(int n = 0; n < GET_INFO_CERTIFICATE_SIZE; n++)
  {
    get_info->certificate[n] = canardDSDLGetU8(buffer, buffer_size, bit_offset, UINT8);
    bit_offset += UINT8;

    if((char)get_info->certificate[n] == '\0')
    {
      break;
    }
  }

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// Severity

// Full name: : uavcan.diagnostic.Severity

int32_t decode_severity(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  Severity* severity
)
{
  if((bit_offset + SEVERITY_DATA_TYPE_SIZE) >= (buffer_size * 8))
  {
    return DECODING_BUFFER_OVERFLOW;
  }

  *severity = canardDSDLGetU8(buffer, buffer_size, bit_offset, UINT3);
  bit_offset += UINT3;

  // void5 padding
  bit_offset += UINT5;

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// Record

// Full name: uavcan.diagnostic.Record

int32_t decode_record(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  Record* record
)
{
  if((bit_offset + RECORD_DATA_TYPE_SIZE) >= (buffer_size * 8))
  {
    return DECODING_BUFFER_OVERFLOW;
  }

  decode_synchronized_timestamp(buffer, buffer_size, bit_offset, &(record->timestamp));
  bit_offset += SYNCHRONIZED_TIMESTAMP_DATA_TYPE_SIZE;

  decode_severity(buffer, buffer_size, bit_offset, &(record->severity));
  bit_offset += SEVERITY_DATA_TYPE_SIZE;

  for(int n = 0; n < RECORD_TEXT_SIZE; n++)
  {
    record->text[n] = canardDSDLGetU8(buffer, buffer_size, bit_offset, UINT8);
    bit_offset += UINT8;

    if((char)record->text[n] == '\0')
    {
      break;
    }
  }

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------
