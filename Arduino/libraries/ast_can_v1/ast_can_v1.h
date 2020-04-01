#ifndef AST_CAN_VERSION
#define AST_CAN_VERSION 1

/*
  CAN driver for the AST-CAN485 board using ASTCanLib and Libcanard v1.
*/

#include <stdint.h> // primative data types

#include <libcanard_v1.h> // canard objects

#include <ASTCanLib.h> // driver library

#include <Arduino.h> // needed for millis()

/*
  Legacy return value
*/
#define CANARD_OK 0

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
  void* canard_memory_pool;
} Canard;

/*
  Initialize the CAN module given Canard struct and Node ID.
*/
int init_can(Canard can, uint8_t id);

/*
  Send a single Canard CAN Frame from the Canard queue.
*/
int sendCanardFrame(CanardInstance* canard, CanardFrame* txf, unsigned int timeout_ms);

/*
  Read a single Canard CAN Frame.
*/
int readCanardFrame(CanardInstance* canard, CanardFrame* rxf, uint8_t transport_index, CanardTransfer* transfer, unsigned int timeout_ms);

/*
  Transmit all Canard CAN Frames from the Canard queue.
*/
int transmitCanardQueue(CanardInstance* canard, int timeout_ms);

#endif // AST_CAN_VERSION