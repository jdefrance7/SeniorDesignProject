#ifndef UAVCAN_SENDING_H
#define UAVCAN_SENDING_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Choose CAN Driver
#include <ASTCanLib.h>
// #include <canard_avr.h>

#include <canard.h>

#include "uavcan_node_struct_v0.h"
#include "uavcan_data_types_v0.h"
#include "uavcan_encoding_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------

// Default

int8_t send_message(CanardInstance* canard, uint8_t buffer[], uint16_t buffer_size, uint64_t data_type_signature, uint16_t data_type_id, uint8_t* inout_transfer_id, uint8_t priority);

//------------------------------------------------------------------------------

// Custom

int8_t send_ahrs_solution(CanardInstance* canard, AhrsSolution ahrs_solution, uint8_t priority);

int8_t send_camera_gimbal_status(CanardInstance* canard, CameraGimbalStatus camera_gimbal_status, uint8_t priority);

int8_t send_device_temperature(CanardInstance* canard, DeviceTemperature device_temperature, uint8_t priority);

int8_t send_range_sensor_measurement(CanardInstance* canard, RangeSensorMeasurement range_sensor_measurement, uint8_t priority);

int8_t send_get_node_info(CanardInstance* canard, GetNodeInfo get_node_info, uint8_t priority);

int8_t send_get_data_type_info(CanardInstance* canard, GetDataTypeInfo get_data_type_info, uint8_t priority);

int8_t send_node_status(CanardInstance* canard, NodeStatus node_status, uint8_t priority);

int8_t send_restart_node(CanardInstance* canard, RestartNode restart_node, uint8_t priority);

int8_t send_key_value(CanardInstance* canard, KeyValue key_value, uint8_t priority);

int8_t send_log_message(CanardInstance* canard, LogMessage log_message, uint8_t priority);

//------------------------------------------------------------------------------

// Called at the end of each "send" to transmit the queue.

int8_t transmit(CanardInstance* canard);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif // UAVCAN_SENDING_H
