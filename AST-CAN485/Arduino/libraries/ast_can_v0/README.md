# AST_CAN_v0

CAN driver interface for the AST-CAN485 using [ASTCanLib](https://github.com/Atlantis-Specialist-Technologies/AST_CAN_Arduino_Library/blob/master/src/ASTCanLib.h) for [UAVCAN v0](https://legacy.uavcan.org/) (legacy).

## Defines

`ERR_INVALID_NODE_ID`, `ERR_COMMAND_ACCEPT_TIMEOUT`, `ERR_COMMAND_EXECUTE` - error code return values

`CANARD_MEMORY_POOL_SIZE` - size of memory to be allocated for Canard message queue (see [canard.h](https://github.com/UAVCAN/libcanard/blob/legacy-v0/canard.h) for details)

## Structures

`Canard` - custom struct containing CAN bitrate, Canard instance, and Canard memory pool

```c
typedef struct
{
  long bitrate;
  CanardInstance canard;
  uint8_t canard_memory_pool[CANARD_MEMORY_POOL_SIZE];
} Canard;
```

## Functions

`int init_can(...)` - initializes the AST-CAN485 CAN module and Canard instance, returns value for error checking

`int sendCanardCANFrame(...)` - sends a single CanardCANFrame from the Canard queue, returns value for error checking

`int readCanardCANFrame(...)` - reads a single CanardCANFrame from the Canard queue, returns value for error checking

`int transmitCanardQueue(...)` - transmitts all CanardCANFrames from the Canard queue, returns value for error checking

`void printCanard(...)` - Serial debugging print of custom `Canard` structure

`void printMsg(...)` - Serial debugging print of ASTCanLib st_cmd_t CAN message structure

## Internals

`onTransferReceived(...)` - used by Canard instance to process received transfers

`shouldAcceptTransfer(...)` - used by Canard instance to filter incomming transfer frames
