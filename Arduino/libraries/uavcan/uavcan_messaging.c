#include "uavcan_messaging.h"

int16_t read_message(
  CanardCANFrame* rx_frame,
  const uint64_t timestamp
)
{
  // Receive CAN frame (Failure = -1, Success = 1, Timeout = 0)
  const int16_t rx_res = canardAVRReceive(rx_frame);
  if (rx_res < 0)
  {
    // Error receiving frame
    return -1;
  }
  else if (rx_res > 0)
  {
    /* canardHandleRxFrame return values
      CANARD_OK                                  0
      CANARD_ERROR_INVALID_ARGUMENT              -2
      CANARD_ERROR_OUT_OF_MEMORY                 -3
      CANARD_ERROR_NODE_ID_NOT_SET               -4
      CANARD_ERROR_INTERNAL                      -9
      CANARD_ERROR_RX_INCOMPATIBLE_PACKET        -10
      CANARD_ERROR_RX_WRONG_ADDRESS              -11
      CANARD_ERROR_RX_NOT_WANTED                 -12
      CANARD_ERROR_RX_MISSED_START               -13
      CANARD_ERROR_RX_WRONG_TOGGLE               -14
      CANARD_ERROR_RX_UNEXPECTED_TID             -15
      CANARD_ERROR_RX_SHORT_FRAME                -16
      CANARD_ERROR_RX_BAD_CRC                    -17
    */
    return canardHandleRxFrame(&g_canard, rx_frame, timestamp);
  }
  else
  {
    // No frames available
    return 1;
  }
}

int16_t push_message(
  uint64_t data_type_signature,
  uint16_t data_type_id,
  uint8_t* inout_transfer_id,
  uint8_t priority,
  const void* payload,
  uint16_t payload_len
)
{
  return canardBroadcast(
   &g_canard,
   data_type_signature,
   data_type_id,
   inout_transfer_id,
   priority,
   payload,
   payload_len
  );
}

int16_t send_message()
{
  const CanardCANFrame* txf;
  if(txf = canardPeekTxQueue(&g_canard) != NULL)
  {
    // Check transmit return value (Failure = -1, Success = 1, Timeout = 0)
    const int16_t tx_res = canardAVRTransmit(txf);
    if (tx_res < 0)
    {
      canardPopTxQueue(&g_canard);
      return -1;
    }
    else if (tx_res > 0)
    {
      canardPopTxQueue(&g_canard);
      return 0;
    }
    else
    {
      return 1; // Timeout
    }
  }
  else
  {
    return -1;
  }
}

int16_t flush_messages()
{
  // Transmit until queue is empty
  for (const CanardCANFrame* txf = NULL; (txf = canardPeekTxQueue(&g_canard)) != NULL;)
  {
    // Check transmit return value (Failure = -1, Success = 1, Timeout = 0)
    const int16_t tx_res = canardAVRTransmit(txf);
    if (tx_res < 0)
    {
      canardPopTxQueue(&g_canard);
      return -1;
    }
    else if (tx_res > 0)
    {
      canardPopTxQueue(&g_canard);
    }
    else
    {
      break;
    }
  }
  return 0;
}
