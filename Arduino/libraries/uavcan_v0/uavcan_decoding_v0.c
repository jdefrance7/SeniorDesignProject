#include "uavcan_decoding_v0.h"

//------------------------------------------------------------------------------

// Coarse Orientation

// Full Name: uavcan.CoarseOrientation

int32_t decode_coarse_orientation(
  const CanardRxTransfer* transfer,
  uint32_t bit_offset,
  CoarseOrientation* coarse_orientation
)
{
  for(int n = 0; n < 3; n++)
  {
    canardDecodeScalar(transfer, bit_offset, INT5, SIGNED, &coarse_orientation->fixed_axis_roll_pitch_yaw[n]);
    bit_offset += INT5;
  }

  canardDecodeScalar(transfer, bit_offset, BOOL, UNSIGNED, &coarse_orientation->orientation_defined);
  bit_offset += BOOL;

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// Timestamp

// Full Name: uavcan.Timestamp

int32_t decode_timestamp(
  const CanardRxTransfer* transfer,
  uint32_t bit_offset,
  Timestamp* timestamp
)
{
  canardDecodeScalar(transfer, bit_offset, UINT56, UNSIGNED, timestamp);
  bit_offset += UINT56;

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// AHRS Solution

// Full name: uavcan.equipment.ahrs.Solution

int32_t decode_ahrs_solution(
  const CanardRxTransfer* transfer,
  uint32_t bit_offset,
  AhrsSolution* ahrs_solution
)
{
  decode_timestamp(transfer, bit_offset, &ahrs_solution->timestamp);
  bit_offset += TIMESTAMP_DATA_TYPE_SIZE;

  for(int n = 0; n < 4; n++)
  {
    canardDecodeScalar(transfer, bit_offset, FLOAT16, SIGNED, &ahrs_solution->orientation_xyzw[n]);
    bit_offset += FLOAT16;
  }

  bit_offset += VOID4;

  for(int n = 0; n < 9; n++)
  {
    canardDecodeScalar(transfer, bit_offset, FLOAT16, SIGNED, &ahrs_solution->orientation_covariance[n]);
    bit_offset += FLOAT16;
  }

  for(int n = 0; n < 3; n++)
  {
    canardDecodeScalar(transfer, bit_offset, FLOAT16, SIGNED, &ahrs_solution->angular_velocity[n]);
    bit_offset += FLOAT16;
  }

  bit_offset += VOID4;

  for(int n = 0; n < 9; n++)
  {
    canardDecodeScalar(transfer, bit_offset, FLOAT16, SIGNED, &ahrs_solution->angular_velocity_covariance[n]);
    bit_offset += FLOAT16;
  }

  for(int n = 0; n < 3; n++)
  {
    canardDecodeScalar(transfer, bit_offset, FLOAT16, SIGNED, &ahrs_solution->linear_acceleration[n]);
    bit_offset += FLOAT16;
  }

  for(int n = 0; n < 9; n++)
  {
    canardDecodeScalar(transfer, bit_offset, FLOAT16, SIGNED, &ahrs_solution->linear_acceleration_covariance[n]);
    bit_offset += FLOAT16;
  }

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// Camera Gimbal Status

// Full name: uavcan.equipment.camera_gimbal.Status

int32_t decode_camera_gimbal_status(
  const CanardRxTransfer* transfer,
  uint32_t bit_offset,
  CameraGimbalStatus* camera_gimbal_status
)
{
  canardDecodeScalar(transfer, bit_offset, UINT8, UNSIGNED, &camera_gimbal_status->gimbal_id);
  bit_offset += UINT8;

  decode_camera_gimbal_mode(transfer, bit_offset, &camera_gimbal_status->mode);
  bit_offset += CAMERA_GIMBAL_MODE_DATA_TYPE_SIZE;

  for(int n = 0; n < 4; n++)
  {
    canardDecodeScalar(transfer, bit_offset, FLOAT16, SIGNED, &camera_gimbal_status->camera_orientation_in_body_frame_xyzw[n]);
    bit_offset += FLOAT16;
  }

  for(int n = 0; n < 9; n++)
  {
    canardDecodeScalar(transfer, bit_offset, FLOAT16, SIGNED, &camera_gimbal_status->camera_orientation_in_body_frame_covariance[n]);
    bit_offset += FLOAT16;
  }

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// Camera Gimbal Mode

// Full name: uavcan.equipment.camera_gimbal.Mode

int32_t decode_camera_gimbal_mode(
  const CanardRxTransfer* transfer,
  uint32_t bit_offset,
  CameraGimbalMode* camera_gimbal_mode
)
{
  canardDecodeScalar(transfer, bit_offset, UINT8, UNSIGNED, camera_gimbal_mode);
  bit_offset += UINT8;

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// Device Temperature

// Full name: uavcan.equipment.device.Temperature

int32_t decode_device_temperature(
  const CanardRxTransfer* transfer,
  uint32_t bit_offset,
  DeviceTemperature* device_temperature
)
{
  canardDecodeScalar(transfer, bit_offset, UINT16, UNSIGNED, &device_temperature->device_id);
  bit_offset += UINT16;

  canardDecodeScalar(transfer, bit_offset, FLOAT16, SIGNED, &device_temperature->temperature);
  bit_offset += FLOAT16;

  canardDecodeScalar(transfer, bit_offset, UINT8, UNSIGNED, &device_temperature->error_flags);
  bit_offset += UINT8;

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// Range Sensor Measurement

// Full name: uavcan.equipment.range_sensor.Measurement

int32_t decode_range_sensor_measurement(
  const CanardRxTransfer* transfer,
  uint32_t bit_offset,
  RangeSensorMeasurement* range_sensor_measurement
)
{
  decode_timestamp(transfer, bit_offset, &range_sensor_measurement->timestamp);
  bit_offset += TIMESTAMP_DATA_TYPE_SIZE;

  canardDecodeScalar(transfer, bit_offset, UINT8, UNSIGNED, &range_sensor_measurement->sensor_id);
  bit_offset += UINT8;

  decode_coarse_orientation(transfer, bit_offset, &range_sensor_measurement->beam_orientation_in_body_frame);
  bit_offset += COARSE_ORIENTATION_DATA_TYPE_SIZE;

  canardDecodeScalar(transfer, bit_offset, FLOAT16, SIGNED, &range_sensor_measurement->field_of_view);
  bit_offset += FLOAT16;

  canardDecodeScalar(transfer, bit_offset, UINT5, UNSIGNED, &range_sensor_measurement->sensor_type);
  bit_offset += UINT5;

  canardDecodeScalar(transfer, bit_offset, UINT3, UNSIGNED, &range_sensor_measurement->reading_type);
  bit_offset += UINT3;

  canardDecodeScalar(transfer, bit_offset, FLOAT16, SIGNED, &range_sensor_measurement->range);
  bit_offset += FLOAT16;

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// Get Node Info

// Full name: uavcan.protocol.GetNodeInfo

int32_t decode_get_node_info(
  const CanardRxTransfer* transfer,
  uint32_t bit_offset,
  GetNodeInfo* get_node_info
)
{
  decode_node_status(transfer, bit_offset, &get_node_info->status);
  bit_offset += NODE_STATUS_DATA_TYPE_SIZE;

  decode_software_version(transfer, bit_offset, &get_node_info->software_version);
  bit_offset += SOFTWARE_VERSION_DATA_TYPE_SIZE;

  decode_hardware_version(transfer, bit_offset, &get_node_info->hardware_version);
  bit_offset += HARDWARE_VERSION_DATA_TYPE_SIZE;

  for(int n = 0; n < 80; n++)
  {
    canardDecodeScalar(transfer, bit_offset, UINT8, UNSIGNED, &get_node_info->name[n]);
    bit_offset += UINT8;
  }

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// Get Data Type Info

// Full name: uavcan.protocol.GetDataTypeInfo

int32_t decode_get_data_type_info(
  const CanardRxTransfer* transfer,
  uint32_t bit_offset,
  GetDataTypeInfo* get_data_type_info
)
{
  canardDecodeScalar(transfer, bit_offset, UINT64, UNSIGNED, &get_data_type_info->signature);
  bit_offset += UINT64;

  canardDecodeScalar(transfer, bit_offset, UINT16, UNSIGNED, &get_data_type_info->id);
  bit_offset += UINT16;

  decode_data_type_kind(transfer, bit_offset, &get_data_type_info->kind);
  bit_offset += DATA_TYPE_KIND_DATA_TYPE_SIZE;

  canardDecodeScalar(transfer, bit_offset, UINT8, UNSIGNED, &get_data_type_info->flags);
  bit_offset += UINT8;

  for(int n = 0; n < 80; n++)
  {
    canardDecodeScalar(transfer, bit_offset, UINT8, UNSIGNED, &get_data_type_info->name[n]);
    bit_offset += UINT8;
  }

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// Node Status

// Full name: uavcan.protocol.NodeStatus

int32_t decode_node_status(
  const CanardRxTransfer* transfer,
  uint32_t bit_offset,
  NodeStatus* node_status
)
{
  canardDecodeScalar(transfer, bit_offset, UINT32, UNSIGNED, &node_status->uptime_sec);
  bit_offset += UINT32;

  canardDecodeScalar(transfer, bit_offset, UINT2, UNSIGNED, &node_status->health);
  bit_offset += UINT2;

  canardDecodeScalar(transfer, bit_offset, UINT3, UNSIGNED, &node_status->mode);
  bit_offset += UINT3;

  canardDecodeScalar(transfer, bit_offset, UINT3, UNSIGNED, &node_status->sub_mode);
  bit_offset += UINT3;

  canardDecodeScalar(transfer, bit_offset, UINT16, UNSIGNED, &node_status->vendor_specific_status_code);
  bit_offset += UINT16;

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// Restart Node

// Full name: uavcan.protocol.RestartNode

int32_t decode_restart_node(
  const CanardRxTransfer* transfer,
  uint32_t bit_offset,
  RestartNode* restart_node
)
{
  canardDecodeScalar(transfer, bit_offset, BOOL, UNSIGNED, restart_node);
  bit_offset += BOOL;

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// Data Type Kind

// Full name: uavcan.protocol.DataTypeKind

int32_t decode_data_type_kind(
  const CanardRxTransfer* transfer,
  uint32_t bit_offset,
  DataTypeKind* data_type_kind
)
{
  canardDecodeScalar(transfer, bit_offset, UINT8, UNSIGNED, data_type_kind);
  bit_offset += UINT8;

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// Hardware Version

// Full name: uavcan.protocol.HardwareVersion

int32_t decode_hardware_version(
  const CanardRxTransfer* transfer,
  uint32_t bit_offset,
  HardwareVersion* hardware_version
)
{
  canardDecodeScalar(transfer, bit_offset, UINT8, UNSIGNED, &hardware_version->major);
  bit_offset += UINT8;

  canardDecodeScalar(transfer, bit_offset, UINT8, UNSIGNED, &hardware_version->minor);
  bit_offset += UINT8;

  for(int n = 0; n < 16; n++)
  {
    canardDecodeScalar(transfer, bit_offset, UINT8, UNSIGNED, &hardware_version->unique_id[n]);
    bit_offset += UINT8;
  }

  for(int n = 0; n < 255; n++)
  {
    canardDecodeScalar(transfer, bit_offset, UINT8, UNSIGNED, &hardware_version->certificate[n]);
    bit_offset += UINT8;
  }

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// Software Version

// Full name: uavcan.protocol.SoftwareVersion

int32_t decode_software_version(
  const CanardRxTransfer* transfer,
  uint32_t bit_offset,
  SoftwareVersion* software_version
)
{
  canardDecodeScalar(transfer, bit_offset, UINT8, UNSIGNED, &software_version->major);
  bit_offset += UINT8;

  canardDecodeScalar(transfer, bit_offset, UINT8, UNSIGNED, &software_version->minor);
  bit_offset += UINT8;

  canardDecodeScalar(transfer, bit_offset, UINT8, UNSIGNED, &software_version->optional_field_flags);
  bit_offset += UINT8;

  canardDecodeScalar(transfer, bit_offset, UINT32, UNSIGNED, &software_version->vcs_commit);
  bit_offset += UINT32;

  canardDecodeScalar(transfer, bit_offset, UINT64, UNSIGNED, &software_version->image_crc);
  bit_offset += UINT64;

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// Key Value

// Full name: uavcan.protocol.debug.KeyValue

int32_t decode_key_value(
  const CanardRxTransfer* transfer,
  uint32_t bit_offset,
  KeyValue* key_value
)
{
  canardDecodeScalar(transfer, bit_offset, FLOAT32, SIGNED, &key_value->value);
  bit_offset += FLOAT32;

  for(int n = 0; n < 58; n++)
  {
    canardDecodeScalar(transfer, bit_offset, UINT8, UNSIGNED, &key_value->key[n]);
    bit_offset += UINT8;
  }

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// Log Message

// Full name: uavcan.protocol.debug.LogMessage

int32_t decode_log_message(
  const CanardRxTransfer* transfer,
  uint32_t bit_offset,
  LogMessage* log_message
)
{
  decode_log_level(transfer, bit_offset, &log_message->level);
  bit_offset += LOG_LEVEL_DATA_TYPE_SIZE;

  for(int n = 0; n < 31; n++)
  {
    canardDecodeScalar(transfer, bit_offset, UINT8, UNSIGNED, &log_message->source[n]);
    bit_offset += UINT8;
  }

  for(int n = 0; n < 90; n++)
  {
    canardDecodeScalar(transfer, bit_offset, UINT8, UNSIGNED, &log_message->text[n]);
    bit_offset += UINT8;
  }

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------

// Log Level

// Full name: uavcan.protocol.debug.LogLevel

int32_t decode_log_level(
  const CanardRxTransfer* transfer,
  uint32_t bit_offset,
  LogLevel* log_level
)
{
  canardDecodeScalar(transfer, bit_offset, UINT3, UNSIGNED, log_level);
  bit_offset += UINT3;

  return (int32_t)bit_offset;
}

//------------------------------------------------------------------------------
