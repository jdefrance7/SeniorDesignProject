#include "uavcan_formatting.h"

int8_t ahrs_solution(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t timestamp[],
  float16_t orientation_x,
  float16_t orientation_y,
  float16_t orientation_z,
  float16_t orientation_w,
  float16_t angular_velocity_x,
  float16_t angular_velocity_y,
  float16_t angular_velocity_z,
  float16_t linear_acceleration_x,
  float16_t linear_acceleration_y,
  float16_t linear_acceleration_z
)
{
  if((offset+AHRS_SOLUTION_MESSAGE_SIZE) > sizeof(buffer))
  {
    return -1;
  }

  if(sizeof(timestamp) > NODE_TIMESTAMP_MESSAGE_SIZE)
  {
    return -1;
  }

  for(int n = 0; n < sizeof(timestamp); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &timestamp[n]);
    offset += 8;
  }
  for(int n = 0; n < NODE_TIMESTAMP_MESSAGE_SIZE-sizeof(timestamp); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, NULL);
    offset += 8;
  }

  canardEncodeScalar(buffer, offset, FLOAT16, &orientation_x);
  offset += FLOAT16;

  canardEncodeScalar(buffer, offset, FLOAT16, &orientation_y);
  offset += FLOAT16;

  canardEncodeScalar(buffer, offset, FLOAT16, &orientation_z);
  offset += FLOAT16;

  canardEncodeScalar(buffer, offset, FLOAT16, &orientation_w);
  offset += FLOAT16;

  canardEncodeScalar(buffer, offset, VOID4, NULL);
  offset += VOID4;

  for(int n = 0; n < 9; n++)
  {
    canardEncodeScalar(buffer, offset, FLOAT16, NULL);
    offset += FLOAT16;
  }

  canardEncodeScalar(buffer, offset, FLOAT16, &angular_velocity_x);
  offset += FLOAT16;

  canardEncodeScalar(buffer, offset, FLOAT16, &angular_velocity_y);
  offset += FLOAT16;

  canardEncodeScalar(buffer, offset, FLOAT16, &angular_velocity_z);
  offset += FLOAT16;

  canardEncodeScalar(buffer, offset, VOID4, NULL);
  offset += 4;

  for(int n = 0; n < 9; n++)
  {
    canardEncodeScalar(buffer, offset, FLOAT16, NULL);
    offset += FLOAT16;
  }

  canardEncodeScalar(buffer, offset, FLOAT16, &linear_acceleration_x);
  offset += FLOAT16;

  canardEncodeScalar(buffer, offset, FLOAT16, &linear_acceleration_y);
  offset += FLOAT16;

  canardEncodeScalar(buffer, offset, FLOAT16, &linear_acceleration_z);
  offset += FLOAT16;

  for(int n = 0; n < 9; n++)
  {
    canardEncodeScalar(buffer, offset, FLOAT16, NULL);
    offset += FLOAT16;
  }

  return 0;
}

int8_t camera_gimbal_status(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t gimbal_id,
  uint8_t camera_mode,
  float16_t camera_orientation_x,
  float16_t camera_orientation_y,
  float16_t camera_orientation_z,
  float16_t camera_orientation_w
)
{
  if((offset + CAMERA_GIMBAL_STATUS_MESSAGE_SIZE) > sizeof(buffer))
  {
    return -1;
  }

  canardEncodeScalar(buffer, offset, UINT8, &gimbal_id);
  offset += UINT8;

  canardEncodeScalar(buffer, offset, UINT8, &camera_mode);
  offset += UINT8;

  canardEncodeScalar(buffer, offset, FLOAT16, &camera_orientation_x);
  offset += UINT16;

  canardEncodeScalar(buffer, offset, FLOAT16, &camera_orientation_y);
  offset += UINT16;

  canardEncodeScalar(buffer, offset, FLOAT16, &camera_orientation_z);
  offset += UINT16;

  canardEncodeScalar(buffer, offset, FLOAT16, &camera_orientation_w);
  offset += UINT16;

  for(int n = 0; n < 9; n++)
  {
    canardEncodeScalar(buffer, offset, FLOAT16, NULL);
    offset += FLOAT16;
  }

  return 0;
}

int8_t coarse_orientation(
  uint8_t buffer[],
  uint8_t offset,
  int8_t roll,
  int8_t pitch,
  int8_t yaw,
  bool orientation_defined
)
{
  if((offset + COARSE_ORIENTATION_MESSAGE_SIZE) > sizeof(buffer))
  {
    return -1;
  }

  canardEncodeScalar(buffer, offset, INT5, &roll);
  offset += INT5;

  canardEncodeScalar(buffer, offset, INT5, &pitch);
  offset += INT5;

  canardEncodeScalar(buffer, offset, INT5, &yaw);
  offset += INT5;

  canardEncodeScalar(buffer, offset, BOOL, &orientation_defined);
  offset += BOOL;

  return 0;
}

