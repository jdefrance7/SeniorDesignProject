/**
 * @file ast_can_v0.h
 *
 * CAN driver for the AST-CAN485 board using ASTCanLib and Libcanard v0.
 *
 * @author Joe DeFrance
 */

#ifndef AST_CAN_VERSION
#define AST_CAN_VERSION 0

#include <stdint.h> // primative data types

#include <libcanard_v0.h> // canard objects

#include <ASTCanLib.h> // driver library

#include <Arduino.h> // needed for millis()

/*
  Error values
*/
#define ERR_INVALID_NODE_ID          -1
#define ERR_COMMAND_ACCEPT_TIMEOUT   -1
#define ERR_COMMAND_EXECUTE_TIMEOUT   1

/*
  Size of the Canard queue.

  Should be increased if peak usage approaches 80%.
*/
#define CANARD_MEMORY_POOL_SIZE 1024

/*
  Custom Canard struct
*/
typedef struct
{
  long bitrate;
  CanardInstance canard;
  uint8_t canard_memory_pool[CANARD_MEMORY_POOL_SIZE];
} Canard;

/*
  Print function for custom Canard struct
*/
void printCanard(Canard* can);

/*
  Print function for ASTCanLib Messages
*/
void printMsg(st_cmd_t* msg);

/*
  Initialize the CAN module given Canard struct and Node ID.
*/
int init_can(Canard* can, uint8_t id);

/*
  Send a single Canard CAN Frame from the Canard queue.
*/
int sendCanardCANFrame(CanardInstance* canard, CanardCANFrame* txf, unsigned int timeout_ms);

/*
  Read a single Canard CAN Frame.
*/
int readCanardCANFrame(CanardInstance* canard, CanardCANFrame* rxf, unsigned int timeout_ms);

/*
  Transmit all Canard CAN Frames from the Canard queue.
*/
int transmitCanardQueue(CanardInstance* canard, unsigned int delay_ms, unsigned int timeout_ms);

/*
  Callback used by Canard to handle responses to incomming Canard CAN Frames.
*/
void onTransferReceived(CanardInstance* ins, CanardRxTransfer* transfer);

/*
  Callback used by Canard to filter incomming Canard CAN Frames.
*/
bool shouldAcceptTransfer(const CanardInstance* ins,
                                 uint64_t* out_data_type_signature,
                                 uint16_t data_type_id,
                                 CanardTransferType transfer_type,
                                 uint8_t source_node_id);

#endif // AST_CAN_VERSION
