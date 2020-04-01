#ifndef AVR_CAN_VERSION
#define AVR_CAN_VERSION 0

/*
  CAN driver for the AST-CAN485 board using canard_avr and Libcanard v0.
*/

#include <stdint.h> // primative data types

#include "canard_avr.h" // includes canard

#include "can_config.h" // can driver config

#define CAN_CONFIG_LOADED // compiler file flag for can.h

#include "can.h" // needed for bitrate

#include <Arduino.h> // needed for millis()

/*
  Error values
*/
#define ERR_INVALID_NODE_ID          -1
#define ERR_CAN_MODULE_INIT          -2
#define ERR_CAN_FILTER_INIT          -3
#define ERR_CANARD_AVR_TIMEOUT       1

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
  can_bitrate_t bitrate;
  CanardInstance canard;
  uint8_t canard_memory_pool[CANARD_MEMORY_POOL_SIZE];
} Canard;

/*
  Initialize the CAN module given Canard struct and Node ID.
*/
int init_can(Canard can, uint8_t id);

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
int transmitCanardQueue(CanardInstance* canard, int timeout_ms);

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

#endif // AVR_CAN_VERSION
