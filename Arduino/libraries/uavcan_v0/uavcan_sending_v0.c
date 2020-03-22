#include "uavcan_sending_v0.h"

//------------------------------------------------------------------------------

// Default

int8_t send_message(CanardInstance* canard, uint8_t buffer[], uint16_t buffer_size, uint64_t data_type_signature, uint16_t data_type_id, uint8_t* inout_transfer_id, uint8_t priority)
{
  canardBroadcast(canard, data_type_signature, data_type_id, inout_transfer_id, priority, buffer, buffer_size);

  return transmit(canard);
}

//------------------------------------------------------------------------------

// Custom

int8_t send_ahrs_solution(CanardInstance* canard, AhrsSolution ahrs_solution, uint8_t priority)
{
  static uint8_t transfer_id = 0;

  uint8_t buffer[((AHRS_SOLUTION_DATA_TYPE_SIZE - 1) / 8) + 1];
  memset(buffer, 0, sizeof(buffer));

  encode_ahrs_solution(buffer, sizeof(buffer), 0, ahrs_solution);

  canardBroadcast(canard, AHRS_SOLUTION_DATA_TYPE_SIGNATURE, AHRS_SOLUTION_DATA_TYPE_ID, &transfer_id, priority, buffer, sizeof(buffer));

  return transmit(canard);
}

int8_t send_camera_gimbal_status(CanardInstance* canard, CameraGimbalStatus camera_gimbal_status, uint8_t priority)
{
  static uint8_t transfer_id = 0;

  uint8_t buffer[((CAMERA_GIMBAL_STATUS_DATA_TYPE_SIZE - 1) / 8) + 1];
  memset(buffer, 0, sizeof(buffer));

  encode_camera_gimbal_status(buffer, sizeof(buffer), 0, camera_gimbal_status);

  canardBroadcast(canard, CAMERA_GIMBAL_STATUS_DATA_TYPE_SIGNATURE, CAMERA_GIMBAL_STATUS_DATA_TYPE_ID, &transfer_id, priority, buffer, sizeof(buffer));

  return transmit(canard);
}

int8_t send_device_temperature(CanardInstance* canard, DeviceTemperature device_temperature, uint8_t priority)
{
  static uint8_t transfer_id = 0;

  uint8_t buffer[((DEVICE_TEMPERATURE_DATA_TYPE_SIZE - 1) / 8) + 1];
  memset(buffer, 0, sizeof(buffer));

  encode_device_temperature(buffer, sizeof(buffer), 0, device_temperature);

  canardBroadcast(canard, DEVICE_TEMPERATURE_DATA_TYPE_SIGNATURE, DEVICE_TEMPERATURE_DATA_TYPE_ID, &transfer_id, priority, buffer, sizeof(buffer));

  return transmit(canard);
}

int8_t send_range_sensor_measurement(CanardInstance* canard, RangeSensorMeasurement range_sensor_measurement, uint8_t priority)
{
  static uint8_t transfer_id = 0;

  uint8_t buffer[((RANGE_SENSOR_MEASUREMENT_DATA_TYPE_SIZE - 1) / 8) + 1];
  memset(buffer, 0, sizeof(buffer));

  encode_range_sensor_measurement(buffer, sizeof(buffer), 0, range_sensor_measurement);

  canardBroadcast(canard, RANGE_SENSOR_MEASUREMENT_DATA_TYPE_SIGNATURE, RANGE_SENSOR_MEASUREMENT_DATA_TYPE_ID, &transfer_id, priority, buffer, sizeof(buffer));

  return transmit(canard);
}

int8_t send_get_node_info(CanardInstance* canard, GetNodeInfo get_node_info, uint8_t priority)
{
  static uint8_t transfer_id = 0;

  uint8_t buffer[((GET_NODE_INFO_DATA_TYPE_SIZE - 1) / 8) + 1];
  memset(buffer, 0, sizeof(buffer));

  encode_get_node_info(buffer, sizeof(buffer), 0, get_node_info);

  canardBroadcast(canard, GET_NODE_INFO_DATA_TYPE_SIGNATURE, GET_NODE_INFO_DATA_TYPE_ID, &transfer_id, priority, buffer, sizeof(buffer));

  return transmit(canard);
}

int8_t send_get_data_type_info(CanardInstance* canard, GetDataTypeInfo get_data_type_info, uint8_t priority)
{
  static uint8_t transfer_id = 0;

  uint8_t buffer[((GET_DATA_TYPE_INFO_DATA_TYPE_SIZE - 1) / 8) + 1];
  memset(buffer, 0, sizeof(buffer));

  encode_get_data_type_info(buffer, sizeof(buffer), 0, get_data_type_info);

  canardBroadcast(canard, GET_DATA_TYPE_INFO_DATA_TYPE_SIGNATURE, GET_DATA_TYPE_INFO_DATA_TYPE_ID, &transfer_id, priority, buffer, sizeof(buffer));

  return transmit(canard);
}

