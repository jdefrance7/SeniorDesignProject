# Libcanard (Version 0)

Contains libcanard source code files pulled from [UAVCAN/libcanard-legacy-v0](https://github.com/UAVCAN/libcanard/tree/legacy-v0):
* `canard.h`
* `canard.c`
* `canard_internals.h` 

Provides custom Serial debugging print statements for Canard objects.

## Serial Debugging

`void printCanardFrame(CanardCANFrame* frame)` - prints details about a Canard frame

`void printCanardInstance(CanardInstance* ins)` - prints details about a Canard instance

`void printCanardRxTransfer(CanardRxTransfer* rxt)` -- prints details about a received Canard transfer

`void printCanardPoolAllocatorStatistics(CanardPoolAllocatorStatistics* stats)` - prints details about a Canard memory pool
