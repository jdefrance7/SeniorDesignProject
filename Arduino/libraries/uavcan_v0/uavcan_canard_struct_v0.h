#ifndef UAVCAN_CANARD_STRUCT_H
#define UAVCAN_CANARD_STRUCT_H

#include <stdint.h>

#include <canard.h>

#define CANARD_MEMORY_POOL_SIZE 1024

#if defined(CANARD_AVR_H)
#include <can.h>
typedef struct
{
  can_bitrate_t bitrate; // canard_avr
  CanardInstance canard;
  uint8_t canard_memory_pool[CANARD_MEMORY_POOL_SIZE];
} Canard;
#else
typedef struct
{
  long bitrate; // ASTCanLib
  CanardInstance canard;
  uint8_t canard_memory_pool[CANARD_MEMORY_POOL_SIZE];
} Canard;
#endif

#endif // UAVCAN_CANARD_STRUCT_H