int8_t send_node_status(CanardInstance* canard, NodeStatus node_status, uint8_t priority)
{
  static uint8_t transfer_id = 0;

  uint8_t buffer[((NODE_STATUS_DATA_TYPE_SIZE - 1) / 8) + 1];
  memset(buffer, 0, sizeof(buffer));

  encode_node_status(buffer, sizeof(buffer), 0, node_status);

  canardBroadcast(canard, NODE_STATUS_DATA_TYPE_SIGNATURE, NODE_STATUS_DATA_TYPE_ID, &transfer_id, priority, buffer, sizeof(buffer));

  return transmit(canard);
}

int8_t send_restart_node(CanardInstance* canard, RestartNode restart_node, uint8_t priority)
{
  static uint8_t transfer_id = 0;

  uint8_t buffer[((RESTART_NODE_DATA_TYPE_SIZE - 1) / 8) + 1];
  memset(buffer, 0, sizeof(buffer));

  encode_restart_node(buffer, sizeof(buffer), 0, restart_node);

  canardBroadcast(canard, RESTART_NODE_DATA_TYPE_SIGNATURE, RESTART_NODE_DATA_TYPE_ID, &transfer_id, priority, buffer, sizeof(buffer));

  return transmit(canard);
}

int8_t send_key_value(CanardInstance* canard, KeyValue key_value, uint8_t priority)
{
  static uint8_t transfer_id = 0;

  uint8_t buffer[(KEY_VALUE_DATA_TYPE_SIZE / 8) + 1];
  memset(buffer, 0, sizeof(buffer));

  encode_key_value(buffer, sizeof(buffer), 0, key_value);

  canardBroadcast(canard, KEY_VALUE_DATA_TYPE_SIGNATURE, KEY_VALUE_DATA_TYPE_ID, &transfer_id, priority, buffer, sizeof(buffer));

  return transmit(canard);
}

int8_t send_log_message(CanardInstance* canard, LogMessage log_message, uint8_t priority)
{
  static uint8_t transfer_id = 0;

  uint8_t buffer[((LOG_MESSAGE_DATA_TYPE_SIZE - 1) / 8) + 1];
  memset(buffer, 0, sizeof(buffer));

  encode_log_message(buffer, sizeof(buffer), 0, log_message);

  canardBroadcast(canard, LOG_MESSAGE_DATA_TYPE_SIGNATURE, LOG_MESSAGE_DATA_TYPE_ID, &transfer_id, priority, buffer, sizeof(buffer));

  return transmit(canard);
}

//------------------------------------------------------------------------------

// Called at the end of each "send" to transmit the queue.

// Using Canard_AVR
#if defined(CANARD_AVR_H)
int8_t transmit(CanardInstance* canard)
{
  // Transmit until queue is empty
  for (const CanardCANFrame* txf = NULL; (txf = canardPeekTxQueue(canard)) != NULL;)
  {
    // Check transmit return value (Failure = -1, Success = 1, Timeout = 0)
    const int16_t tx_res = canardAVRTransmit(txf);
    if (tx_res < 0)
    {
      canardPopTxQueue(canard);
      return -1;
    }
    else if (tx_res > 0)
    {
      canardPopTxQueue(canard);
    }
    else
    {
      break;
    }
  }
  return 0;
}
#endif

// Using ASTCanLib
#if defined(_CAN_LIB_H_)
int8_t transmit(CanardInstance* canard)
{
  st_cmd_t txMsg;

  // Transmit until queue is empty
  for (const CanardCANFrame* txf = NULL; (txf = canardPeekTxQueue(canard)) != NULL;)
  {
    // Transmit buffer
    uint8_t txBuffer[8] = {};
    memset(txBuffer, 0, sizeof(txBuffer));

    // Allocate transmit buffer
    txMsg.pt_data = &txBuffer[0];
    memcpy(txBuffer, txf->data, txf->data_len);

    txMsg.ctrl.ide = 1; // CAN 2.0B
    txMsg.id.ext   = txf->id & CANARD_CAN_EXT_ID_MASK;
    txMsg.dlc      = txf->data_len;
    txMsg.ctrl.rtr = 0;

    // Send command to the CAN port controller
    txMsg.cmd = CMD_TX_DATA;       // send message
    // Wait for the command to be accepted by the controller
    while(can_cmd(&txMsg) != CAN_CMD_ACCEPTED);
    // Wait for command to finish executing
    while(can_get_status(&txMsg) == CAN_STATUS_NOT_COMPLETED);

    canardPopTxQueue(canard);
  }
  return 0;
}
#endif

//------------------------------------------------------------------------------
