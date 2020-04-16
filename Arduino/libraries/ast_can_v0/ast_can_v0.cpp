#include "ast_can_v0.h"

void printCanard(Canard* can)
{
  Serial.println("\nCan");
  Serial.print("  Bitrate: ");  Serial.println(can->bitrate);
  printCanardInstance(&(can->canard));
  Serial.print("  Memory: ");   Serial.println(CANARD_MEMORY_POOL_SIZE);
  CanardPoolAllocatorStatistics stats = canardGetPoolAllocatorStatistics(&(can->canard));
  printCanardPoolAllocatorStatistics(&stats);
}

void printMsg(st_cmd_t* msg)
{
  Serial.println("\nASTCanLib Message");
  Serial.print("  ID: ");   Serial.println(msg->id.ext, HEX);
  Serial.print("  IDE: ");  Serial.println(msg->ctrl.ide, BIN);
  Serial.print("  RTR: ");  Serial.println(msg->ctrl.rtr, BIN);
  Serial.print("  DLC: ");  Serial.println(msg->dlc);
  Serial.print("  Data: ");
  for(int n = 0; n < msg->dlc; n++)
  {
    Serial.print((byte)msg->pt_data[n], HEX);

    if(n == (msg->dlc-1))
    {
      Serial.print("\n");
    }

    Serial.print(",");
  }
}

int init_can(Canard* can, uint8_t id)
{
  // Check valid node ID
  if(id == 0)
  {
    return ERR_INVALID_NODE_ID;
  }

  // Initialize canard instance
  canardInit(
    &(can->canard),
    &(can->canard_memory_pool),
    sizeof(can->canard_memory_pool),
    onTransferReceived,
    shouldAcceptTransfer,
    NULL
  );

  // Set canard ID
  canardSetLocalNodeID(&(can->canard), id);

  // Init CAN module
  canInit(can->bitrate);

  // Initialization success
  return CANARD_OK;
}

int sendCanardCANFrame(CanardInstance* canard, CanardCANFrame* txf, unsigned int timeout_ms)
{
  // CAN message object
  st_cmd_t txMsg;

  // Timeout counter
  uint64_t timeout;

  // Create data field
  uint8_t txBuffer[8] = {};

  // Clear data field
  memset(txBuffer, 0, sizeof(txBuffer));

  // Assign data field
  txMsg.pt_data = &txBuffer[0];

  // Write data field
  memcpy(txBuffer, txf->data, txf->data_len);

  // Identifier Extension Bit = 1 (CAN 2.0B)
  txMsg.ctrl.ide = 1;

  // Extended Message ID
  txMsg.id.ext   = txf->id & CANARD_CAN_EXT_ID_MASK;

  // Data Lenght Code (DLC)
  txMsg.dlc      = txf->data_len;

  // Remote Transfer Request (RTR) = 0 (Message)
  txMsg.ctrl.rtr = 0;

  // Send transfer command to the CAN controller
  txMsg.cmd = CMD_TX_DATA;

  // Wait for the command to be accepted by the controller
  // timeout = millis();
  // while(can_cmd(&txMsg) != CAN_CMD_ACCEPTED)
  // {
  //   if((millis() - timeout) > timeout_ms)
  //   {
  //     return ERR_COMMAND_ACCEPT_TIMEOUT; // timed out
  //   }
  // }

  // Wait for command to finish executing
  // timeout = millis();
  // while(can_get_status(&txMsg) == CAN_STATUS_NOT_COMPLETED)
  // {
  //   if((millis() - timeout) > timeout_ms)
  //   {
  //     return ERR_COMMAND_EXECUTE_TIMEOUT; // timed out
  //   }
  // }

  while(can_cmd(&txMsg) != CAN_CMD_ACCEPTED);
  while(can_get_status(&txMsg) == CAN_STATUS_NOT_COMPLETED);

  // Return success
  return CANARD_OK;
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
      return ERR_COMMAND_ACCEPT_TIMEOUT; // timed out
    }
  }

  // Wait for command to finish executing
  timeout = millis();
  while(can_get_status(&rxMsg) == CAN_STATUS_NOT_COMPLETED)
  {
    if((millis() - timeout) > timeout_ms)
    {
      return ERR_COMMAND_EXECUTE_TIMEOUT; // timed out
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

int transmitCanardQueue(CanardInstance* canard, unsigned int delay_ms, unsigned int timeout_ms)
{
  // Return value from send frame
  int reVal;
  int frames = 0;

  // Iterate through Canard queue
  for(CanardCANFrame* txf = NULL; (txf = canardPeekTxQueue(canard)) != NULL;)
  {
    // Send CAN frame
    reVal = sendCanardCANFrame(canard, txf, timeout_ms);

    delay(delay_ms);

    // Success
    if(reVal == CANARD_OK)
    {
      // Remove frame from Canard queue
      canardPopTxQueue(canard);
      frames++;
    }
    // Timeout
    else if(reVal == ERR_COMMAND_EXECUTE_TIMEOUT)
    {
      return ERR_COMMAND_EXECUTE_TIMEOUT;
    }
    // Error
    else
    {
      return reVal;
    }
  }

  // Return success
  return frames;
}

/**
 * This callback is invoked by the library when a new message or request or response is received.
 */
void onTransferReceived(CanardInstance* ins, CanardRxTransfer* transfer)
{
  if(transfer->transfer_type == CanardTransferTypeResponse)
  {
    // Add response handlers here

    //...
  }
  else if(transfer->transfer_type == CanardTransferTypeRequest)
  {
    // Add request handlers here

    //...

    /*
      // Example Handler: Get Node Info
      if(transfer->data_type_id == GET_NODE_INFO_DATA_TYPE_ID)
      {
        // send GET_NODE_INFO response
      }
    */
  }
  else if(transfer->transfer_type == CanardTransferTypeBroadcast)
  {
    // Add braodcast handlers here

    //...
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
    // Add response handlers here

    //...
  }
  else if(transfer_type == CanardTransferTypeRequest)
  {
    // Add request handlers here

    //...

    /*
      // Example Handler: Node Info
      if(data_type_id == NODE_INFO_DATA_TYPE_ID)
      {
        *out_data_type_signature = NODE_INFO_DATA_TYPE_SIGNATURE;
        accept_transfer = true;
      }
    */
  }
  else if(transfer_type == CanardTransferTypeBroadcast)
  {
    // Add broadcast handlers here

    //...
  }

  return accept_transfer;
}
