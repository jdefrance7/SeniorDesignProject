/**
 * @file uavcan_decoding_v0.h
 *
 * Support functions to handle decoding serialized bytes to UAVCAN data types.
 *
 * @author Joe DeFrance
 */

#ifndef UAVCAN_DECODING_H
#define UAVCAN_DECODING_H

//------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <libcanard_v0.h>

#include "uavcan_dsdl_sizes_v0.h"
#include "uavcan_data_types_v0.h"

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------

// Functions return new bit_offset

// --- OR ---

#define DECODING_BUFFER_OVERFLOW -1

//------------------------------------------------------------------------------

// Decoding Constants

#define SIGNED true

#define UNSIGNED false

//------------------------------------------------------------------------------

// Coarse Orientation

// Full Name: uavcan.CoarseOrientation

int32_t decode_coarse_orientation(
  CanardRxTransfer* transfer,
  uint32_t bit_offset,
  CoarseOrientation* coarse_orientation
);

//------------------------------------------------------------------------------

// Timestamp

// Full Name: uavcan.Timestamp

int32_t decode_timestamp(
  CanardRxTransfer* transfer,
  uint32_t bit_offset,
  Timestamp* timestamp
);

//------------------------------------------------------------------------------

// AHRS Solution

// Full name: uavcan.equipment.ahrs.Solution

int32_t decode_ahrs_solution(
  CanardRxTransfer* transfer,
  uint32_t bit_offset,
  AhrsSolution* ahrs_solution
);

//------------------------------------------------------------------------------

// Angular Command

// Full name: uavcan.equipment.camera_gimbal.AngularCommand

int32_t decode_angular_command(
  CanardRxTransfer* transfer,
  uint32_t bit_offset,
  AngularCommand* angular_command
);

//------------------------------------------------------------------------------

// Camera Gimbal Status

// Full name: uavcan.equipment.camera_gimbal.Status

int32_t decode_camera_gimbal_status(
  CanardRxTransfer* transfer,
  uint32_t bit_offset,
  CameraGimbalStatus* camera_gimbal_status
);

//------------------------------------------------------------------------------

// Camera Gimbal Mode

// Full name: uavcan.equipment.camera_gimbal.Mode

int32_t decode_camera_gimbal_mode(
  CanardRxTransfer* transfer,
  uint32_t bit_offset,
  CameraGimbalMode* camera_gimbal_mode
);

//------------------------------------------------------------------------------

// Device Temperature

// Full name: uavcan.equipment.device.Temperature

int32_t decode_device_temperature(
  CanardRxTransfer* transfer,
  uint32_t bit_offset,
  DeviceTemperature* device_temperature
);

//------------------------------------------------------------------------------

// Range Sensor Measurement

// Full name: uavcan.equipment.range_sensor.Measurement

int32_t decode_range_sensor_measurement(
  CanardRxTransfer* transfer,
  uint32_t bit_offset,
  RangeSensorMeasurement* range_sensor_measurement
);

//------------------------------------------------------------------------------

// Get Node Info

// Full name: uavcan.protocol.GetNodeInfo

int32_t decode_get_node_info(
  CanardRxTransfer* transfer,
  uint32_t bit_offset,
  GetNodeInfo* get_node_info
);

//------------------------------------------------------------------------------

// Get Data Type Info

// Full name: uavcan.protocol.GetDataTypeInfo

int32_t decode_get_data_type_info(
  CanardRxTransfer* transfer,
  uint32_t bit_offset,
  GetDataTypeInfo* get_data_type_info
);

//------------------------------------------------------------------------------

// Node Status

// Full name: uavcan.protocol.NodeStatus

int32_t decode_node_status(
  CanardRxTransfer* transfer,
  uint32_t bit_offset,
  NodeStatus* node_status
);

//------------------------------------------------------------------------------

// Restart Node

// Full name: uavcan.protocol.RestartNode

int32_t decode_restart_node(
  CanardRxTransfer* transfer,
  uint32_t bit_offset,
  RestartNode* restart_node
);

//------------------------------------------------------------------------------

// Data Type Kind

// Full name: uavcan.protocol.DataTypeKind

int32_t decode_data_type_kind(
  CanardRxTransfer* transfer,
  uint32_t bit_offset,
  DataTypeKind* data_type_kind
);

//------------------------------------------------------------------------------

// Hardware Version

// Full name: uavcan.protocol.HardwareVersion

int32_t decode_hardware_version(
  CanardRxTransfer* transfer,
  uint32_t bit_offset,
  HardwareVersion* hardware_version
);

//------------------------------------------------------------------------------

// Software Version

// Full name: uavcan.protocol.SoftwareVersion

int32_t decode_software_version(
  CanardRxTransfer* transfer,
  uint32_t bit_offset,
  SoftwareVersion* software_version
);

//------------------------------------------------------------------------------

// Key Value

// Full name: uavcan.protocol.debug.KeyValue

int32_t decode_key_value(
  CanardRxTransfer* transfer,
  uint32_t bit_offset,
  KeyValue* key_value
);

//------------------------------------------------------------------------------

// Log Message

// Full name: uavcan.protocol.debug.LogMessage

int32_t decode_log_message(
  CanardRxTransfer* transfer,
  uint32_t bit_offset,
  LogMessage* log_message
);

//------------------------------------------------------------------------------

// Log Level

// Full name: uavcan.protocol.debug.LogLevel

int32_t decode_log_level(
  CanardRxTransfer* transfer,
  uint32_t bit_offset,
  LogLevel* log_level
);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------------

#endif // UAVCAN_DECODING_H
