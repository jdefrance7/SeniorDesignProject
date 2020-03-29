#include "avr_can_v0.h"

int init_can(Canard can, uint8_t id)
{
  // Check node ID
  if(id == 0)
  {
    return -1;
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
    return -1;
  }

  // Set CAN filters
  if(canardAVRConfigureAcceptanceFilters(id) != 1)
  {
    return -1;
  }

  // Initialization success
  return 0;
}

int sendCanardCANFrame(CanardInstance* canard, CanardCANFrame* txf, unsigned int timeout_ms)
{
  // Send CAN frame
  const int16_t tx_res = canardAVRTransmit(txf);

  // Error sending frame
  if(tx_res < 0)
  {
    canardPopTxQueue(canard);
    return -1;
  }
  // Frame sent successfully
  else if (tx_res > 0)
  {
    return 0;
  }
  // Timeout
  else
  {
    return -1;
  }
}

int readCanardCANFrame(CanardInstance* canard, CanardCANFrame* rxf, unsigned int timeout_ms)
{
  // Read CAN frame
  const int16_t rx_res = canardAVRReceive(rxf);

  // Error reading frame
  if(rx_res < 0)
  {
    return -1;
  }
  // Frame received
  else if(rx_res > 0)
  {
    return canardHandleRxFrame(canard, rxf, 1000*millis());
  }
  // Timeout
  else
  {
    return 1;
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
    if(reVal == 0)
    {
      // Remove frame from Canard queue
      canardPopTxQueue(canard);
    }
    // Timeout
    else if(reVal == 1)
    {
      return 1;
    }
    // Error
    else
    {
      return -1;
    }
  }

  // Return success
  return 0;
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