int8_t data_type_info(
  uint8_t buffer[],
  uint16_t offset,
  uint64_t signature,
  uint16_t id,
  uint8_t kind,
  uint8_t flags,
  uint8_t name[]
)
{
  if((offset + DATA_TYPE_INFO_MESSAGE_SIZE) > sizeof(buffer))
  {
    return -1;
  }

  canardEncodeScalar(buffer, offset, UINT16, &id);
  offset += UINT16;

  canardEncodeScalar(buffer, offset, UINT8, &kind);
  offset += UINT8;

  canardEncodeScalar(buffer, offset, UINT8, &flags);
  offset += UINT8;

  for(int n = 0; n < sizeof(name); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &name[n]);
    offset += UINT8;
  }

  for(int n = 0; n < DATA_TYPE_INFO_NAME_SIZE-sizeof(name); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, NULL);
    offset += UINT8;
  }

  return 0;
}

int8_t debug_key_value(
  uint8_t buffer[],
  uint16_t offset,
  float32_t value,
  uint8_t key[]
)
{
  if((offset + sizeof(buffer)) > DEBUG_KEY_VALUE_MESSAGE_SIZE)
  {
    return -1;
  }

  if(sizeof(key) > DEBUG_KEY_MAX_SIZE)
  {
    return -1;
  }

  canardEncodeScalar(buffer, offset, FLOAT32, &value);
  offset += FLOAT32;

  for(int n = 0; n < sizeof(key); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &key[n]);
    offset += UINT8;
  }
  for(int n = 0; DEBUG_KEY_VALUE_MESSAGE_SIZE-sizeof(buffer); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, NULL);
    offset += UINT8;
  }

  return 0;
}

int8_t debug_log_message(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t level,
  uint8_t source[],
  uint8_t text[]
)
{
  if((offset + DEBUG_LOG_MESSAGE_MESSAGE_SIZE) > sizeof(buffer))
  {
    return -1;
  }

  canardEncodeScalar(buffer, offset, UINT8, &level);
  offset += UINT8;

  for(int n = 0; n < sizeof(source); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &source[n]);
    offset += UINT8;
  }
  for(int n = 0; n < DEBUG_SOURCE_SIZE-sizeof(source); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, NULL);
    offset += UINT8;
  }

  for(int n = 0; n < sizeof(text); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &text[n]);
    offset += UINT8;
  }
  for(int n = 0; n < DEBUG_TEXT_SIZE-sizeof(text); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, NULL);
    offset += UINT8;
  }

  return 0;
}

int8_t device_temperature(
  uint8_t buffer[],
  uint16_t offset,
  uint16_t device_id,
  float16_t temperature,
  uint8_t error_flags
)
{
  if((offset + DEVICE_TEMPERATURE_MESSAGE_SIZE) > sizeof(buffer))
  {
    return -1;
  }

  canardEncodeScalar(buffer, offset,  UINT16, &device_id);
  offset += UINT16;

  canardEncodeScalar(buffer, offset, FLOAT16, &temperature);
  offset += FLOAT16;

  canardEncodeScalar(buffer, offset,   UINT8, &error_flags);
  offset += UINT8;

  return 0;
}

int8_t hardware_version(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t major,
  uint8_t minor,
  uint8_t unique_id[],
  uint8_t certificate_of_authenticity[]
)
{
  if((offset + HARDWARE_VERSION_MESSAGE_SIZE) > sizeof(buffer))
  {
    return -1;
  }

  canardEncodeScalar(buffer, offset, UINT8, &major);
  offset += UINT8;

  canardEncodeScalar(buffer, offset, UINT8, &minor);
  offset += UINT8;

  for(int n = 0; n < sizeof(unique_id); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &unique_id[n]);
    offset += UINT8;
  }
  for(int n = 0; n < HARDWARE_VERSION_UNIQUE_ID_SIZE-sizeof(unique_id); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, NULL);
    offset += UINT8;
  }

  for(int n = 0; n < sizeof(certificate_of_authenticity); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &certificate_of_authenticity[n]);
    offset += UINT8;
  }
  for(int n = 0; n < HARDWARE_VERSION_CERTIFICATE_OF_AUTHENTICITY_SIZE-sizeof(certificate_of_authenticity); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, NULL);
    offset += UINT8;
  }

  return 0;
}

int8_t magnetic_field_strength(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t sensor_id,
  float16_t magnetic_field_x,
  float16_t magnetic_field_y,
  float16_t magnetic_field_z
)
{
  if((offset + MAGNETIC_FIELD_STRENGTH_MESSAGE_SIZE) > sizeof(buffer))
  {
    return -1;
  }

  canardEncodeScalar(buffer, offset, UINT8,   &sensor_id);
  offset += UINT8;

  canardEncodeScalar(buffer, offset, FLOAT16, &magnetic_field_x);
  offset += FLOAT16;

  canardEncodeScalar(buffer, offset, FLOAT16, &magnetic_field_y);
  offset += FLOAT16;

  canardEncodeScalar(buffer, offset, FLOAT16, &magnetic_field_z);
  offset += FLOAT16;

  for(int n = 0; n < 9; n++)
  {
    canardEncodeScalar(buffer, offset, FLOAT16, NULL);
    offset += FLOAT16;
  }

  return 0;
}

