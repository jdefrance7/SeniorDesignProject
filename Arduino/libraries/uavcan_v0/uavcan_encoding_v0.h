#ifndef UAVCAN_ENCODING_H
#define UAVCAN_ENCODING_H

/*
  Support functions to handle encoding UAVCAN data types into serialized bytes.
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <canard.h>

#include "uavcan_dsdl_sizes_v0.h"
#include "uavcan_data_types_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------

// Functions return bit_offset index

// --- OR ---

#define ENCODING_BUFFER_OVERFLOW -1

//------------------------------------------------------------------------------

// Coarse Orientation

// Full Name: uavcan.CoarseOrientation

int32_t encode_coarse_orientation(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  CoarseOrientation coarse_orientation
);

//------------------------------------------------------------------------------

// Timestamp

// Full Name: uavcan.Timestamp

int32_t encode_timestamp(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  Timestamp timestamp
);

//------------------------------------------------------------------------------

// AHRS Solution

// Full name: uavcan.equipment.ahrs.Solution

int32_t encode_ahrs_solution(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  AhrsSolution ahrs_solution
);

//------------------------------------------------------------------------------

// Camera Gimbal Status

// Full name: uavcan.equipment.camera_gimbal.Status

int32_t encode_camera_gimbal_status(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  CameraGimbalStatus camera_gimbal_status
);

//------------------------------------------------------------------------------

// Camera Gimbal Mode

// Full name: uavcan.equipment.camera_gimbal.Mode

int32_t encode_camera_gimbal_mode(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  CameraGimbalMode camera_gimbal_mode
);

//------------------------------------------------------------------------------

// Device Temperature

// Full name: uavcan.equipment.device.Temperature

int32_t encode_device_temperature(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  DeviceTemperature device_temperature
);

//------------------------------------------------------------------------------

// Range Sensor Measurement

// Full name: uavcan.equipment.range_sensor.Measurement

int32_t encode_range_sensor_measurement(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  RangeSensorMeasurement range_sensor_measurement
);

//------------------------------------------------------------------------------

// Get Node Info

// Full name: uavcan.protocol.GetNodeInfo

int32_t encode_get_node_info(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  GetNodeInfo get_node_info
);

//------------------------------------------------------------------------------

// Get Data Type Info

// Full name: uavcan.protocol.GetDataTypeInfo

int32_t encode_get_data_type_info(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  GetDataTypeInfo get_data_type_info
);

//------------------------------------------------------------------------------

// Node Status

// Full name: uavcan.protocol.NodeStatus

int32_t encode_node_status(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  NodeStatus node_status
);

//------------------------------------------------------------------------------

// Restart Node

// Full name: uavcan.protocol.RestartNode

int32_t encode_restart_node(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  RestartNode restart_node
);

//------------------------------------------------------------------------------

// Data Type Kind

// Full name: uavcan.protocol.DataTypeKind

int32_t encode_data_type_kind(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  DataTypeKind data_type_kind
);

//------------------------------------------------------------------------------

// Hardware Version

// Full name: uavcan.protocol.HardwareVersion

int32_t encode_hardware_version(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  HardwareVersion hardware_version
);

//------------------------------------------------------------------------------

// Software Version

// Full name: uavcan.protocol.SoftwareVersion

int32_t encode_software_version(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  SoftwareVersion software_version
);

//------------------------------------------------------------------------------

// Key Value

// Full name: uavcan.protocol.debug.KeyValue

int32_t encode_key_value(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  KeyValue key_value
);

//------------------------------------------------------------------------------

// Log Message

// Full name: uavcan.protocol.debug.LogMessage

int32_t encode_log_message(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  LogMessage log_message
);

//------------------------------------------------------------------------------

// Log Level

// Full name: uavcan.protocol.debug.LogLevel

int32_t encode_log_level(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  LogLevel log_level
);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif // UAVCAN_ENCODING_H
