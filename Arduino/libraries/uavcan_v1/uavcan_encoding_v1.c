#include "uavcan_encoding_v1.h"

//------------------------------------------------------------------------------

// Heartbeat

// Full name: uavcan.node.Heartbeat

int32_t encode_heartbeat(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  Heartbeat heartbeat
)
{
  if((bit_offset + HEARTBEAT_DATA_TYPE_SIZE) >= (buffer_size * 8))
  {
    return ENCODING_BUFFER_OVERFLOW;
  }

  canardDSDLSetUxx(buffer, bit_offset, (uint64_t)heartbeat.uptime, UINT56);
  bit_offset += UINT56;

  canardDSDLSetUxx(buffer, bit_offset, (uint64_t)heartbeat.health, UINT2);
  bit_offset += UINT2;

  canardDSDLSetUxx(buffer, bit_offset, (uint64_t)heartbeat.mode, UINT3);
  bit_offset += UINT3;

  canardDSDLSetUxx(buffer, bit_offset, (uint64_t)heartbeat.vendor_specific_status_code, UINT19);
  bit_offset += UINT19;

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// SynchronizedTimestamp

// Full name: uavcan.time.SynchronizedTimestamp

int32_t encode_synchronized_timestamp(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  SynchronizedTimestamp timestamp
)
{
  if((bit_offset + SYNCHRONIZED_TIMESTAMP_DATA_TYPE_SIZE) >= (buffer_size * 8))
  {
    return ENCODING_BUFFER_OVERFLOW;
  }

  canardDSDLSetUxx(buffer, bit_offset, (uint64_t)timestamp, UINT56);
  bit_offset += UINT56;

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// Quaternion

// Full name: uavcan.si.unit.angle.Quaternion

int32_t encode_quaternion(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  Quaternion quaternion
)
{
  if((bit_offset + QUATERNION_DATA_TYPE_SIZE) >= (buffer_size * 8))
  {
    return ENCODING_BUFFER_OVERFLOW;
  }

  canardDSDLSetF32(buffer, bit_offset, quaternion.wxyz[QW_INDEX]);
  bit_offset += FLOAT32;

  canardDSDLSetF32(buffer, bit_offset, quaternion.wxyz[QX_INDEX]);
  bit_offset += FLOAT32;

  canardDSDLSetF32(buffer, bit_offset, quaternion.wxyz[QY_INDEX]);
  bit_offset += FLOAT32;

  canardDSDLSetF32(buffer, bit_offset, quaternion.wxyz[QZ_INDEX]);
  bit_offset += FLOAT32;

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// ID

// Full name: uavcan.node.ID

int32_t encode_id(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  ID id
)
{
  if((bit_offset + ID_DATA_TYPE_SIZE) >= (buffer_size * 8))
  {
    return ENCODING_BUFFER_OVERFLOW;
  }

  canardDSDLSetUxx(buffer, bit_offset, (uint64_t)id, UINT16);
  bit_offset += UINT16;

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// Version

// Full name: uavcan.node.Version

int32_t encode_version(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  Version version
)
{
  if((bit_offset + VERSION_DATA_TYPE_SIZE) >= (buffer_size * 8))
  {
    return ENCODING_BUFFER_OVERFLOW;
  }

  canardDSDLSetUxx(buffer, bit_offset, (uint64_t)version.major, UINT8);
  bit_offset += UINT8;

  canardDSDLSetUxx(buffer, bit_offset, (uint64_t)version.minor, UINT8);
  bit_offset += UINT8;

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// GetInfo

// Full name: uavcan.node.GetInfo

int32_t encode_get_info(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  GetInfo get_info
)
{
  if((bit_offset + GET_INFO_DATA_TYPE_SIZE) >= (buffer_size * 8))
  {
    return ENCODING_BUFFER_OVERFLOW;
  }

  encode_version(buffer, buffer_size, bit_offset, get_info.protocol_version);
  bit_offset += VERSION_DATA_TYPE_SIZE;

  encode_version(buffer, buffer_size, bit_offset, get_info.hardware_version);
  bit_offset += VERSION_DATA_TYPE_SIZE;

  encode_version(buffer, buffer_size, bit_offset, get_info.software_version);
  bit_offset += VERSION_DATA_TYPE_SIZE;

  canardDSDLSetUxx(buffer, bit_offset, (uint64_t)get_info.software_vcs_revision_id, UINT64);
  bit_offset += UINT64;

  for(int n = 0; n < GET_INFO_UNIQUE_ID_SIZE; n++)
  {
    canardDSDLSetUxx(buffer, bit_offset, (uint64_t)get_info.unique_id[n], UINT8);
    bit_offset += UINT8;

    if((char)get_info.unique_id[n] == '\0')
    {
      break;
    }
  }

  for(int n = 0; n < GET_INFO_NAME_SIZE; n++)
  {
    canardDSDLSetUxx(buffer, bit_offset, (uint64_t)get_info.name[n], UINT8);
    bit_offset += UINT8;

    if((char)get_info.name[n] == '\0')
    {
      break;
    }
  }

  canardDSDLSetUxx(buffer, bit_offset, (uint64_t)get_info.software_image_crc, UINT64);
  bit_offset += UINT64;

  for(int n = 0; n < GET_INFO_CERTIFICATE_SIZE; n++)
  {
    canardDSDLSetUxx(buffer, bit_offset, (uint64_t)get_info.certificate[n], UINT8);
    bit_offset += UINT8;

    if((char)get_info.certificate[n] == '\0')
    {
      break;
    }
  }

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// Severity

// Full name: : uavcan.diagnostic.Severity

int32_t encode_severity(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  Severity severity
)
{
  if((bit_offset + SEVERITY_DATA_TYPE_SIZE) >= (buffer_size * 8))
  {
    return ENCODING_BUFFER_OVERFLOW;
  }

  canardDSDLSetUxx(buffer, bit_offset, (uint64_t)severity, UINT3);
  bit_offset += UINT3;

  canardDSDLSetUxx(buffer, bit_offset, (uint64_t)0, VOID5);
  bit_offset += VOID5;

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// Record

// Full name: uavcan.diagnostic.Record

int32_t encode_record(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  Record record
)
{
  if((bit_offset + RECORD_DATA_TYPE_SIZE) >= (buffer_size * 8))
  {
    return ENCODING_BUFFER_OVERFLOW;
  }

  encode_synchronized_timestamp(buffer, buffer_size, bit_offset, record.timestamp);
  bit_offset += SYNCHRONIZED_TIMESTAMP_DATA_TYPE_SIZE;

  encode_severity(buffer, buffer_size, bit_offset, record.severity);
  bit_offset += SEVERITY_DATA_TYPE_SIZE;

  for(int n = 0; n < RECORD_TEXT_SIZE; n++)
  {
    canardDSDLSetUxx(buffer, bit_offset, (uint64_t)record.text[n], UINT8);
    bit_offset += UINT8;

    if((char)record.text[n] == '\0')
    {
      break;
    }
  }

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------
