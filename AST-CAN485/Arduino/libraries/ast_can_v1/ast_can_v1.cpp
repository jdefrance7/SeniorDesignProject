/**
 * @file ast_can_v1.cpp
 *
 * CAN driver for the AST-CAN485 board using ASTCanLib and Libcanard v1.
 *
 * @author Joe DeFrance
 */

#include "ast_can_v1.h"

void printCanard(Canard* can)
{
  Serial.println("\nCan");
  Serial.print("  Bitrate: ");  Serial.println(can->bitrate);
  printCanardInstance(&(can->canard));
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

int init_can(Canard can, uint8_t id)
{
  // Check valid node ID
  if(id == 0)
  {
    return ERR_INVALID_NODE_ID;
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
  return CANARD_OK;
}

int sendCanardFrame(CanardInstance* canard, CanardFrame* txf, unsigned int delay_ms, unsigned int timeout_ms)
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
      return ERR_COMMAND_ACCEPT_TIMEOUT;
    }
  }

  // Wait for command to finish executing
  timeout = millis();
  while(can_get_status(&txMsg) == CAN_STATUS_NOT_COMPLETED)
  {
    if((millis() - timeout) > timeout_ms)
    {
      return ERR_COMMAND_EXECUTE_TIMEOUT;
    }
  }

  delay(delay_ms);

  // Return success
  return CANARD_OK;
}

int readCanardFrame(CanardInstance* canard, CanardFrame* rxf, uint8_t transport_index, CanardTransfer* transfer, unsigned int delay_ms, unsigned int timeout_ms)
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
      return ERR_COMMAND_ACCEPT_TIMEOUT;
    }
  }

  // Wait for command to finish executing
  timeout = millis();
  while(can_get_status(&rxMsg) == CAN_STATUS_NOT_COMPLETED)
  {
    if((millis() - timeout) > timeout_ms)
    {
      return ERR_COMMAND_EXECUTE_TIMEOUT;
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
  /*
    canardRxAccept return values
      TRANSFER_COMPLETE               (1)
      INVALID_FRAME                   (0)
      NO_SUBSCRIPTION                 (0)
      TRANSFER_INCOMPLETE             (0)
      INVALID_FRAME_SEQUENCE          (0)
      DUPLICATE_FRAME                 (0)
      ADDRESS_MISMATCH                (0)
      CANARD_ERROR_INVALID_ARGUMENT   (-2)
      CANARD_ERROR_OUT_OF_MEMORY      (-3)
  */
}

int transmitCanardQueue(CanardInstance* canard, unsigned int delay_ms, unsigned int timeout_ms)
{
  // Return value from send frame
  int reVal;

  // Iterate through Canard queue
  for(CanardFrame* txf = NULL; (txf = canardTxPeek(canard)) != NULL;)
  {
    // Send CAN frame
    reVal = sendCanardFrame(canard, txf, delay_ms, timeout_ms);

    // Success
    if(reVal == CANARD_OK)
    {
      // Remove frame from Canard queue
      canardTxPop(canard);
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
  return CANARD_OK;
}
