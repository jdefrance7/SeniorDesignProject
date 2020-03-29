#ifndef AST_CAN_VERSION
#define AST_CAN_VERSION 1

#include <stdint.h> // primative data types

#include <libcanard_v1.h> // canard objects

#include <ASTCanLib.h> // driver library

#include <Arduino.h> // needed for millis()

#define CANARD_MEMORY_POOL_SIZE 1024

// Canard struct
typedef struct
{
  long bitrate;
  CanardInstance canard;
  void* canard_memory_pool;
} Canard;

int init_can(Canard can, uint8_t id);

int sendCanardFrame(CanardInstance* canard, CanardFrame* txf, unsigned int timeout_ms);

int readCanardFrame(CanardInstance* canard, CanardFrame* rxf, uint8_t transport_index, CanardTransfer* transfer, unsigned int timeout_ms);

int transmitCanardQueue(CanardInstance* canard, int timeout_ms);

#endif // AST_CAN_VERSION
