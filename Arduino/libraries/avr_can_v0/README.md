# AVR_CAN_v0

CAN driver using Canard AVR for UAVCAN v0.

Includes source files pulled from [UAVCAN/libcanard/drivers/avr](https://github.com/UAVCAN/libcanard/tree/legacy-v0/drivers/avr) (legacy).

## Defines

`ERR_INVALID_NODE_ID`, `ERR_CAN_MODULE_INIT`, `ERR_CAN_FILTER_INIT`, `ERR_CANARD_AVR_TIMEOUT` - error code values

`CANARD_MEMORY_POOL_SIZE` - size of memory to be allocated for Canard message queue (see [canard.h](https://github.com/UAVCAN/libcanard/blob/legacy-v0/canard.h) for details)

## Structures

`Canard` - custom struct containing CAN bitrate, Canard instance, and Canard memory pool

## Functions 

`int init_can(...)` - initializes the AST-CAN485 CAN module and Canard instance, returns value for error checking

`int sendCanardCANFrame(...)` - sends a single CanardCANFrame from the Canard queue, returns value for error checking

`int readCanardCANFrame(...)` - reads a single CanardCANFrame from the Canard queue, returns value for error checking

`int transmitCanardQueue(...)` - transmitts all CanardCANFrames from the Canard queue, returns value for error checking

`void printCanard(...)` - Serial debugging print of custom `Canard` structure

`void printMsg(...)` - Serial debugging print of ASTCanLib st_cmd_t CAN message structure

## Internals

`onTransferReceived(...)` - used by Canard instance to process incomming transfer frames

`shouldAcceptTransfer(...)` - used by Canard instance to filter incomming transfer frames