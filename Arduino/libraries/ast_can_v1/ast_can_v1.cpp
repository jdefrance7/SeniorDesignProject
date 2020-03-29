#include "ast_can_v1.h"

int init_can(Canard can, uint8_t id)
{
  // Check valid node ID
  if(id == 0)
  {
    return -1;
  }

  // Initialize Canard instance
  canardInit(
    CanardMemoryAllocate(&(can.canard), CANARD_MEMORY_POOL_SIZE),
    CanardMemoryFree(&(can.canard), can.canard_memory_pool)
  );

  // Set Canard node ID
  can.canard.node_id = id;

  // Init CAN module
  canInit(can.bitrate);

  // Initialization success
  return 0;
}

int sendCanardFrame(CanardInstance* canard, CanardFrame* txf, unsigned int timeout_ms)
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
  memcpy(txBuffer, txf->payload, txf->payload_size);

  // Identifier Extension Bit = 1 (CAN 2.0B)
  txMsg.ctrl.ide = 1;

  // Message ID
  txMsg.id.ext   = txf->extended_can_id;

  // Data Lenght Code (DLC)
  txMsg.dlc      = txf->payload_size;

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

  // Return success
  return 0;
}

int readCanardFrame(CanardInstance* canard, CanardFrame* rxf, uint8_t transport_index, CanardTransfer* transfer, unsigned int timeout_ms)
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
  rxf->extended_can_id = rxMsg.id.ext;

  // Get message Data
  memcpy(rxf->payload, rxMsg.pt_data, rxMsg.dlc);

  // Get message DLC
  rxf->payload_size = rxMsg.dlc;

  // Process received message frame
  return canardRxAccept(canard, rxf, transport_index, transfer);
}

int transmitCanardQueue(CanardInstance* canard, int timeout_ms)
{
  // Return value from send frame
  int reVal;

  // Iterate through Canard queue
  for(CanardFrame* txf = NULL; (txf = canardTxPeek(canard)) != NULL;)
  {
    // Send CAN frame
    reVal = sendCanardFrame(canard, txf, timeout_ms);

    // Success
    if(reVal == 0)
    {
      // Remove frame from Canard queue
      canardTxPop(canard);
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