int8_t node_info(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t status[],
  uint8_t software_version[],
  uint8_t hardware_version[],
  uint8_t name[]
)
{
  if((offset + NODE_INFO_MESSAGE_SIZE) > sizeof(buffer))
  {
    return -1;
  }

  for(int n = 0; n < sizeof(status); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &status[n]);
    offset += UINT8;
  }
  for(int n = 0; n < NODE_STATUS_MESSAGE_SIZE - sizeof(status); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, NULL);
    offset += UINT8;
  }

  for(int n = 0; n < sizeof(software_version); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &software_version[n]);
    offset += UINT8;
  }
  for(int n = 0; n < SOFTWARE_VERSION_MESSAGE_SIZE-sizeof(software_version); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, NULL);
    offset += UINT8;
  }

  for(int n = 0; n < sizeof(hardware_version); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &hardware_version[n]);
    offset += UINT8;
  }
  for(int n = 0; n < HARDWARE_VERSION_MESSAGE_SIZE-sizeof(hardware_version); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, NULL);
    offset += UINT8;
  }

  for(int n = 0; n < sizeof(name); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &name[n]);
    offset += UINT8;
  }

  return 0;
}

int8_t node_status(
  uint8_t buffer[],
  uint16_t offset,
  uint32_t uptime_sec,
  uint8_t health,
  uint8_t mode
)
{
  if((offset + NODE_STATUS_MESSAGE_SIZE) > sizeof(buffer))
  {
    return -1;
  }

  canardEncodeScalar(buffer, offset, UINT32, &uptime_sec);
  offset += UINT32;

  canardEncodeScalar(buffer, offset, UINT2, &health);
  offset += UINT2;

  canardEncodeScalar(buffer, offset, UINT3, &mode);
  offset += UINT3;

  canardEncodeScalar(buffer, offset, UINT3, NULL);
  offset += UINT3;

  canardEncodeScalar(buffer, offset, UINT16, NULL);
  offset += UINT16;

  return 0;
}

int8_t node_timestamp(
  uint8_t buffer[],
  uint8_t offset,
  uint64_t usec
)
{
  if((offset + NODE_TIMESTAMP_MESSAGE_SIZE) > sizeof(buffer))
  {
    return -1;
  }

  canardEncodeScalar(buffer, offset, UINT56, &usec);
  offset += UINT56;

  return 0;
}

int8_t range_sensor_measurement(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t usec[],
  uint8_t sensor_id,
  uint8_t coarse_orientation[],
  float16_t field_of_view,
  uint8_t sensor_type,
  float16_t range
)
{
  if((offset + RANGE_SENSOR_MEASUREMENT_MESSAGE_SIZE) > sizeof(buffer))
  {
    return -1;
  }

  for(int n = 0; n < sizeof(usec); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &usec[n]);
    offset += UINT8;
  }
  for(int n = 0; n < NODE_TIMESTAMP_MESSAGE_SIZE - sizeof(usec); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, NULL);
    offset += UINT8;
  }

  canardEncodeScalar(buffer, offset, UINT8, sensor_id);
  offset += UINT8;

  for(int n = 0; n < sizeof(coarse_orientation); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, &coarse_orientation[n]);
    offset += UINT8;
  }
  for(int n = 0; n < COARSE_ORIENTATION_MESSAGE_SIZE - sizeof(coarse_orientation); n++)
  {
    canardEncodeScalar(buffer, offset, UINT8, NULL);
    offset += UINT8;
  }

  canardEncodeScalar(buffer, offset, FLOAT16, &field_of_view);
  offset += FLOAT16;

  canardEncodeScalar(buffer, offset, UINT5, &sensor_type);
  offset += UINT5;

  canardEncodeScalar(buffer, offset, FLOAT16, &range);
  offset += FLOAT16;

  return 0;
}

int8_t restart_node(
  uint8_t buffer[],
  uint16_t offset,
  bool ok
)
{
  if((offset + RESTART_NODE_MESSAGE_SIZE) > sizeof(buffer))
  {
    return -1;
  }

  canardEncodeScalar(buffer, offset, BOOL, &ok);
  offset += BOOL;

  return 0;
}

int8_t software_version(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t major,
  uint8_t minor,
  uint8_t optional_field_flags,
  uint32_t vcs_commit,
  uint64_t image_crc
)
{
  if((offset + SOFTWARE_VERSION_MESSAGE_SIZE) > sizeof(buffer))
  {
    return -1;
  }

  canardEncodeScalar(buffer, offset, UINT8, &major);
  offset += UINT8;

  canardEncodeScalar(buffer, offset, UINT8, &minor);
  offset += UINT8;

  canardEncodeScalar(buffer, offset, UINT8, &optional_field_flags);
  offset += UINT8;

  canardEncodeScalar(buffer, offset, UINT32, &vcs_commit);
  offset += UINT32;

  canardEncodeScalar(buffer, offset, UINT64, &image_crc);
  offset += UINT64;

  return 0;
}
