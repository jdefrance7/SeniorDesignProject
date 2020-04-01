#include "avr_can_v0.h"

int init_can(Canard can, uint8_t id)
{
  // Check node ID
  if(id == 0)
  {
    return ERR_INVALID_NODE_ID;
  }

  // Initialize canard instance
  canardInit(
    &can.canard,
    can.canard_memory_pool,
    sizeof(can.canard_memory_pool),
    onTransferReceived,
    shouldAcceptTransfer,
    NULL
  );

  // Set canard ID
  canardSetLocalNodeID(&can.canard, id);

  // Init CAN module
  if(canardAVRInit(can.bitrate) != 0)
  {
    return ERR_CAN_MODULE_INIT;
  }

  // Set CAN filters
  if(canardAVRConfigureAcceptanceFilters(id) != 1)
  {
    return ERR_CAN_FILTER_INIT;
  }

  // Initialization success
  return CANARD_OK;
}

int sendCanardCANFrame(CanardInstance* canard, CanardCANFrame* txf, unsigned int timeout_ms)
{
  // Send Canard CAN frame
  const int16_t tx_res = canardAVRTransmit(txf);

  // Error
  if(tx_res < 0)
  {
    canardPopTxQueue(canard);
    return tx_res;
  }
  // Success
  else if (tx_res > 0)
  {
    return CANARD_OK;
  }
  // Timeout
  else
  {
    return ERR_CANARD_AVR_TIMEOUT;
  }
}

int readCanardCANFrame(CanardInstance* canard, CanardCANFrame* rxf, unsigned int timeout_ms)
{
  // Read CAN frame
  const int16_t rx_res = canardAVRReceive(rxf);

  // Error
  if(rx_res < 0)
  {
    return rx_res;
  }
  // Success
  else if(rx_res > 0)
  {
    return canardHandleRxFrame(canard, rxf, 1000*millis());
    /*
      canardHandleRxFrame return values
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
  }
  // Timeout
  else
  {
    return ERR_CANARD_AVR_TIMEOUT;
  }
}

int transmitCanardQueue(CanardInstance* canard, int timeout_ms)
{
  // Return value from send frame
  int reVal;

  // Iterate through Canard queue
  for (const CanardCANFrame* txf = NULL; (txf = canardPeekTxQueue(canard)) != NULL;)
  {
    // Send CAN frame
    reVal = sendCanardCANFrame(canard, txf, timeout_ms);

    // Success
    if(reVal == CANARD_OK)
    {
      // Remove frame from Canard queue
      canardPopTxQueue(canard);
    }
    // Timeout
    else if(reVal == ERR_CANARD_AVR_TIMEOUT)
    {
      return ERR_CANARD_AVR_TIMEOUT;
    }
    // Error
    else
    {
      return reVal;
    }
  }

  // Return success
  return CANARD_OK;
}

/**
 * This callback is invoked by the library when a new message or request or response is received.
 */
void onTransferReceived(CanardInstance* ins, CanardRxTransfer* transfer)
{
  if(transfer->transfer_type == CanardTransferTypeResponse)
  {
    // Add response handlers here
  }
  else if(transfer->transfer_type == CanardTransferTypeRequest)
  {
    // Add request handlers here

    // // Example Handler: Get Node Info
    // if(transfer->data_type_id == GET_NODE_INFO_DATA_TYPE_ID)
    // {
    //   // send GET_NODE_INFO response
    // }
  }
  else if(transfer->transfer_type == CanardTransferTypeBroadcast)
  {
    // Add braodcast handlers here
  }
}

/**
 * This callback is invoked by the library when it detects beginning of a new transfer on the bus that can be received
 * by the local node.
 * If the callback returns true, the library will receive the transfer.
 * If the callback returns false, the library will ignore the transfer.
 * All transfers that are addressed to other nodes are always ignored.
 */
bool shouldAcceptTransfer(const CanardInstance* ins,
                                 uint64_t* out_data_type_signature,
                                 uint16_t data_type_id,
                                 CanardTransferType transfer_type,
                                 uint8_t source_node_id)
{
  (void)source_node_id;

  static bool accept_transfer;

  accept_transfer = false;

  if(transfer_type == CanardTransferTypeResponse)
  {
    // Add response callbacks here
  }
  else if(transfer_type == CanardTransferTypeRequest)
  {
    // Add request callbacks here

    // // Example Handler: Node Info
    // if(data_type_id == NODE_INFO_DATA_TYPE_ID)
    // {
    //   *out_data_type_signature = NODE_INFO_DATA_TYPE_SIGNATURE;
    //   accept_transfer = true;
    // }
  }
  else if(transfer_type == CanardTransferTypeBroadcast)
  {
    // Add broadcast callbacks here
  }

  return accept_transfer;
}
