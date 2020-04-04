# Libcanard v0

Libcanard v0 source code files (legacy).

Files `canard.h`, `canard.c`, and `canard_internals.h` pulled from [Libcanard](https://github.com/UAVCAN/libcanard/tree/legacy-v0)

Also provides custom Serial debugging print statements.

## Serial Debugging

`void printCanardFrame(...)` - prints details about a Canard frame

`void printCanardInstance(...)` - prints details about a Canard instance

`void printCanardRxTransfer(...)` -- prints details about a received Canard transfer

`void printCanardPoolAllocatorStatistics(...)` - prints details about a Canard memory pool
