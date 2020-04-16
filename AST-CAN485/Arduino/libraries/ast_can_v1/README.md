# AST_CAN_v1

CAN driver interface for the AST-CAN485 using [ASTCanLib](https://github.com/Atlantis-Specialist-Technologies/AST_CAN_Arduino_Library/blob/master/src/ASTCanLib.h) for [UAVCAN v1](https://uavcan.org/).

## Defines

`CANARD_OK` - legacy Libcanard success return value

`ERR_INVALID_NODE_ID`, `ERR_COMMAND_ACCEPT_TIMEOUT`, `ERR_COMMAND_EXECUTE` - error code return values

`CANARD_MEMORY_POOL_SIZE` - size of memory to be allocated for Canard message queue (see [canard.h](https://github.com/UAVCAN/libcanard/blob/legacy-v0/canard.h) for details)

## Structures

`Canard` - custom struct containing CAN bitrate, Canard instance, and Canard memory pool

## Functions

`int init_can(...)` - initializes the AST-CAN485 CAN module and Canard instance, returns value for error checking

`int sendCanardFrame(...)` - sends a single CanardCANFrame from the Canard queue, returns value for error checking

`int readCanardFrame(...)` - reads a single CanardCANFrame from the Canard queue, returns value for error checking

`int transmitCanardQueue(...)` - transmitts all CanardCANFrames from the Canard queue, returns value for error checking

`void printCanard(...)` - Serial debugging print of custom `Canard` structure

`void printMsg(...)` - Serial debugging print of ASTCanLib st_cmd_t CAN message structure
