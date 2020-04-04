# Libcanard (Version 0)

Contains libcanard source code files:
* `canard.h`
* `canard.c`
* `canard_internals.h` 

Pulled from [UAVCAN/libcanard-legacy-v0](https://github.com/UAVCAN/libcanard/tree/legacy-v0).

Also provides custom Serial debugging print statements for Canard objects.

## Serial Debugging

`void printCanardFrame(...)` - prints details about a Canard frame

`void printCanardInstance(...)` - prints details about a Canard instance

`void printCanardRxTransfer(...)` -- prints details about a received Canard transfer

`void printCanardPoolAllocatorStatistics(...)` - prints details about a Canard memory pool
