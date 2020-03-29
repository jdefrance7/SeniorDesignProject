#include "ast_can.h"

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
  canInit(can.bitrate);

  // Initialization success
  return 0;
}

int sendCanardCANFrame(CanardInstance* canard, CanardCANFrame* txf, unsigned int timeout_ms)
{
  // CAN message object
  st_cmd_t txMsg;

  // Timeout counter
  uint64_t timeout;

  // Create transmit buffer
  uint8_t txBuffer[8] = {};

  // Clear transmit buffer
  memset(txBuffer, 0, sizeof(txBuffer));

  // Assign transmit buffer
  txMsg.pt_data = &txBuffer[0];

  // Write transmission data
  memcpy(txBuffer, txf->data, txf->data_len);

  // Identifier Extension Bit = 1 (CAN 2.0B)
  txMsg.ctrl.ide = 1;

  // Message ID
  txMsg.id.ext   = txf->id & CANARD_CAN_EXT_ID_MASK;

  // Data Lenght Code (DLC)
  txMsg.dlc      = txf->data_len;

  // Remote Transfer Request (RTR) = 0 (Message)
  txMsg.ctrl.rtr = 0;

  // Send transfer command to the CAN controller
  txMsg.cmd = CMD_TX_DATA;

  // Wait for the command to be accepted by the controller
  timeout = millis();
  while(can_cmd(&txMsg) != CAN_CMD_ACCEPTED)
  {
    if((millis() - timeout) > timeout_ms)
    {
      return -1;
    }
  }

  // Wait for command to finish executing
  timeout = millis();
  while(can_get_status(&txMsg) == CAN_STATUS_NOT_COMPLETED)
  {
    if((millis() - timeout) > timeout_ms)
    {
      return 1;
    }
  }

  // Remove frame from Canard queue
  canardPopTxQueue(canard);

  // Return success
  return 0;
}

int readCanardCANFrame(CanardInstance* canard, CanardCANFrame* rxf, unsigned int timeout_ms)
{
  // CAN message object
  st_cmd_t rxMsg;

  // Timeout counter
  uint64_t timeout;

  // Create receive buffer
  uint8_t rxBuffer[8];

  // Clear receive buffer
  memset(rxBuffer, 0, sizeof(rxBuffer));

  // Assign receive buffer
  rxMsg.pt_data = &rxBuffer[0];

  // Send transfer command to the CAN controller
  rxMsg.cmd = CMD_RX_DATA;

  // Wait for the command to be accepted by the controller
  timeout = millis();
  while(can_cmd(&rxMsg) != CAN_CMD_ACCEPTED)
  {
    if((millis() - timeout) > timeout_ms)
    {
      return -1;
    }
  }

  // Wait for command to finish executing
  timeout = millis();
  while(can_get_status(&rxMsg) == CAN_STATUS_NOT_COMPLETED)
  {
    if((millis() - timeout) > timeout_ms)
    {
      return 1;
    }
  }

  // Get message ID
  rxf->id = rxMsg.id.ext;

  // Get message Data
  memcpy(rxf->data, rxMsg.pt_data, rxMsg.dlc);

  // Get message DLC
  rxf->data_len = rxMsg.dlc;

  // Process received message frame
  return canardHandleRxFrame(canard, rxf, 1000*millis());
}

int transmitCanardQueue(CanardInstance* canard, int timeout_ms)
{
  // Return value from send frame
  int reVal;

  // Iterate through Canard queue
  for(CanardCANFrame* txf = NULL; (txf = canardPeekTxQueue(canard)) != NULL;)
  {
    // Send CAN frame
    reVal = sendCanardCANFrame(canard, txf, timeout_ms);

    // Success
    if(reVal == 0)
    {
      // continue
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
